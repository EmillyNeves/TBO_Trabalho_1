#ifndef GRAPH_H
#define GRAPH_H

#include "../point/point.h"

typedef struct edge  Edge;
typedef struct graph Graph;

/**
 * @brief Aloca o grafo e o vetor de arestas sem calcular os pesos
 *
 * @param points Array de ponteiros para os pontos do grafo
 * @param n Número de pontos
 * @return Ponteiro para o grafo alocado
 */
Graph *graph_construct(Point **points, int n);

/**
 * @brief Libera o vetor de arestas e a struct do grafo
 *
 * @param g Grafo a ser destruído
 */
void graph_destroy(Graph *g);

/**
 * @brief Retorna o array de pontos do grafo
 *
 * @param g Grafo
 * @return Array de ponteiros para os pontos
 */
Point **graph_get_points(Graph *g);

/**
 * @brief Retorna o número de pontos do grafo
 *
 * @param g Grafo
 * @return Número de pontos
 */
int graph_get_num_points(Graph *g);

/**
 * @brief Retorna o vetor de arestas do grafo
 *
 * @param g Grafo
 * @return Array de arestas
 */
Edge *graph_get_edges(Graph *g);

/**
 * @brief Retorna o número de arestas do grafo
 *
 * @param g Grafo
 * @return Número de arestas
 */
int graph_get_num_edges(Graph *g);

/**
 * @brief Retorna o peso de uma aresta
 *
 * @param e Aresta
 * @return Peso da aresta
 */
double edge_get_weight(Edge *e);

/**
 * @brief Retorna o índice do primeiro ponto da aresta
 *
 * @param e Aresta
 * @return Índice p1
 */
int edge_get_p1(Edge *e);

/**
 * @brief Retorna o índice do segundo ponto da aresta
 *
 * @param e Aresta
 * @return Índice p2
 */
int edge_get_p2(Edge *e);

#endif
