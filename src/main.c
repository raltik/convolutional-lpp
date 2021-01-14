#include <stdio.h>
#include <stdlib.h>

#include "image.h"

int main(int argc, char **argv) {
    
    int W = 1920;
    int H = 1080;
    int C = 3;

    float *image = (float *) malloc(W * H * C * sizeof(float));

    randomImage(W, H, C, image);

    // int addr1, addr = 0;
    // for (int c=0; c < C; c++) {
    //     printf("\nCHANNEL %2d\n\n", c);
    //       addr1 = c * (H * W);
    //       for (int h=0; h < H; h++) {
    //            for (int w=0; w < W; w++) {
    //                 addr = addr1 + w + h*W;
    //                 printf("%3.1f, ", image[addr]);
    //            }
    //            printf("\n");
    //       }
    //  }

}