#include "graph.h"
#include <stdlib.h>
#include <math.h>

struct edge {
    double edge_weight;
    int    p1;
    int    p2;
};

struct graph {
    Point **points;
    int     num_points;
    Edge   *edges_matrix;
    int     num_edges; // n*(n-1)/2
};

static double euclidean_distance(Point *a, Point *b)
{
    int     dim   = point_get_dim(a);
    double *ca    = point_get_coord(a);
    double *cb    = point_get_coord(b);
    double  sum   = 0.0;
    for (int d = 0; d < dim; d++) {
        double diff = ca[d] - cb[d];
        sum += diff * diff;
    }
    return sqrt(sum);
}

Graph *graph_construct(Point **points, int n)
{
    Graph *g       = (Graph *)malloc(sizeof(Graph));
    g->points      = points;
    g->num_points  = n;
    g->num_edges   = n * (n - 1) / 2;
    g->edges_matrix = (Edge *)malloc(g->num_edges * sizeof(Edge));

    int idx = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            g->edges_matrix[idx].p1          = i;
            g->edges_matrix[idx].p2          = j;
            g->edges_matrix[idx].edge_weight = euclidean_distance(points[i], points[j]);
            idx++;
        }
    }

    return g;
}

void graph_destroy(Graph *g)
{
    free(g->edges_matrix);
    free(g);
}

static int compare_edges(const void *a, const void *b)
{
    Edge *e1 = (Edge *)a;
    Edge *e2 = (Edge *)b;
    if (e1->edge_weight < e2->edge_weight) return -1;
    if (e1->edge_weight > e2->edge_weight) return 1;
    return 0;
}

void graph_sort_edges(Graph *g)
{
    qsort(g->edges_matrix, g->num_edges, sizeof(Edge), compare_edges);
}

Point **graph_get_points(Graph *g)   { return g->points; }
int     graph_get_num_points(Graph *g) { return g->num_points; }
Edge   *graph_get_edges(Graph *g)    { return g->edges_matrix; }
Edge   *graph_get_edge(Graph *g, int i) { return &g->edges_matrix[i]; }
int     graph_get_num_edges(Graph *g)  { return g->num_edges; }

double edge_get_weight(Edge *e) { return e->edge_weight; }
int    edge_get_p1(Edge *e)     { return e->p1; }
int    edge_get_p2(Edge *e)     { return e->p2; }
