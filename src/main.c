#include <stdio.h>
#include <stdlib.h>

#include "image.h"
#include "conv.h"

int main(int argc, char **argv) {
    
    int W = 1920;
    int H = 1080;
    int C = 3;
    int CO = 64;

    float *image = (float *) malloc(W * H * C * sizeof(float));
    float *output = (float *) malloc(W * H * CO * sizeof(float));

    randomImage(W, H, C, image);

    generate_random_bias(CO);
    generate_random_kernels(C, CO);

    convolutional(W, H, C, CO, image, output);



}