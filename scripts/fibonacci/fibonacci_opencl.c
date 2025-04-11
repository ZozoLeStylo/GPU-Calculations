#define CL_TARGET_OPENCL_VERSION 220
#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const char* kernelSource =
"__kernel void test_kernel(int n, __global unsigned long long *solutions) {"
"    int col = get_global_id(0);"
"    if (col >= n) return;" // Chaque work-item doit s'exécuter"
""
"    // Ajout d'une valeur fixe pour chaque work-item exécuté"
"    *solutions = n;"
"}";


int main() {
    int n = 8; // Taille de l'échiquier (valeurs faibles pour validation initiale)
    unsigned long long h_solutions = 0;
    struct timespec start, end;

    // Initialisation OpenCL
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_kernel kernel;
    cl_mem d_solutions;

    clGetPlatformIDs(1, &platform, NULL);
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
    context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);
    queue = clCreateCommandQueueWithProperties(context, device, 0, NULL);

    // Allocation mémoire
    d_solutions = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(unsigned long long), NULL, NULL);

    // Chargement du kernel
    program = clCreateProgramWithSource(context, 1, &kernelSource, NULL, NULL);
    clBuildProgram(program, 1, &device, NULL, NULL, NULL);
    kernel = clCreateKernel(program, "test_kernel", NULL);

    // Arguments du kernel
    clSetKernelArg(kernel, 0, sizeof(int), &n);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_solutions);

    // Mesure du temps total
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Initialiser les solutions à zéro
    clEnqueueWriteBuffer(queue, d_solutions, CL_TRUE, 0, sizeof(unsigned long long), &h_solutions, 0, NULL, NULL);

    // Lancer le kernel
    size_t globalSize = n; // Un work-item par colonne
    clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalSize, NULL, 0, NULL, NULL);
    clFinish(queue);

    // Récupérer le résultat
    clEnqueueReadBuffer(queue, d_solutions, CL_TRUE, 0, sizeof(unsigned long long), &h_solutions, 0, NULL, NULL);

    clock_gettime(CLOCK_MONOTONIC, &end);

    // Calcul du temps
    double total_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    // Affichage des résultats
    printf("Nombre attendu : %d, nombre calculé : %llu\n", n, h_solutions);
    printf("Temps total : %f secondes\n", total_time);

    // Nettoyage
    clReleaseMemObject(d_solutions);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    return 0;
}

