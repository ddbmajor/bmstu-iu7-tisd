#ifndef _GRAPH_H_

#include "graph_cdio.h"
#include "list.h"


node_t *longest_simple_path(graph_t *graph, int *len);

int longest_simple_path_from_vertex(graph_t *graph, int vertex, node_t **path);

int add_new_path(graph_t *graph);

int delete_path(graph_t *graph);

void add_new_vertex(graph_t *graph);

#define _GRAPH_H_
#endif