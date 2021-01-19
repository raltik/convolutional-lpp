
void generate_random_bias(int COUT, float *ptr_bias);
void generate_random_kernels(int CIN, int COUT, float *ptr_kernels);
void convolutional(int W, int H, int CIN, int COUT, data_in_t *ptr_in, data_out_t *ptr_out, float *ptr_kernels, float *ptr_bias, int offset);