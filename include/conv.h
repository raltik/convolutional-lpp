#define CHANNELS_OUT 16

#define KERNEL_WIDTH  3
#define KERNEL_HEIGHT 3
#define STRIDE_WIDTH  1
#define STRIDE_HEIGHT 1
#define PADDING_WIDTH 1
#define PADDING_HEIGHT 1

float *bias;
float *kernels;

void generate_random_bias(int COUT);
void generate_random_kernels(int CIN, int COUT);
void convolutional(int W, int H, int CIN, int COUT, float *ptr_in, float *ptr_out);