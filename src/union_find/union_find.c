#include "union_find.h"
#include <stdlib.h>

struct union_find {
    int *parent;
    int *rank;
};

UnionFind *uf_construct(int n)
{
    UnionFind *uf = (UnionFind *)malloc(sizeof(UnionFind));
    uf->parent    = (int *)malloc(n * sizeof(int));
    uf->rank      = (int *)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        uf->parent[i] = i;
        uf->rank[i]   = 0;
    }

    return uf;
}

void uf_destroy(UnionFind *uf)
{
    free(uf->parent);
    free(uf->rank);
    free(uf);
}

int uf_find(UnionFind *uf, int x)
{
    if (uf->parent[x] != x)
        uf->parent[x] = uf_find(uf, uf->parent[x]); /* compressão de caminho */
    return uf->parent[x];
}

void uf_union(UnionFind *uf, int x, int y)
{
    int rx = uf_find(uf, x);
    int ry = uf_find(uf, y);

    if (rx == ry) return;

    /* union by rank: árvore menor pendurada na maior */
    if (uf->rank[rx] < uf->rank[ry])
        uf->parent[rx] = ry;
    else if (uf->rank[rx] > uf->rank[ry])
        uf->parent[ry] = rx;
    else {
        uf->parent[ry] = rx;
        uf->rank[rx]++;
    }
}
