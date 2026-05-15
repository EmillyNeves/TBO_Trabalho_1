#include <stdlib.h>
#include "union_find.h"

struct union_find
{
    int *parent;
    int *rank;
};

UnionFind *uf_construct(int n)
{
    UnionFind *uf = (UnionFind *)malloc(sizeof(UnionFind));
    uf->parent = (int *)malloc(n * sizeof(int));
    uf->rank = (int *)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++)
    {
        uf->rank[i] = 0;
        uf->parent[i] = i;
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
    // primeiro acha a raiz
    int raiz = x;
    while (raiz != uf->parent[raiz])
        raiz = uf->parent[raiz];

    // path compression: faz todo nó do caminho apontar pra raiz
    while (x != raiz)
    {
        int proximo = uf->parent[x];
        uf->parent[x] = raiz;
        x = proximo;
    }
    return raiz;
}

void uf_union(UnionFind *uf, int x, int y)
{
    int rX = uf_find(uf, x);
    int rY = uf_find(uf, y);

    if (rX == rY)
        return;

    // a arvore de menor rank vai sob a de maior rank, se empatepa escolhe um e incrementa o rank do vencedor
    if (uf->rank[rX] < uf->rank[rY])
        uf->parent[rX] = rY;
    else if (uf->rank[rX] > uf->rank[rY])
        uf->parent[rY] = rX;
    else
    {
        uf->parent[rY] = rX;
        uf->rank[rX] ++;
    }
}
