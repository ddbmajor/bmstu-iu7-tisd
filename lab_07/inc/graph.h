#ifndef _GRAPH_H_

#include "graph_cdio.h"
#include "list.h"


// Нахождение самого длинного простого пути в графе
node_t *longest_simple_path(graph_t *graph, int *len);

// Нахождение самого длинного просто пути исходящего из заданной вершины
int longest_simple_path_from_vertex(graph_t *graph, int vertex, node_t **path);

// Добавление ребра
int add_new_path(graph_t *graph);

// Удаление ребра
int delete_path(graph_t *graph);

// Добавление вершины
void add_new_vertex(graph_t *graph);

#define _GRAPH_H_
#endif