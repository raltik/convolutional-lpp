#include <stdio.h>
#include <stdlib.h>

#include "conv.h"
#include "utils.h"

/*
* Allocates and generates random values for the bias.
* 
*   COUT - Channels OUT
*/
void generate_random_bias(int COUT) {
    int size = CHANNELS_OUT;

    bias = (float *) malloc(size * sizeof(float));

    for (int co=0; co < size; co++) {
        bias[co] = random_float(-1.0, 1.0); 
    }
}

/*
* Allocates and generates random values for the weights.
* 
*   CIN  - Channels IN
*   COUT - Channels OUT
*/
void generate_random_kernels(int CIN, int COUT) {
    int size = KERNEL_HEIGHT * KERNEL_WIDTH * COUT * CIN;

    kernels = (float *) malloc(size * sizeof(float));

    for (int i=0; i < size; i++) {
        kernels[i] = random_float(-1.0, 1.0);
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
void convolutional(int W, int H, int CIN, int COUT, float *ptr_in, float *ptr_out) {
    int addr_in, addr_out;

    int WIDTH_OUT  = (int) ((W - KERNEL_WIDTH + 2 * PADDING_WIDTH) / STRIDE_WIDTH + 1);
    int HEIGHT_OUT = (int) ((H - KERNEL_HEIGHT + 2 * PADDING_HEIGHT) / STRIDE_HEIGHT + 1);

    for (int co=0; co < COUT; co++) {
        for (int ci=0; ci < CIN; ci++) {
            
            for (int hi=0; hi < HEIGHT_OUT; hi++) {
                for (int wi=0; wi < WIDTH_OUT; wi++) {
                    addr_out = wi + hi * WIDTH_OUT + co * (WIDTH_OUT * HEIGHT_OUT);

                    for (int kh=0; kh < KERNEL_HEIGHT; kh++) {
                        for (int kw=0; kw < KERNEL_WIDTH; kw++) {
                            int idx_h = (hi * STRIDE_HEIGHT) + kh;
                            int idx_w = (wi * STRIDE_WIDTH) + kw;
                            int idx = idx_w + idx_h * W;
                            addr_in = idx + (ci * W * H);

                            int addr_k = kh + kw + (co * KERNEL_HEIGHT * KERNEL_WIDTH) + (ci * KERNEL_HEIGHT * KERNEL_WIDTH);

                            ptr_out[addr_out] += ptr_in[addr_in] * kernels[addr_k];
                        }
                    }

                    ptr_out[addr_out] += bias[co]; // Add bias
                }
            }
        }
    }
}