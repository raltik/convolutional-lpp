#define RGB_SIZE 3

typedef struct {
    unsigned char red, green, blue;
} PPMPixel;

typedef struct {
    int width, height;
    PPMPixel *data;
} Image;

#define RGB_COMPONENT_COLOR 255

Image *readImage(const char *filename);