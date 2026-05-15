/*
 * Versao instrumentada do main para medir o tempo gasto em cada etapa.
 * Reimplementa inline o conteudo de kruskal_solve para conseguir separar
 * "obtencao da MST" de "identificacao dos grupos" sem alterar o modulo.
 *
 * Saida:
 *  - Arquivo de saida normal (idem ao trab1).
 *  - Linha em stderr com os tempos em segundos, separados por TAB:
 *      arquivo n m k t_leitura t_dist t_sort t_mst t_id t_write t_total
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../src/point/point.h"
#include "../src/graph/graph.h"
#include "../src/union_find/union_find.h"
#include "../src/io/io.h"

static double now_sec(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9;
}

int main(int argc, char *argv[])
{
    if (argc != 4)
        exit(1);

    int k = atoi(argv[2]);
    int n, m;

    double t0 = now_sec();

    /* 1) Leitura dos dados */
    Point **points = io_read(argv[1], &n, &m);
    if (k < 1 || k > n)
        exit(1);

    double t1 = now_sec();

    /* 2) Calculo das distancias (graph_construct preenche os pesos das arestas) */
    Graph *g = graph_construct(points, n);

    double t2 = now_sec();

    /* 3) Ordenacao das distancias */
    graph_sort_edges(g);

    double t3 = now_sec();

    /* 4) Obtencao da MST (loop de Kruskal com parada antecipada em n-k arestas) */
    long long  num_edges = g_get_num_edges(g);
    UnionFind *uf        = uf_construct(n);
    int        count     = 0;
    for (long long i = 0; i < num_edges && count < n - k; i++) {
        Edge *e = g_get_edge(g, i);
        int   u = ed_get_p1(e);
        int   v = ed_get_p2(e);
        if (uf_find(uf, u) != uf_find(uf, v)) {
            uf_union(uf, u, v);
            count++;
        }
    }

    double t4 = now_sec();

    /* 5) Identificacao dos grupos (mapeia raizes UF para ids 0..k-1) */
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

    double t5 = now_sec();

    /* 6) Escrita do arquivo de saida */
    io_write(argv[3], points, n, cluster_of, k);

    double t6 = now_sec();

    free(cluster_of);
    graph_destroy(g);
    for (int i = 0; i < n; i++)
        point_destroy(points[i]);
    free(points);

    double t_read  = t1 - t0;
    double t_dist  = t2 - t1;
    double t_sort  = t3 - t2;
    double t_mst   = t4 - t3;
    double t_id    = t5 - t4;
    double t_write = t6 - t5;
    double t_total = t6 - t0;

    fprintf(stderr,
            "%s\t%d\t%d\t%d\t%.9f\t%.9f\t%.9f\t%.9f\t%.9f\t%.9f\t%.9f\n",
            argv[1], n, m, k,
            t_read, t_dist, t_sort, t_mst, t_id, t_write, t_total);

    return 0;
}
