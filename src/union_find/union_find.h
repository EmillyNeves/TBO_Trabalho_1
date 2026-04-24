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

#endif
