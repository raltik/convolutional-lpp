#include <time.h>
#include <stdlib.h>
#include <stdio.h>

float random_float(float min, float max) {
    float scale = rand() / (float) RAND_MAX;
    return min + scale * ( max - min ); 
}

float **allocate_data(int width, int height, int channels) {
    float **ptr;

    ptr = (float **) malloc(width * height * sizeof(float*));
    
    if (ptr == NULL) {
        fprintf(stderr, "Error allocating memory.\n");
        exit(-1);
    }

    for (int i=0; i < width * height; i++) {
        ptr[i] = (float *) malloc(channels * sizeof(float));
    }

    return ptr;    
}