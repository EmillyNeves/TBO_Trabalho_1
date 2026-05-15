#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_SIZE 10
#define INCREASE 2

Point **io_read(const char *file_name, int *n, int *m)
{
    FILE *f = fopen(file_name, "r");
    if (f == NULL)
    {
        fprintf(stderr, "Error em ler arquivo '%s'\n", file_name);
        exit(1);
    }

    char *line = NULL;
    size_t cap = 0;

    int capacity = INITIAL_SIZE;

    Point **points = (Point **)malloc(capacity * sizeof(Point *));
    double *coord = NULL;
    *n = 0;
    *m = -1;

    while (getline(&line, &cap, f) != -1)
    {
        line[strcspn(line, "\r\n")] = '\0';

        if (line[0] == '\0')
            continue;

        if (*m == -1)
        {
            int comma_count = 0;
            for (int i = 0; line[i] != '\0'; i++)
                if (line[i] == ',')
                    comma_count++;
            *m = comma_count;
            if (*m <= 0)
            {
                fprintf(stderr, "Erro formato inválido\n");
                free(points);
                free(line);
                fclose(f);
                exit(1);
            }
            coord = (double *)malloc(*m * sizeof(double));
        }

        char *comma = strtok(line, ",");
        if (comma == NULL)
            continue;
        char *id = strdup(comma);

        int d;
        for (d = 0; d < *m; d++)
        {
            comma = strtok(NULL, ",");
            if (comma == NULL)
                break;
            coord[d] = atof(comma);
        }
        if (d < *m)
        {
            free(id);
            continue;
        }

        if (*n == capacity)
        {
            capacity *= INCREASE;
            points = (Point **)realloc(points, capacity * sizeof(Point *));
        }
        points[*n] = point_construct(id, coord, *m);
        (*n)++;
        free(id);
    }

    free(coord);
    free(line);
    fclose(f);

    if (*n == 0)
    {
        fprintf(stderr, "Erro na leitura dos pontos\n");
        free(points);
        exit(1);
    }
    return points;
}

typedef struct group
{
    char **ids;
    int size;
} Group;

int compare_groups(const void *a, const void *b)
{
    Group *g1 = (Group *)a;
    Group *g2 = (Group *)b;
    return strcmp(g1->ids[0], g2->ids[0]);
}
int compare_ids(const void *a, const void *b)
{
    return strcmp(*(char **)a, *(char **)b);
}

void io_write(const char *file_name, Point **points, int n, int *cluster_of, int k)
{
    FILE *f = fopen(file_name, "w");
    if (f == NULL)
        exit(1);

    Group *groups = (Group *)calloc(k, sizeof(Group));

    for (int i = 0; i < n; i++)
    {
        groups[cluster_of[i]].size++;
    }
    for (int j = 0; j < k; j++)
    {

        groups[j].ids = (char **)malloc(groups[j].size * sizeof(char *));
    }

    // preenche cada grupo com os ponteiros pros ids dos pontos
    int *pos = (int *)calloc(k, sizeof(int));
    for (int i = 0; i < n; i++)
    {
        int c = cluster_of[i];
        groups[c].ids[pos[c]++] = p_get_id(points[i]);
    }
    free(pos);

    // ordena os ids dentro de cada grupo
    for (int j = 0; j < k; j++)
    {
        qsort(groups[j].ids, groups[j].size, sizeof(char *), compare_ids);
    }

    // ordena os grupos pelo id
    qsort(groups, k, sizeof(Group), compare_groups);

    for (int j = 0; j < k; j++)
    {
        for (int i = 0; i < groups[j].size; i++)
        {
            if (i > 0)
                fputc(',', f);
            fputs(groups[j].ids[i], f);
        }
        fputc('\n', f);
    }

    for (int j = 0; j < k; j++)
    {
        free(groups[j].ids);
    }
    free(groups);

    fclose(f);
}
