#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#include "image.h"
#include "conv.h"
#include "utils.h"

#define WI 10
#define HI 10
#define CI 1
#define CO 1

float **image;
float **output;
float **image_loc;
float **output_loc;
float *bias;
float *kernels;

int main(int argc, char **argv) {
    int procs, rank;
    int width_proc, height_proc;
    int size_image, size_loc, size_kernels;
    
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);

    MPI_Status status;

    width_proc = (WI / procs) + 1;
    height_proc = HI + 2;
    
    image_loc = allocate_data(width_proc, height_proc, CI);
    output_loc = allocate_data(width_proc, height_proc, CO);
    bias = (float *) malloc(CO * sizeof(float));
    kernels = (float *) malloc(KERNEL_WIDTH * KERNEL_HEIGHT * CI * CO * sizeof(float));

    generate_random_bias(CO, bias);
    generate_random_kernels(CI, CO, kernels);

    size_loc = width_proc * height_proc * CI;
    size_kernels = KERNEL_HEIGHT * KERNEL_WIDTH * CI * CO;

    if (rank == 0) { // Root
        int size_image = (WI + 2) * (HI + 2) * CI;
        
        image = allocate_data(WI + 2, HI + 2, CI);
        output = allocate_data(WI + 2, HI + 2, CO);

        randomImage(WI+2, HI+2, CI, image); // Sumamos 2 para tener en cuenta el padding.
        
        for (int i=1; i < procs; i++) {
            int offset = i * width_proc * height_proc;
            MPI_Send(&image[offset], size_loc, MPI_FLOAT, i, 1, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(&image_loc, size_loc, MPI_FLOAT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        convolutional(width_proc, height_proc, CI, CO, image_loc, output_loc, kernels, bias);
    }

    


    MPI_Finalize();
}