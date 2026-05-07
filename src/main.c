#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "point/point.h"
#include "graph/graph.h"
#include "union_find/union_find.h"
#include "kruskal/kruskal.h"
#include "io/io.h"

int main(int argc, char *argv[])
{
    if (argc != 4)
        exit(1);

    // String de comando é convertida para o inteiro k
    int k = atoi(argv[2]);
    int n, m;

    // Leitura dos pontos
    Point **points = io_read(argv[1], &n, &m);
    if (k < 1 || k > n)
        exit(1);

    Graph *g = graph_construct(points, n);
    graph_sort_edges(g);

    int *cluster_of = kruskal_solve(g, k);
    io_write(argv[3], points, n, cluster_of, k);

    free(cluster_of);
    graph_destroy(g);

    for (int i = 0; i < n; i++)
        point_destroy(points[i]);
    free(points);

    return 0;
}
