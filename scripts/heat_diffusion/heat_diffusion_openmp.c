#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define GRID_SIZE 32768
#define ITERATIONS 100  // Nombre d'itérations
#define ALPHA 0.1       // Facteur de diffusion

void heat_diffusion(float *current, float *next, int N, int iterations) {
    for (int t = 0; t < iterations; t++) {
        #pragma omp parallel for collapse(2)
        for (int i = 1; i < N - 1; i++) {
            for (int j = 1; j < N - 1; j++) {
                next[i * N + j] = current[i * N + j] +
                    ALPHA * (current[(i+1) * N + j] +
                             current[(i-1) * N + j] +
                             current[i * N + (j+1)] +
                             current[i * N + (j-1)] -
                             4 * current[i * N + j]);
            }
        }
        // Échange des pointeurs entre current et next
        float *temp = current;
        current = next;
        next = temp;
    }
}

int main() {
    int N = GRID_SIZE;
    int iterations = ITERATIONS;
    size_t size = N * N * sizeof(float);
    float *current = (float *)malloc(size);
    float *next = (float *)malloc(size);

    // Initialisation de la grille
    for (int i = 0; i < N * N; i++) {
        current[i] = rand() / (float)RAND_MAX;
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Simulation
    heat_diffusion(current, next, N, iterations);

    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Taille : %d, Iterations : %d, Temps CPU : %f secondes\n", N, iterations, elapsed);

    free(current);
    free(next);
    return 0;
}

