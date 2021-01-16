#define WI 16
#define HI 16
#define CI 1
#define CO 1

#define KERNEL_WIDTH  3
#define KERNEL_HEIGHT 3
#define STRIDE_WIDTH  1
#define STRIDE_HEIGHT 1
#define PADDING_WIDTH 1
#define PADDING_HEIGHT 1

typedef struct {
    float channels[CI];
} data_in_t;

typedef struct {
    float channels[CO];
} data_out_t;