#include <stdlib.h>
#include <math.h>
#include "graph.h"

struct edge
{
    int p1;
    int p2;
    double edge_weight;
};

struct graph
{
    Point **points;
    Edge *edges_matrix;
    int num_points;
    long long num_edges; // n*(n-1)/2
};

static double euclidean_dist(Point *a, Point *b)
{
    int dim = p_get_dim(a);
    double *cA = p_get_coord(a);
    double *cB = p_get_coord(b);
    double sum = 0.0;
    for (int d = 0; d < dim; d++)
    {
        double diff = cA[d] - cB[d];
        sum += diff * diff;
    }
    return sqrt(sum);
}

Graph *graph_construct(Point **points, int n)
{
    Graph *g = (Graph *)malloc(sizeof(Graph));
    g->points = points;
    g->num_points = n;
    g->num_edges = (long long)n * (long long)(n - 1) / 2;
    g->edges_matrix = (Edge *)malloc(g->num_edges * sizeof(Edge));

    long long idx = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            g->edges_matrix[idx].p1 = i;
            g->edges_matrix[idx].p2 = j;
            g->edges_matrix[idx].edge_weight = euclidean_dist(points[i], points[j]);
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

int compare_edges(const void *a, const void *b)
{
    Edge *e1 = (Edge *)a;
    Edge *e2 = (Edge *)b;
    if (e1->edge_weight < e2->edge_weight)
        return -1;
    if (e1->edge_weight > e2->edge_weight)
        return 1;
    return 0;
}

void graph_sort_edges(Graph *g)
{
    qsort(g->edges_matrix, g->num_edges, sizeof(Edge), compare_edges);
}

Point **g_get_points(Graph *g)
{
    return g->points;
}
int g_get_num_points(Graph *g)
{
    return g->num_points;
}
Edge *g_get_edges(Graph *g)
{
    return g->edges_matrix;
}
Edge *g_get_edge(Graph *g, long long i)
{
    return &g->edges_matrix[i];
}
long long g_get_num_edges(Graph *g)
{
    return g->num_edges;
}

double ed_get_weight(Edge *e)
{
    return e->edge_weight;
}
int ed_get_p1(Edge *e)
{
    return e->p1;
}
int ed_get_p2(Edge *e)
{
    return e->p2;
}
