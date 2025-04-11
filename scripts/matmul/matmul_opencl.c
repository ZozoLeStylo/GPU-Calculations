#define CL_TARGET_OPENCL_VERSION 220
#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MATRIX_SIZE 16384

const char* kernelSource = "__kernel void matmul(__global float* A, __global float* B, __global float* C, int n) {"
                            "    int row = get_global_id(1);"
                            "    int col = get_global_id(0);"
                            "    float value = 0.0;"
                            "    for (int k = 0; k < n; k++) {"
                            "        value += A[row * n + k] * B[k * n + col];"
                            "    }"
                            "    C[row * n + col] = value;"
                            "}";

int main() {
    int N = MATRIX_SIZE;
    size_t size = N * N * sizeof(float);
    float *A = (float *)malloc(size);
    float *B = (float *)malloc(size);
    float *C = (float *)malloc(size);

    for (int i = 0; i < N * N; i++) {
        A[i] = rand() / (float)RAND_MAX;
        B[i] = rand() / (float)RAND_MAX;
    }

    struct timespec start, end;
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_kernel kernel;
    cl_mem d_A, d_B, d_C;

    // Initialisation OpenCL
    clGetPlatformIDs(1, &platform, NULL);
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
    context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);
    queue = clCreateCommandQueueWithProperties(context, device, 0, NULL);

    d_A = clCreateBuffer(context, CL_MEM_READ_ONLY, size, NULL, NULL);
    d_B = clCreateBuffer(context, CL_MEM_READ_ONLY, size, NULL, NULL);
    d_C = clCreateBuffer(context, CL_MEM_WRITE_ONLY, size, NULL, NULL);

    // Compilation et exécution du kernel
    program = clCreateProgramWithSource(context, 1, &kernelSource, NULL, NULL);
    clBuildProgram(program, 1, &device, NULL, NULL, NULL);
    kernel = clCreateKernel(program, "matmul", NULL);

    clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_A);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_B);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &d_C);
    clSetKernelArg(kernel, 3, sizeof(int), &N);
    
    // Mesure du temps total
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Transfert CPU -> GPU
    struct timespec transfer_start_1, transfer_end_1;
    clock_gettime(CLOCK_MONOTONIC, &transfer_start_1);

    clEnqueueWriteBuffer(queue, d_A, CL_TRUE, 0, size, A, 0, NULL, NULL);
    clEnqueueWriteBuffer(queue, d_B, CL_TRUE, 0, size, B, 0, NULL, NULL);

    clock_gettime(CLOCK_MONOTONIC, &transfer_end_1);

    size_t globalSize[2] = {N, N};
    size_t localSize[2] = {16, 16};

    struct timespec kernel_start, kernel_end;
    clock_gettime(CLOCK_MONOTONIC, &kernel_start);

    clEnqueueNDRangeKernel(queue, kernel, 2, NULL, globalSize, localSize, 0, NULL, NULL);
    clFinish(queue);

    clock_gettime(CLOCK_MONOTONIC, &kernel_end);

    // Transfert GPU -> CPU
    struct timespec transfer_start_2, transfer_end_2;
    clock_gettime(CLOCK_MONOTONIC, &transfer_start_2);

    clEnqueueReadBuffer(queue, d_C, CL_TRUE, 0, size, C, 0, NULL, NULL);

    clock_gettime(CLOCK_MONOTONIC, &transfer_end_2);

    clock_gettime(CLOCK_MONOTONIC, &end);

    // Calculs des temps
    double total_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    double kernel_time = (kernel_end.tv_sec - kernel_start.tv_sec) + (kernel_end.tv_nsec - kernel_start.tv_nsec) / 1e9;
    double transfer_time_1 = (transfer_end_1.tv_sec - transfer_start_1.tv_sec) + (transfer_end_1.tv_nsec - transfer_start_1.tv_nsec) / 1e9;
    double transfer_time_2 = (transfer_end_2.tv_sec - transfer_start_2.tv_sec) + (transfer_end_2.tv_nsec - transfer_start_2.tv_nsec) / 1e9;
    double transfer = transfer_time_1 + transfer_time_2;

    printf("Taille : %d, Temps total : %f secondes, Temps GPU : %f secondes, Temps transfert : %f secondes\n",
           N, total_time, kernel_time, transfer);

    clReleaseMemObject(d_A);
    clReleaseMemObject(d_B);
    clReleaseMemObject(d_C);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
    free(A);
    free(B);
    free(C);

    return 0;
}

