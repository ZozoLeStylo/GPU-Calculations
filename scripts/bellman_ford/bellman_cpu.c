#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <omp.h>
#include <time.h>

#define INF INT_MAX
#define VERTICES 15000
#define EDGES 112492500

typedef struct {
    int src;
    int dest;
    int weight;
} Edge;

void bellman_ford_cpu(int vertices, int edges, Edge *edge_list, int src, int *dist) {
    for (int i = 0; i < vertices; i++) {
        dist[i] = INF;
    }
    dist[src] = 0;

    for (int i = 1; i <= vertices - 1; i++) {
        #pragma omp parallel for
        for (int j = 0; j < edges; j++) {
            int u = edge_list[j].src;
            int v = edge_list[j].dest;
            int weight = edge_list[j].weight;

            if (dist[u] != INF && dist[u] + weight < dist[v]) {
                #pragma omp critical
                dist[v] = dist[u] + weight;
            }
        }
    }
}

int main() {
    int vertices = VERTICES ; // Nombre de sommets
    int edges = EDGES ;    // Nombre d'arêtes
    int *dist = (int *)malloc(vertices * sizeof(int));
    Edge *edge_list = (Edge *)malloc(edges * sizeof(Edge));
    struct timespec start, end;

    // Génération aléatoire des arêtes
    srand(0);
    for (int i = 0; i < edges; i++) {
        edge_list[i].src = rand() % vertices;
        edge_list[i].dest = rand() % vertices;
        edge_list[i].weight = rand() % 20 + 1;
    }

    int src = 0; // Sommet source

    clock_gettime(CLOCK_MONOTONIC, &start);
    bellman_ford_cpu(vertices, edges, edge_list, src, dist);
    clock_gettime(CLOCK_MONOTONIC, &end);

    double total_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Temps CPU (OpenMP) : %f secondes, Distance du sommet %d au sommet %d : %d\n", total_time, src, vertices - 1, dist[vertices - 1]);

    free(dist);
    free(edge_list);
    return 0;
}

