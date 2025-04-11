#include <cuda_runtime.h>
#include <stdio.h>
#include <time.h>

#define DATA_SIZE 1024
#define THREADS_PER_BLOCK 256

__global__ void reduction_kernel(const float *A, float *result, int N) {
    __shared__ float partialSum[THREADS_PER_BLOCK];
    int tid = threadIdx.x + blockIdx.x * blockDim.x;
    int local_tid = threadIdx.x;

    partialSum[local_tid] = (tid < N) ? A[tid] : 0.0f;
    __syncthreads();

    for (int stride = blockDim.x / 2; stride > 0; stride /= 2) {
        if (local_tid < stride) {
            partialSum[local_tid] += partialSum[local_tid + stride];
        }
        __syncthreads();
    }

    if (local_tid == 0) {
        atomicAdd(result, partialSum[0]);
    }
}

int main() {
    int N = DATA_SIZE;
    size_t size = N * sizeof(float);
    float *A, *d_A, *d_result;
    float result = 0.0f;
    struct timespec start, end;
    cudaEvent_t startKernel, stopKernel;
    float kernelTime;

    // Allocation sur l'hôte
    A = (float *)malloc(size);
    for (int i = 0; i < N; i++) {
        A[i] = rand() / (float)RAND_MAX;
    }

    // Allocation sur le GPU
    cudaMalloc((void **)&d_A, size);
    cudaMalloc((void **)&d_result, sizeof(float));

    clock_gettime(CLOCK_MONOTONIC, &start);

    // Transfert CPU -> GPU
    cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_result, &result, sizeof(float), cudaMemcpyHostToDevice);

    // Mesure du kernel
    cudaEventCreate(&startKernel);
    cudaEventCreate(&stopKernel);
    cudaEventRecord(startKernel, 0);

    // Lancement du kernel
    int blocks = (N + THREADS_PER_BLOCK - 1) / THREADS_PER_BLOCK;
    reduction_kernel<<<blocks, THREADS_PER_BLOCK>>>(d_A, d_result, N);

    cudaEventRecord(stopKernel, 0);
    cudaEventSynchronize(stopKernel);
    cudaEventElapsedTime(&kernelTime, startKernel, stopKernel);

    // Transfert GPU -> CPU
    cudaMemcpy(&result, d_result, sizeof(float), cudaMemcpyDeviceToHost);

    clock_gettime(CLOCK_MONOTONIC, &end);

    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    double transferTime = elapsed - (kernelTime / 1000.0);

    printf("Taille : %d, Temps total : %f secondes, Temps GPU : %f secondes, Temps transfert : %f secondes, Résultat : %f\n",
           N, elapsed, kernelTime / 1000.0, transferTime, result);

    cudaFree(d_A);
    cudaFree(d_result);
    free(A);

    return 0;
}

