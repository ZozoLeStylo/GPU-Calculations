#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define DATA_SIZE 65536

double reduction_parallel(float *A, int N) {
    double sum = 0.0;
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < N; i++) {
        sum += A[i];
    }
    return sum;
}

int main() {
    int N = DATA_SIZE;
    float *A = (float *)malloc(N * sizeof(float));
    for (int i = 0; i < N; i++) {
        A[i] = rand() / (float)RAND_MAX;
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Calcul de la réduction
    double result = reduction_parallel(A, N);

    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Taille : %d, Temps CPU : %f secondes, Résultat : %f\n", N, elapsed, result);

    free(A);
    return 0;
}

