#include "main.h"

/*
* Allocates and generates random values for the bias.
* 
*   COUT - Channels OUT
*/
void generate_random_bias(int COUT, float *ptr_bias) {
    int size = COUT;

    for (int co=0; co < size; co++) {
        ptr_bias[co] = random_float(-1.0, 1.0); 
    }
}

/*
* Allocates and generates random values for the weights.
* 
*   CIN  - Channels IN
*   COUT - Channels OUT
*/
void generate_random_kernels(int CIN, int COUT, float *ptr_kernels) {
    int size = KERNEL_HEIGHT * KERNEL_WIDTH * COUT * CIN;

    for (int i=0; i < size; i++) {
        ptr_kernels[i] = random_float(-1.0, 1.0);
    }
}

/*
* Convolutional of the input image, 
* It uses the global pointers of bias and kernels
*   
* W - Width of input image
* H - Height of input image
* CIN - Channels IN
* COUT - Channels OUT
* ptr_in - Pointer to the input image
* ptr_out - Pointer to the output
*/
void convolutional(int W, int H, int CIN, int COUT, float *ptr_in, float *ptr_out, float *ptr_kernels, float *ptr_bias) {
    
    int addr_out, addr_k;
    int idx_h, idx_w, idx;
    int ci, co, hi, wi, kh, kw;


    int WIDTH_OUT  = (int) ((W - KERNEL_WIDTH + 2 * PADDING_WIDTH) / STRIDE_WIDTH + 1);
    int HEIGHT_OUT = (int) ((H - KERNEL_HEIGHT + 2 * PADDING_HEIGHT) / STRIDE_HEIGHT + 1);

    #pragma omp parallel for private(ci, co, hi, wi, kh, kw, addr_out, idx_h, idx_w, idx, addr_k)
    for (ci=0; ci < CIN; ci++) {
        for (co=0; co < COUT; co++) {
            addr_out = 0;
            for (hi=0; hi < HEIGHT_OUT; hi++) { 
                for (wi=0; wi < WIDTH_OUT; wi++) {
                    addr_out = co + (wi + hi * WIDTH_OUT);
                    
                    for (kh=0; kh < KERNEL_HEIGHT; kh++) {
                        for (kw=0; kw < KERNEL_WIDTH; kw++) {
                            idx_h = (hi * STRIDE_HEIGHT) + kh;
                            idx_w = (wi * STRIDE_WIDTH) + kw;
                            idx = ci + (idx_w + idx_h * W);

                            addr_k = kh + kw + (co * KERNEL_HEIGHT * KERNEL_WIDTH) + (ci * KERNEL_HEIGHT * KERNEL_WIDTH);

                            ptr_out[addr_out] += ptr_in[idx] * ptr_kernels[addr_k];
                        }
                    }
                    ptr_out[addr_out] += ptr_bias[co]; // Add bias
                }
            }
        }
    }
}