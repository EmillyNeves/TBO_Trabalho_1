#include "graph.h"
#include <stdlib.h>

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
