#include "main.h"

#include <time.h>

#define TAG_IMAGE   1000
#define TAG_OUTPUT  1001

float *image;
float *output;

float *image_loc;
float *output_loc;

float *bias;
float *kernels;

int main(int argc, char **argv) {
    int procs, rank, threads;
    int size_image, size_loc, size_kernels;
    int wi_proc, hi_proc, hi_comm;
   
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    MPI_Status status;
    MPI_Request request;

    #pragma omp parallel
    threads = omp_get_num_threads();

    if (!rank) {
        printf("MPI Processes: %2d\n", procs);
        printf("Threads per Proc: %2d\n\n", threads);
    }

    double mytime = MPI_Wtime();
    
    wi_proc = WI;
    hi_comm = HI / procs;

    if (!rank) hi_proc = hi_comm + 1;
    else if (rank == procs - 1) hi_proc = hi_comm + 1;
    else hi_proc = hi_comm + 2;
    
    bias = (float *) malloc(CO * sizeof(float));
    kernels = (float *) malloc(KERNEL_WIDTH * KERNEL_HEIGHT * CI * CO * sizeof(float));

    if (!rank) { // Root
        image = (float *) malloc(WI * (HI+1) * CI * sizeof(float));
        output = (float *) malloc(WI * (HI+1) * CO * sizeof(float));

        randomImage(WI, (HI+1), CI, image);

        generate_random_bias(CO, bias);
        generate_random_kernels(CI, CO, kernels);

        int size_image_loc;
        for (int i=1; i < procs; i++) {
            size_image_loc = WI * (hi_comm + 2) * CI;
            if (i == procs - 1) size_image_loc = WI * (hi_comm + 1) * CI;
             
            int offset = (i * (hi_comm) - 1) * wi_proc * CI; 
            MPI_Send(&image[offset], size_image_loc, MPI_FLOAT, i, TAG_IMAGE, MPI_COMM_WORLD);
        }
    } else {
        image_loc = (float *) malloc(wi_proc * hi_proc * CI * sizeof(float));
        output_loc = (float *) malloc(wi_proc * hi_proc * CO * sizeof(float));

        MPI_Recv(image_loc, wi_proc * hi_proc * CI, MPI_FLOAT, 0, TAG_IMAGE, MPI_COMM_WORLD, &status);
    }

    MPI_Bcast(kernels, 3 * 3 * CI * CO, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Bcast(bias, CO, MPI_FLOAT, 0, MPI_COMM_WORLD);
    
    printf("[P%d] - Starting CONV...\n", rank);

    if (!rank) {
        convolutional(wi_proc, hi_proc, CI, CO, image, output, kernels, bias);
    } else {
        convolutional(wi_proc, hi_proc, CI, CO, image_loc, output_loc, kernels, bias);    
    }

    MPI_Barrier(MPI_COMM_WORLD);
    mytime = MPI_Wtime() - mytime;

    if (!rank) {
        printf("\n#########################\n\t%2.4f secs.\n#########################\n", mytime);
    }

    MPI_Finalize();
}