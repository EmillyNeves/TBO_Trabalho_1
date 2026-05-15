#include "point.h"
#include <stdlib.h>
#include <string.h>

struct point
{
    double *coord;
    char *id;
    int dim;
};

Point *point_construct(char *id, double *coord, int dim)
{
    Point *p = (Point *)malloc(sizeof(Point));
    p->id = strdup(id);
    p->coord = (double *)malloc(dim * sizeof(double));

    for (int i = 0; i < dim; i++)
    {
        p->coord[i] = coord[i];
    }

    p->dim = dim;
    return p;
}

void point_destroy(Point *p)
{
    free(p->id);
    free(p->coord);
    free(p);
}

char *p_get_id(Point *p)
{
    return p->id;
}

double *p_get_coord(Point *p)
{
    return p->coord;
}

int p_get_dim(Point *p)
{
    return p->dim;
}
