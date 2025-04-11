#include <cuda_runtime.h>
#include <stdio.h>
#include <time.h>

#define GRID_SIZE 1024
#define ITERATIONS 100
#define ALPHA 0.1
#define BLOCK_SIZE 16

__global__ void heat_diffusion_kernel(float *current, float *next, int N, int iterations) {
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    if (x > 0 && x < N - 1 && y > 0 && y < N - 1) {
        for (int t = 0; t < iterations; t++) {
            next[y * N + x] = current[y * N + x] +
                ALPHA * (current[(y+1) * N + x] +
                         current[(y-1) * N + x] +
                         current[y * N + (x+1)] +
                         current[y * N + (x-1)] -
                         4 * current[y * N + x]);

            __syncthreads();

            float *temp = current;
            current = next;
            next = temp;
        }
    }
}

int main() {
    int N = GRID_SIZE;
    int iterations = ITERATIONS;
    size_t size = N * N * sizeof(float);
    float *current, *next, *d_current, *d_next;

    current = (float *)malloc(size);
    next = (float *)malloc(size);

    for (int i = 0; i < N * N; i++) {
        current[i] = rand() / (float)RAND_MAX;
    }

    cudaMalloc((void **)&d_current, size);
    cudaMalloc((void **)&d_next, size);

    cudaMemcpy(d_current, current, size, cudaMemcpyHostToDevice);

    dim3 threadsPerBlock(BLOCK_SIZE, BLOCK_SIZE);
    dim3 numBlocks((N + BLOCK_SIZE - 1) / BLOCK_SIZE, (N + BLOCK_SIZE - 1) / BLOCK_SIZE);

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    heat_diffusion_kernel<<<numBlocks, threadsPerBlock>>>(d_current, d_next, N, iterations);

    clock_gettime(CLOCK_MONOTONIC, &end);

    cudaMemcpy(current, d_current, size, cudaMemcpyDeviceToHost);

    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Taille : %d, Iterations : %d, Temps GPU : %f secondes\n", N, iterations, elapsed);

    cudaFree(d_current);
    cudaFree(d_next);
    free(current);
    free(next);

    return 0;
}
