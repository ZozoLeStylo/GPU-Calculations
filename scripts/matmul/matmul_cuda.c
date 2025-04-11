#include <cuda_runtime.h>
#include <stdio.h>
#include <time.h>

#define MATRIX_SIZE 1024
#define BLOCK_SIZE 16

__global__ void matmul(const float *A, const float *B, float *C, int N) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < N && col < N) {
        float value = 0.0;
        for (int k = 0; k < N; k++) {
            value += A[row * N + k] * B[k * N + col];
        }
        C[row * N + col] = value;
    }
}

int main() {
    int N = MATRIX_SIZE;
    size_t size = N * N * sizeof(float);
    float *A, *B, *C, *d_A, *d_B, *d_C;
    struct timespec start, end;
    cudaEvent_t startKernel, stopKernel;
    float kernelTime;

    // Allocation sur l'hôte
    A = (float *)malloc(size);
    B = (float *)malloc(size);
    C = (float *)malloc(size);

    for (int i = 0; i < N * N; i++) {
        A[i] = rand() / (float)RAND_MAX;
        B[i] = rand() / (float)RAND_MAX;
    }

    // Allocation sur le GPU
    cudaMalloc((void **)&d_A, size);
    cudaMalloc((void **)&d_B, size);
    cudaMalloc((void **)&d_C, size);

    clock_gettime(CLOCK_MONOTONIC, &start);

    // Transfert CPU -> GPU
    cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice);

    // Mesure du kernel
    cudaEventCreate(&startKernel);
    cudaEventCreate(&stopKernel);
    cudaEventRecord(startKernel, 0);

    // Lancement du kernel
    dim3 threadsPerBlock(BLOCK_SIZE, BLOCK_SIZE);
    dim3 numBlocks((N + BLOCK_SIZE - 1) / BLOCK_SIZE, (N + BLOCK_SIZE - 1) / BLOCK_SIZE);
    matmul<<<numBlocks, threadsPerBlock>>>(d_A, d_B, d_C, N);

    cudaEventRecord(stopKernel, 0);
    cudaEventSynchronize(stopKernel);
    cudaEventElapsedTime(&kernelTime, startKernel, stopKernel);

    // Transfert GPU -> CPU
    cudaMemcpy(C, d_C, size, cudaMemcpyDeviceToHost);

    clock_gettime(CLOCK_MONOTONIC, &end);

    // Temps total
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    double transferTime = elapsed - (kernelTime / 1000.0);

    printf("Taille : %d, Temps total : %f secondes, Temps GPU : %f secondes, Temps transfert : %f secondes\n",
           N, elapsed, kernelTime / 1000.0, transferTime);

    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);
    free(A);
    free(B);
    free(C);

    return 0;
}

