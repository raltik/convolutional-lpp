#include "main.h"

#include <time.h>

#define TAG_IMAGE  1000
#define TAG_OUTPUT 1001

float *image;
float *output;

float *image_loc;
float *output_loc;

float *bias;
float *kernels;

int main(int argc, char **argv) {
    int procs, rank;
    int size_image, size_loc, size_kernels;
    int wi_proc, hi_proc;
   
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    MPI_Status status;
    MPI_Request request;

    double mytime = MPI_Wtime();
    
    wi_proc = WI;
    hi_proc = (HI / procs) + 1;
    
    bias = (float *) malloc(CO * sizeof(float));
    kernels = (float *) malloc(KERNEL_WIDTH * KERNEL_HEIGHT * CI * CO * sizeof(float));
    
    if (rank == 0) { // Root
        image = (float *) malloc(WI * HI * CI * sizeof(float));
        output = (float *) malloc(WI * HI * CO * sizeof(float));

        randomImage(WI, HI, CI, image);

        generate_random_bias(CO, bias);
        generate_random_kernels(CI, CO, kernels);

        for (int i=1; i < procs; i++) {
            int offset = (i * wi_proc * hi_proc * CI) - 2 * wi_proc * CI;
            MPI_Send(&bias, CO, MPI_FLOAT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&kernels, 3 * 3, MPI_FLOAT, i, 2, MPI_COMM_WORLD);
            MPI_Send(&image[offset], wi_proc * hi_proc * CI, MPI_FLOAT, i, 3, MPI_COMM_WORLD);
        }
        
        convolutional(wi_proc, hi_proc, CI, CO, image, output, kernels, bias);

    } else {

        image_loc = (float *) malloc(wi_proc * hi_proc * CI * sizeof(float));
        output_loc = (float *) malloc(wi_proc * hi_proc * CO * sizeof(float));

        MPI_Recv(image_loc, wi_proc * hi_proc * CI, MPI_FLOAT, 0, 3, MPI_COMM_WORLD, &status);
        MPI_Recv(bias, CO, MPI_FLOAT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(kernels, 3*3, MPI_FLOAT, 0, 2, MPI_COMM_WORLD, &status);
        
        convolutional(wi_proc, hi_proc, CI, CO, image_loc, output_loc, kernels, bias);    
    }

    MPI_Barrier(MPI_COMM_WORLD);
    mytime = MPI_Wtime() - mytime;

    if (!rank) {
        printf("\n#########################\n\t%2.4f secs.\n#########################\n", mytime);
    }

    MPI_Finalize();
}