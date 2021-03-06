#include "main.h"

/*
* Generates a random image.
* W - Width of image
* H - Height of image
* C - Number of channels (RGB=3, GrayScale=1)
* ptr_image - Pointer to the image
*/
void randomImage(int W, int H, int C, float *ptr_image) {
     int addr;
     for (int h=0; h < H; h++) {
          for (int w=0; w < W; w++) {
               for (int c=0; c < C; c++) {
                    addr = c + (w + h*W);
                    if (!h || !w || w==W-1 || h == H-1) ptr_image[addr] = 0.0;
                    else ptr_image[addr] = random_float(0.0, 255.0);
               }
          }
     }
}