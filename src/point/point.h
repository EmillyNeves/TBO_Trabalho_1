#ifndef POINT_H
#define POINT_H

typedef struct point Point;

/**
 * @brief Aloca e retorna um novo ponto
 *
 * @param id Identificador do ponto
 * @param coord Vetor de coordenadas a ser copiado
 * @param dim Número de dimensões do ponto
 * @return Ponteiro para o ponto alocado
 */
Point *point_construct(char *id, double *coord, int dim);

/**
 * @brief Libera toda a memória associada ao ponto
 *
 * @param p Ponto a ser destruído
 */
void point_destroy(Point *p);

/**
 * @brief Retorna o identificador do ponto
 *
 * @param p Ponto
 * @return String com o id
 */
char *p_get_id(Point *p);

/**
 * @brief Retorna o vetor de coordenadas do ponto
 *
 * @param p Ponto
 * @return Array de doubles com as coordenadas
 */
double *p_get_coord(Point *p);

/**
 * @brief Retorna o número de dimensões do ponto
 *
 * @param p Ponto
 * @return Número de dimensões
 */
int p_get_dim(Point *p);

#endif
