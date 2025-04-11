#define CL_TARGET_OPENCL_VERSION 220
#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define GRID_SIZE 32768
#define ITERATIONS 100

const char* kernelSource = 
"__kernel void heat_diffusion(__global float* current, __global float* next, int N, float alpha) {"
"    int x = get_global_id(0);"
"    int y = get_global_id(1);"
"    if (x > 0 && x < N-1 && y > 0 && y < N-1) {"
"        next[y * N + x] = current[y * N + x] + alpha * ("
"            current[(y+1) * N + x] + current[(y-1) * N + x] +"
"            current[y * N + (x+1)] + current[y * N + (x-1)] -"
"            4.0f * current[y * N + x]);"
"    }"
"}";

int main() {
    const float ALPHA = 0.01f;
    int N = GRID_SIZE;
    size_t size = N * N * sizeof(float);
    float *current = (float *)malloc(size);
    float *next = (float *)malloc(size);

    for (int i = 0; i < N * N; i++) {
        current[i] = rand() / (float)RAND_MAX;
    }

    // OpenCL setup
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_kernel kernel;
    cl_mem d_current, d_next;

    clGetPlatformIDs(1, &platform, NULL);
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
    context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);
    queue = clCreateCommandQueueWithProperties(context, device, 0, NULL);

    d_current = clCreateBuffer(context, CL_MEM_READ_WRITE, size, NULL, NULL);
    d_next = clCreateBuffer(context, CL_MEM_READ_WRITE, size, NULL, NULL);

    struct timespec start, end, transfer_start, transfer_end, kernel_start, kernel_end;

    program = clCreateProgramWithSource(context, 1, &kernelSource, NULL, NULL);
    clBuildProgram(program, 1, &device, NULL, NULL, NULL);
    kernel = clCreateKernel(program, "heat_diffusion", NULL);

    clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_current);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_next);
    clSetKernelArg(kernel, 2, sizeof(int), &N);
    clSetKernelArg(kernel, 3, sizeof(float), &ALPHA);
    
    // Mesure du temps total
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    // Measure transfer time CPU -> GPU
    clock_gettime(CLOCK_MONOTONIC, &transfer_start);
    clEnqueueWriteBuffer(queue, d_current, CL_TRUE, 0, size, current, 0, NULL, NULL);
    clEnqueueWriteBuffer(queue, d_next, CL_TRUE, 0, size, next, 0, NULL, NULL);
    clock_gettime(CLOCK_MONOTONIC, &transfer_end);

    size_t globalSize[2] = {N, N};
    size_t localSize[2] = {16, 16};
	
    // Measure kernel execution time
    clock_gettime(CLOCK_MONOTONIC, &kernel_start);

    for (int t = 0; t < ITERATIONS; t++) {
        clEnqueueNDRangeKernel(queue, kernel, 2, NULL, globalSize, localSize, 0, NULL, NULL);
        clFinish(queue);

        // Swap buffers
        cl_mem temp = d_current;
        d_current = d_next;
        d_next = temp;
    }
    clock_gettime(CLOCK_MONOTONIC, &kernel_end);

    // Measure transfer time GPU -> CPU
    clock_gettime(CLOCK_MONOTONIC, &transfer_start);
    clEnqueueReadBuffer(queue, d_current, CL_TRUE, 0, size, current, 0, NULL, NULL);
    clock_gettime(CLOCK_MONOTONIC, &transfer_end);

    clock_gettime(CLOCK_MONOTONIC, &end);

    // Calculate times
    double total_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    double kernel_time = (kernel_end.tv_sec - kernel_start.tv_sec) + (kernel_end.tv_nsec - kernel_start.tv_nsec) / 1e9;
    double transfer_time_to_gpu = (transfer_end.tv_sec - transfer_start.tv_sec) + (transfer_end.tv_nsec - transfer_start.tv_nsec) / 1e9;
    double transfer_time_from_gpu = (transfer_end.tv_sec - transfer_start.tv_sec) + (transfer_end.tv_nsec - transfer_start.tv_nsec) / 1e9;
    double transfer = transfer_time_to_gpu + transfer_time_from_gpu;

    printf("Taille : %d, Iterations : %d, Temps total : %f secondes, Temps GPU : %f secondes, Temps transfert : %f secondes\n",
           N, ITERATIONS, total_time, kernel_time, transfer);

    clReleaseMemObject(d_current);
    clReleaseMemObject(d_next);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
    free(current);
    free(next);

    return 0;
}

