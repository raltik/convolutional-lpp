#include <time.h>
#include <stdlib.h>
#include <stdio.h>

float random_float(float min, float max) {
    float scale = rand() / (float) RAND_MAX;
    return min + scale * ( max - min ); 
}

float *allocate_data(int width, int height, int channels) {
    float *ptr;
    int size = width * height * channels;

    ptr = (float *) malloc(size * sizeof(float));
    
    if (ptr == NULL) {
        fprintf(stderr, "Error allocating memory.\n");
        exit(-1);
    }

    return ptr;    
}