#ifndef IO_H
#define IO_H

#include "../point/point.h"

/**
 * @brief Lê o arquivo de entrada e retorna um array de pontos
 *
 * Descobre n e m dinamicamente pela primeira linha
 * O chamador é responsável por liberar os pontos e o array
 *
 * @param file_name Caminho para o arquivo de entrada
 * @param n Ponteiro onde será escrito o número de pontos lidos
 * @param m Ponteiro onde será escrito o número de dimensões
 * @return Array de ponteiros para os pontos lidos
 */
Point **io_read(const char *file_name, int *n, int *m);

/**
 * @brief Escreve os grupos no arquivo de saída em ordem lexicográfica
 *
 * Cada linha corresponde a um grupo, com os ids dos pontos separados por
 * vírgula. Os ids dentro de cada grupo são ordenados, e os grupos são ordenados pelo primeiro id de cada um
 *
 * @param file_name   Caminho do arquivo de saída
 * @param points     Array de pontos
 * @param n          Número de pontos
 * @param cluster_of Vetor [n] com o id do grupo (0..k-1) de cada ponto
 * @param k          Número de grupos
 */
void io_write(const char *file_name, Point **points, int n, int *cluster_of, int k);

#endif
