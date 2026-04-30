#ifndef IO_H
#define IO_H

#include "../point/point.h"

/**
 * @brief Lê o arquivo de entrada e retorna um array de pontos
 *
 * Descobre n e m dinamicamente pela primeira linha.
 * O chamador é responsável por liberar os pontos e o array.
 *
 * @param filename Caminho para o arquivo de entrada
 * @param n Ponteiro onde será escrito o número de pontos lidos
 * @param m Ponteiro onde será escrito o número de dimensões
 * @return Array de ponteiros para os pontos lidos
 */
Point **io_read(const char *filename, int *n, int *m);

#endif
