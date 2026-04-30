#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int count_commas(const char *line) {
    int count = 0;
    for (int i = 0; line[i] != '\0'; i++)
        if (line[i] == ',') count++;
    return count;
}

Point **io_read(const char *filename, int *n, int *m) {
    FILE *f = fopen(filename, "r");
    if (f == NULL)
        exit(1);

    char  *line = NULL;
    size_t cap  = 0;

    getline(&line, &cap, f);
    *m = count_commas(line);

    rewind(f);

    int count = 0;
    while (getline(&line, &cap, f) != -1)
        if (line[0] != '\n') count++;
    *n = count;

    rewind(f);

    Point **points = (Point **)malloc(*n * sizeof(Point *));
    double *coord  = (double *)malloc(*m * sizeof(double));

    for (int idx = 0; idx < *n; idx++) {
        getline(&line, &cap, f);

        line[strcspn(line, "\r\n")] = '\0';

        char *token = strtok(line, ",");
        char *id    = strdup(token);

        for (int d = 0; d < *m; d++) {
            token    = strtok(NULL, ",");
            coord[d] = atof(token);
        }

        points[idx] = point_construct(id, coord, *m);
        free(id);
    }

    free(coord);
    free(line);
    fclose(f);

    return points;
}
