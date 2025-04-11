#define CL_TARGET_OPENCL_VERSION 220
#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define INF INT_MAX
#define VERTICES 15000
#define EDGES 112492500

const char* kernelSource =
"__kernel void bellman_ford_kernel(__global int *dist, __global int *src, __global int *dest, __global int *weight, int edges) {"
"    int id = get_global_id(0);"
"    if (id < edges) {"
"        int u = src[id];"
"        int v = dest[id];"
"        int w = weight[id];"
"        if (dist[u] != 2147483647 && dist[u] + w < dist[v]) {"
"            atomic_min(&dist[v], dist[u] + w);"
"        }"
"    }"
"}";


int main() {
    int vertices = VERTICES;
    int edges = EDGES;
    int *h_dist = (int *)malloc(vertices * sizeof(int));
    int *h_src = (int *)malloc(edges * sizeof(int));
    int *h_dest = (int *)malloc(edges * sizeof(int));
    int *h_weight = (int *)malloc(edges * sizeof(int));
    struct timespec start, end, transfer_start_1, transfer_end_1, kernel_start, kernel_end, transfer_start_2, transfer_end_2;

    // Génération aléatoire des arêtes
    srand(0);
    for (int i = 0; i < edges; i++) {
        h_src[i] = rand() % vertices;
        h_dest[i] = rand() % vertices;
        h_weight[i] = rand() % 20 + 1;
    }
    for (int i = 0; i < vertices; i++) {
        h_dist[i] = INF;
    }
    h_dist[0] = 0; // Sommet source

    // Initialisation OpenCL
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_kernel kernel;
    cl_mem d_dist, d_src, d_dest, d_weight;
    cl_int err;

    err = clGetPlatformIDs(1, &platform, NULL);
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
    context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
    queue = clCreateCommandQueueWithProperties(context, device, 0, &err);

    // Allocation mémoire sur le GPU
    d_dist = clCreateBuffer(context, CL_MEM_READ_WRITE, vertices * sizeof(int), NULL, &err);
    d_src = clCreateBuffer(context, CL_MEM_READ_ONLY, edges * sizeof(int), NULL, &err);
    d_dest = clCreateBuffer(context, CL_MEM_READ_ONLY, edges * sizeof(int), NULL, &err);
    d_weight = clCreateBuffer(context, CL_MEM_READ_ONLY, edges * sizeof(int), NULL, &err);

    

    // Compilation du kernel
    program = clCreateProgramWithSource(context, 1, &kernelSource, NULL, &err);
    clBuildProgram(program, 1, &device, NULL, NULL, NULL);
    kernel = clCreateKernel(program, "bellman_ford_kernel", &err);

    // Arguments du kernel
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_dist);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_src);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &d_dest);
    clSetKernelArg(kernel, 3, sizeof(cl_mem), &d_weight);
    clSetKernelArg(kernel, 4, sizeof(int), &edges);
    
    // Calcul Temps total
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Transfert des données vers le GPU
    clock_gettime(CLOCK_MONOTONIC, &transfer_start_1);
    clEnqueueWriteBuffer(queue, d_dist, CL_TRUE, 0, vertices * sizeof(int), h_dist, 0, NULL, NULL);
    clEnqueueWriteBuffer(queue, d_src, CL_TRUE, 0, edges * sizeof(int), h_src, 0, NULL, NULL);
    clEnqueueWriteBuffer(queue, d_dest, CL_TRUE, 0, edges * sizeof(int), h_dest, 0, NULL, NULL);
    clEnqueueWriteBuffer(queue, d_weight, CL_TRUE, 0, edges * sizeof(int), h_weight, 0, NULL, NULL);
    clock_gettime(CLOCK_MONOTONIC, &transfer_end_1);

    // Lancer les itérations
    size_t globalSize = edges;
    clock_gettime(CLOCK_MONOTONIC, &kernel_start);
    for (int i = 0; i < vertices - 1; i++) {
        clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalSize, NULL, 0, NULL, NULL);
        clFinish(queue);
    }
    clock_gettime(CLOCK_MONOTONIC, &kernel_end);

    // Récupérer les résultats
    clock_gettime(CLOCK_MONOTONIC, &transfer_start_2);
    clEnqueueReadBuffer(queue, d_dist, CL_TRUE, 0, vertices * sizeof(int), h_dist, 0, NULL, NULL);
    clock_gettime(CLOCK_MONOTONIC, &transfer_end_2);
	
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    double total_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    double kernel_time = (kernel_end.tv_sec - kernel_start.tv_sec) + (kernel_end.tv_nsec - kernel_start.tv_nsec) / 1e9;
    double transfer_time_to_gpu = (transfer_end_1.tv_sec - transfer_start_1.tv_sec) + (transfer_end_1.tv_nsec - transfer_start_1.tv_nsec) / 1e9;
    double transfer_time_from_gpu = (transfer_end_2.tv_sec - transfer_start_2.tv_sec) + (transfer_end_2.tv_nsec - transfer_start_2.tv_nsec) / 1e9;
    double transfer = transfer_time_to_gpu + transfer_time_from_gpu;

    printf("Temps total : %f secondes, Temps GPU : %f secondes, Temps transfert : %f secondes, Distance du sommet %d au sommet %d : %d\n", total_time, kernel_time, transfer, 0, vertices - 1, h_dist[vertices - 1]);

    // Nettoyage
    clReleaseMemObject(d_dist);
    clReleaseMemObject(d_src);
    clReleaseMemObject(d_dest);
    clReleaseMemObject(d_weight);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
    free(h_dist);
    free(h_src);
    free(h_dest);
    free(h_weight);

    return 0;
}

