#include <stdlib.h>
#include "kruskal.h"
#include "../union_find/union_find.h"

int *kruskal_solve(Graph *g, int k)
{
    int num = 0;
    int n = g_get_num_points(g);
    long long num_edges = g_get_num_edges(g);

    UnionFind *uf = uf_construct(n);

    // para quando junta n - k arestas e sobram os k grupos
    for (long long i = 0; i < num_edges && num < n - k; i++)
    {
        Edge *e = g_get_edge(g, i);
        int u = ed_get_p1(e);
        int v = ed_get_p2(e);
        if (uf_find(uf, u) != uf_find(uf, v))
        {
            uf_union(uf, u, v);
            num++;
        }
    }

    // a raiz do UF é um índice qualquer, mapeia cada raiz
    int *root_id = (int *)malloc(n * sizeof(int));
    int *cluster = (int *)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++)
        root_id[i] = -1;

    for (int i = 0, next_id = 0; i < n; i++)
    {
        int r = uf_find(uf, i);
        if (root_id[r] == -1)
            root_id[r] = next_id++;
        cluster[i] = root_id[r];
    }

    free(root_id);
    uf_destroy(uf);

    return cluster;
}
