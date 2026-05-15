#include <stdio.h>
#include <stdlib.h>

#include "union_find/union_find.h"
#include "kruskal/kruskal.h"
#include "point/point.h"
#include "graph/graph.h"
#include "io/io.h"

#define NUM_ARGS 4

int main(int argc, char *argv[])
{
    if (argc != NUM_ARGS)
    {
        fprintf(stderr, "Erro no formato padrão: %s <input_file> <k> <output_file>\n", argv[0]);
        exit(1);
    }

    int n, m;
    int k = atoi(argv[2]); // String vira inteiro k

    if (k < 1)
    {
        fprintf(stderr, "Error k deve ser >= 1\n");
        exit(1);
    }

    // Lê pontos
    Point **points = io_read(argv[1], &n, &m);
    if (k > n)
    {
        fprintf(stderr, "Error k deve ser <= n (n = %d)\n", n);
        exit(1);
    }

    Graph *g = graph_construct(points, n);
    graph_sort_edges(g);

    int *cluster = kruskal_solve(g, k);
    io_write(argv[3], points, n, cluster, k);

    free(cluster);
    graph_destroy(g);

    for (int i = 0; i < n; i++)
    {
        point_destroy(points[i]);
    }
    free(points);

    return 0;
}
