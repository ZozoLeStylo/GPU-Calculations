#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

int total_solutions = 0;

bool is_safe(int board[], int row, int col, int n) {
    for (int i = 0; i < row; i++) {
        if (board[i] == col || 
            board[i] - i == col - row || 
            board[i] + i == col + row) {
            return false;
        }
    }
    return true;
}

void solve(int board[], int row, int n) {
    if (row == n) {
        total_solutions++;
        return;
    }

    for (int col = 0; col < n; col++) {
        if (is_safe(board, row, col, n)) {
            board[row] = col;
            solve(board, row + 1, n);
        }
    }
}

int main() {
    int n = 15;
    int *board = (int *)malloc(n * sizeof(int));
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);

    solve(board, 0, n);

    clock_gettime(CLOCK_MONOTONIC, &end);

    double total_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Nombre de solutions pour %d reines : %d, Temps total : %f secondes\n", n, total_solutions, total_time);

    free(board);
    return 0;
}

