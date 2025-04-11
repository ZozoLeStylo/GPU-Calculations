#define CL_TARGET_OPENCL_VERSION 220
#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DATA_SIZE 65536
#define THREADS_PER_BLOCK 256

const char* kernelSource = 
"__kernel void reduction(__global float* A, __global float* result, int N) {"
"    __local float partialSum[256];"
"    int tid = get_global_id(0);"
"    int local_tid = get_local_id(0);"
"    partialSum[local_tid] = (tid < N) ? A[tid] : 0.0f;"
"    barrier(CLK_LOCAL_MEM_FENCE);"
"    for (int stride = get_local_size(0) / 2; stride > 0; stride /= 2) {"
"        if (local_tid < stride) {"
"            partialSum[local_tid] += partialSum[local_tid + stride];"
"        }"
"        barrier(CLK_LOCAL_MEM_FENCE);"
"    }"
"    if (local_tid == 0) {"
"        atomic_add(result, partialSum[0]);"
"    }"
"}";

int main() {
    int N = DATA_SIZE;
    size_t size = N * sizeof(float);
    float *A = (float *)malloc(size);
    float result = 0.0f;
    struct timespec start, end, transfer_start_1, transfer_end_1, kernel_start, kernel_end, transfer_start_2, transfer_end_2;

    // Initialisation des données
    for (int i = 0; i < N; i++) {
        A[i] = rand() / (float)RAND_MAX;
    }

    // Initialisation OpenCL
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_kernel kernel;
    cl_mem d_A, d_result;

    clGetPlatformIDs(1, &platform, NULL);
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
    context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);
    queue = clCreateCommandQueueWithProperties(context, device, 0, NULL);

    // Création des buffers
    d_A = clCreateBuffer(context, CL_MEM_READ_ONLY, size, NULL, NULL);
    d_result = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float), NULL, NULL);

    clock_gettime(CLOCK_MONOTONIC, &start);

    // Transfert CPU -> GPU
    clock_gettime(CLOCK_MONOTONIC, &transfer_start_1);
    clEnqueueWriteBuffer(queue, d_A, CL_TRUE, 0, size, A, 0, NULL, NULL);
    clEnqueueWriteBuffer(queue, d_result, CL_TRUE, 0, sizeof(float), &result, 0, NULL, NULL);
    clock_gettime(CLOCK_MONOTONIC, &transfer_end_1);

    // Compilation et configuration du kernel
    program = clCreateProgramWithSource(context, 1, &kernelSource, NULL, NULL);
    clBuildProgram(program, 1, &device, NULL, NULL, NULL);
    kernel = clCreateKernel(program, "reduction", NULL);

    clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_A);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_result);
    clSetKernelArg(kernel, 2, sizeof(int), &N);

    size_t globalSize = (N + THREADS_PER_BLOCK - 1) / THREADS_PER_BLOCK * THREADS_PER_BLOCK;
    size_t localSize = THREADS_PER_BLOCK;

    clock_gettime(CLOCK_MONOTONIC, &kernel_start);

    // Exécution du kernel
    clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalSize, &localSize, 0, NULL, NULL);
    clFinish(queue);

    clock_gettime(CLOCK_MONOTONIC, &kernel_end);

    // Transfert GPU -> CPU
    clock_gettime(CLOCK_MONOTONIC, &transfer_start_2);
    clEnqueueReadBuffer(queue, d_result, CL_TRUE, 0, sizeof(float), &result, 0, NULL, NULL);
    clock_gettime(CLOCK_MONOTONIC, &transfer_end_2);

    clock_gettime(CLOCK_MONOTONIC, &end);

    // Calcul des temps
    double total_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    double kernel_time = (kernel_end.tv_sec - kernel_start.tv_sec) + (kernel_end.tv_nsec - kernel_start.tv_nsec) / 1e9;
    double transfer_time_1 = (transfer_end_1.tv_sec - transfer_start_1.tv_sec) + (transfer_end_1.tv_nsec - transfer_start_1.tv_nsec) / 1e9;
    double transfer_time_2 = (transfer_end_2.tv_sec - transfer_start_2.tv_sec) + (transfer_end_2.tv_nsec - transfer_start_2.tv_nsec) / 1e9;
    double transfer = transfer_time_1 + transfer_time_2;

    // Affichage des résultats
    printf("Taille : %d, Temps total : %f secondes, Temps GPU : %f secondes, Temps transfert : %f secondes, Résultat : %f\n",
           N, total_time, kernel_time, transfer, result);

    // Libération des ressources
    clReleaseMemObject(d_A);
    clReleaseMemObject(d_result);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
    free(A);

    return 0;
}

