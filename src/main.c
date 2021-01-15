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

int main(int argc, char **argv) {
    int procs, rank;
    
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);

    if (rank == 0) { // Root
        float *image = allocate_data(WI+2, HI+2, CI); //Sumamos 2 para tener en cuenta el padding.
        float *output = allocate_data(WI, HI, CO);

        randomImage(WI+2, HI+2, CI, image); // Sumamos 2 para tener en cuenta el padding.
        generate_random_bias(CO);
        generate_random_kernels(CI, CO);
    }

    MPI_Finalize();
}