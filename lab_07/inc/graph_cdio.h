#ifndef _GRAPH_CDIO_H_

#include <stdlib.h>
#include <stdio.h>
#include "defines.h"
#include "list.h"


typedef struct graph graph_t;

struct graph
{
    int **matrix;
    int *visited;
    int vertex_num;
};


graph_t *create_graph(int vertex_num);

void delete_graph(graph_t *graph);

int input_graph(graph_t **graph, FILE *f);

void graphMatrixMakeDot(graph_t *src, char *fileName, node_t *res);

void graphMatrixOpenDotFile(char *fileName);

#define _GRAPH_CDIO_H_
#endif