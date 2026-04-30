#ifndef UNION_FIND_H
#define UNION_FIND_H

typedef struct union_find UnionFind;

/**
 * @brief Aloca e retorna um Union-Find com n elementos
 *
 * @param n Número de elementos
 * @return Ponteiro para o Union-Find alocado
 */
UnionFind *uf_construct(int n);

/**
 * @brief Libera toda a memória associada ao Union-Find
 *
 * @param uf Union-Find a ser destruído
 */
void uf_destroy(UnionFind *uf);

/**
 * @brief Retorna o representante do conjunto de x (com compressão de caminho)
 *
 * @param uf Union-Find
 * @param x  Elemento
 * @return Representante do conjunto
 */
int uf_find(UnionFind *uf, int x);

/**
 * @brief Une os conjuntos de x e y (por rank)
 *
 * @param uf Union-Find
 * @param x  Elemento do primeiro conjunto
 * @param y  Elemento do segundo conjunto
 */
void uf_union(UnionFind *uf, int x, int y);

#endif
