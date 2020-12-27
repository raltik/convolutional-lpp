#include <stdio.h>
#include <stdlib.h>

#include "image.h"

Image *readImage(const char *filename) {
    
    char buffer[16];
    Image *img;
    FILE *fp;
    int c, rgb_comp;

    fp = fopen(filename, "rb");
    if (!fp) {
        fprintf(stderr, "Error opening the file %s.\n", filename);
        exit(-1);
    }

    if (!fgets(buffer, sizeof(buffer), fp)) {
        perror(filename);
        exit(-1);
    }
    
    //check the image format
    if (buffer[0] != 'P' || buffer[1] != '6') {
         fprintf(stderr, "Invalid image format (must be 'P6')\n");
         exit(-1);
    }

    //alloc memory form image
    img = (Image *)malloc(sizeof(Image));
    if (!img) {
         fprintf(stderr, "Unable to allocate memory\n");
         exit(1);
    }

    //check for comments
    c = getc(fp);
    while (c == '#') {
    while (getc(fp) != '\n') ;
         c = getc(fp);
    }

    ungetc(c, fp);
    //read image size information
    if (fscanf(fp, "%d %d", &img->width, &img->height) != 2) {
         fprintf(stderr, "Invalid image size (error loading '%s')\n", filename);
         exit(1);
    }

    //read rgb component
    if (fscanf(fp, "%d", &rgb_comp) != 1) {
         fprintf(stderr, "Invalid rgb component (error loading '%s')\n", filename);
         exit(1);
    }

    //check rgb component depth
    if (rgb_comp != RGB_COMPONENT_COLOR) {
         fprintf(stderr, "'%s' does not have 8-bits components\n", filename);
         exit(1);
    }

    while (fgetc(fp) != '\n');

    img->data = (PPMPixel*) malloc(img->width * img->height * sizeof(PPMPixel));

    if (!img) {
        fprintf(stderr, "Unable to allocate memory\n");
        exit(1);
    }

    if (fread(img->data, RGB_SIZE * img->width, img->height, fp) != img->height) {
         fprintf(stderr, "Error loading image '%s'\n", filename);
         exit(1);
    }

    fclose(fp);
    
    return img;
}