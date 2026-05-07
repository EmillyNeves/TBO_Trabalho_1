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

typedef struct group {
    char **ids;
    int    size;
} Group;

static int compare_ids(const void *a, const void *b)
{
    return strcmp(*(char **)a, *(char **)b);
}

static int compare_groups(const void *a, const void *b)
{
    Group *g1 = (Group *)a;
    Group *g2 = (Group *)b;
    return strcmp(g1->ids[0], g2->ids[0]);
}

void io_write(const char *filename, Point **points, int n, int *cluster_of, int k)
{
    FILE *f = fopen(filename, "w");
    if (f == NULL)
        exit(1);

    Group *groups = (Group *)malloc(k * sizeof(Group));

    for (int c = 0; c < k; c++)
        groups[c].size = 0;
    for (int i = 0; i < n; i++)
        groups[cluster_of[i]].size++;
    for (int c = 0; c < k; c++)
        groups[c].ids = (char **)malloc(groups[c].size * sizeof(char *));

    /* preenche cada grupo com os ponteiros pros ids dos pontos */
    int *pos = (int *)calloc(k, sizeof(int));
    for (int i = 0; i < n; i++) {
        int c = cluster_of[i];
        groups[c].ids[pos[c]++] = point_get_id(points[i]);
    }
    free(pos);

    /* ordena os ids dentro de cada grupo */
    for (int c = 0; c < k; c++)
        qsort(groups[c].ids, groups[c].size, sizeof(char *), compare_ids);

    /* ordena os grupos pelo primeiro id (que já é o menor depois do sort acima) */
    qsort(groups, k, sizeof(Group), compare_groups);

    for (int c = 0; c < k; c++) {
        for (int i = 0; i < groups[c].size; i++) {
            if (i > 0) fputc(',', f);
            fputs(groups[c].ids[i], f);
        }
        fputc('\n', f);
    }

    for (int c = 0; c < k; c++)
        free(groups[c].ids);
    free(groups);

    fclose(f);
}
