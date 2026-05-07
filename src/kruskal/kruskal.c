#include "kruskal.h"
#include "../union_find/union_find.h"
#include <stdlib.h>

int *kruskal_solve(Graph *g, int k)
{
    int n         = graph_get_num_points(g);
    int num_edges = graph_get_num_edges(g);

    UnionFind *uf = uf_construct(n);

    /* paramos quando juntar n - k arestas: sobram k componentes (os k grupos) */
    int count = 0;
    for (int i = 0; i < num_edges && count < n - k; i++) {
        Edge *e = graph_get_edge(g, i);
        int   u = edge_get_p1(e);
        int   v = edge_get_p2(e);
        if (uf_find(uf, u) != uf_find(uf, v)) {
            uf_union(uf, u, v);
            count++;
        }
    }

    /* a raiz do UF é um índice qualquer; mapeia cada raiz pro id 0..k-1 */
    int *root_id    = (int *)malloc(n * sizeof(int));
    int *cluster_of = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
        root_id[i] = -1;

    int next_id = 0;
    for (int i = 0; i < n; i++) {
        int r = uf_find(uf, i);
        if (root_id[r] == -1)
            root_id[r] = next_id++;
        cluster_of[i] = root_id[r];
    }

    free(root_id);
    uf_destroy(uf);

    return cluster_of;
}
