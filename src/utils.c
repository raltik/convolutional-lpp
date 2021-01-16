#include "main.h"

float random_float(float min, float max) {
    float scale = rand() / (float) RAND_MAX;
    return min + scale * ( max - min ); 
}
