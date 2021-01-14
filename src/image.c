#include <stdio.h>
#include <stdlib.h>

#include "image.h"
#include "utils.h"

/*
* Generates a random image.
* W - Width of image
* H - Height of image
* C - Number of channels (RGB=3, GrayScale=1)
* ptr_image - Pointer to the image
*/
void randomImage(int W, int H, int C, float *ptr_image) {
     int addr1, addr;
     for (int c=0; c < C; c++) {
          addr1 = c * (H * W);
          for (int h=0; h < H; h++) {
               for (int w=0; w < W; w++) {
                    addr = addr1 + w + h*W;
                    ptr_image[addr] = random_float(0.0, 255.0);
               }
          }
     }
}