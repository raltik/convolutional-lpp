#include "main.h"

data_in_t *image;
data_out_t *output;

float *bias;
float *kernels;

int main(int argc, char **argv) {
    int procs, rank;
    int size_image, size_loc, size_kernels;
    int wi_proc, hi_proc;
   
   
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);

    MPI_Datatype in_struct, out_struct;
    MPI_Datatype type[1] = {MPI_FLOAT};
    int blocklen[1] = {CI};
    MPI_Aint disps[1] = {0};

    MPI_Type_create_struct(1, blocklen, disps, type, &in_struct);
    MPI_Type_commit(&in_struct);

    blocklen[0] = CO;
    MPI_Type_create_struct(1, blocklen, disps, type, &out_struct);
    MPI_Type_commit(&out_struct);


    wi_proc = WI;
    hi_proc = (HI / 2) + 1;

    data_in_t image_loc[wi_proc * hi_proc];
    data_out_t output_loc[wi_proc * hi_proc];

    MPI_Status status;
    
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
        
        for (int i=1; i < procs; i++) {
            int offset = (i * wi_proc * hi_proc) - 2 * wi_proc;
            
            MPI_Ssend(&image[offset], wi_proc * hi_proc, in_struct, i, 1234, MPI_COMM_WORLD);
        }

        convolutional(wi_proc, hi_proc, CI, CO, image, output, kernels, bias, 0);

        for (int i=1; i < procs; i++) {
            int offset = (i * wi_proc * hi_proc) - 2 * wi_proc;

            MPI_Recv(&output[offset], wi_proc * hi_proc, out_struct, i, 1234, MPI_COMM_WORLD, &status);
        }

    } else {
        
        MPI_Recv(&image_loc, wi_proc * hi_proc, in_struct, 0, 1234, MPI_COMM_WORLD, &status);
        
        convolutional(wi_proc, hi_proc, CI, CO, image_loc, output_loc, kernels, bias, 1);

        MPI_Ssend(&output_loc, wi_proc * hi_proc, out_struct, 0, 1234, MPI_COMM_WORLD);
        
    }  


    MPI_Finalize();
}