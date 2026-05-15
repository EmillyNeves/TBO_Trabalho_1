#ifndef KRUSKAL_H
#define KRUSKAL_H

#include "../graph/graph.h"

/**
 * @brief Executa o Kruskal com parada antecipada em (n - k) arestas aceitas,
 *        deixando exatamente k componentes conexas/clusters
 *
 * Pré-condição: as arestas do grafo já devem estar ordenadas por peso crescente
 *
 * @param g Grafo já com arestas ordenadas
 * @param k Número de grupos desejados
 * @return Vetor de tamanho n alocado dinamicamente, onde cluster[i] é o
 *         id do grupo (0..k-1) do ponto i. O chamador deve liberar com free
 */
int *kruskal_solve(Graph *g, int k);

#endif
