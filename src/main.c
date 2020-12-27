#include <stdio.h>

#include "image.h"

int main(int argc, char **argv) {
    
    const char *file = "./dog.ppm";
    Image *img = readImage(file);


    //TEST
    
    int H = img->height;
    int W = img->width;

    for (int h=0; h < 2; h++) {
        for (int w=0; w < 10; w++) {
            int index = w + (h * W);
            
            PPMPixel pix = img->data[index];

            printf("Index: %3d\n", index);
            printf("\tRed: %3d\n", pix.red);
            printf("\tGreen: %3d\n", pix.green);
            printf("\tBlue: %3d\n", pix.blue);

        }
    }

}