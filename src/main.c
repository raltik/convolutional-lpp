#include "main.h"

data_in_t *image;
data_out_t *output;

data_in_t *image_loc;
data_out_t *output_loc;

float *bias;
float *kernels;

int main(int argc, char **argv) {
    int procs, rank;
    int size_image, size_loc, size_kernels;
    int wi_proc, hi_proc;
    
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);

    // Struct MPI
    MPI_Datatype struct_mpi;
    MPI_Datatype type[1] = {MPI_FLOAT};
    int blocklen[1] = {CO};
    MPI_Aint disps[1] = {0};

    MPI_Type_create_struct(1, blocklen, disps, type, &struct_mpi);
    MPI_Type_commit(&struct_mpi);
    //

    MPI_Status status;

    wi_proc = WI;
    hi_proc = (HI / 2) + 1;
    
    image_loc = (data_in_t *) malloc(wi_proc * hi_proc * sizeof(data_in_t));
    output_loc = (data_out_t *) malloc(wi_proc * hi_proc * sizeof(data_out_t));
    bias = (float *) malloc(CO * sizeof(float));
    kernels = (float *) malloc(KERNEL_WIDTH * KERNEL_HEIGHT * CI * CO * sizeof(float));

    generate_random_bias(CO, bias);
    generate_random_kernels(CI, CO, kernels);

    size_loc = wi_proc * hi_proc;
    size_kernels = KERNEL_HEIGHT * KERNEL_WIDTH * CI * CO;
    size_image = WI * HI * CI;
    
    if (rank == 0) { // Root
        
        image = (data_in_t *) malloc(WI * HI * sizeof(data_in_t));
        output = (data_out_t *) malloc(WI * HI * sizeof(data_out_t));

        randomImage(WI, HI, CI, image); // Sumamos 2 para tener en cuenta el padding.

        MPI_Send(&image, wi_proc * hi_proc, struct_mpi, 1, 1234, MPI_COMM_WORLD);
    } else {
        MPI_Recv(&image_loc, wi_proc * hi_proc, struct_mpi, 0, 1234, MPI_COMM_WORLD, &status);

        printf("%f\n", image_loc[0].channels[0]);
    }  


    MPI_Finalize();
}