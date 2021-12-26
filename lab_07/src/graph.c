#include "graph.h"


node_t *longest_simple_path(graph_t *graph, int *len)
{
    node_t *max_path = NULL;
    int max_len = longest_simple_path_from_vertex(graph, 0, &max_path);
    node_t *curr_path = NULL;
    int curr_len = 0;
    for (int i = 1; i < graph->vertex_num; i++)
    {
        curr_len = longest_simple_path_from_vertex(graph, i, &curr_path);
        if (curr_len > max_len)
        {
            max_len = curr_len;
            node_t *tmp = max_path;
            max_path = curr_path;
            delete_list(tmp);
            curr_path = NULL;
        }
        else
        {
            delete_list(curr_path);
            curr_path = NULL;
        }
    }

    *len = max_len;
    return max_path;
}


int longest_simple_path_from_vertex(graph_t *graph, int vertex, node_t **path)
{
    int max_len = 0;
    node_t *max_path = NULL;

    int curr_len = 0;
    node_t *curr_path = create_node(vertex);

    graph->visited[vertex] = 1;

    int flag = 0;
    for (int i = 0; i < graph->vertex_num; i++)
    {
        if (graph->matrix[vertex][i] > 0 && graph->visited[i] == 0)
        {
            flag = 1;
            curr_len = graph->matrix[vertex][i] + longest_simple_path_from_vertex(graph, i, &curr_path);
            if (curr_len > max_len)
            {
                max_len = curr_len;
                delete_list(max_path);
                max_path = curr_path;
                curr_path = create_node(vertex);
            }
            else
            {
                delete_list(curr_path);
                curr_path = create_node(vertex);
            }
        }
    }
    graph->visited[vertex] = 0;

    if (flag == 1)
    {
        *path = append_list(*path, max_path);
        return max_len;
    }
    if (flag == 0)
    {
        node_t *temp_node = create_node(vertex);
        *path = append_list(*path, temp_node);
        return 0;
    }

    return -1;
}


int add_new_path(graph_t *graph)
{
    printf("input vartexes and value\n");
    int src, dst, value;
    int rc = scanf("%d %d %d", &src, &dst, &value);
    if (rc != 3)
        return WEIGHT_INPUT_ERROR + VERTEX_NUM_INPUT_ERROR;
    if (dst < 0 || dst >= graph->vertex_num || src < 0 || src >= graph->vertex_num || value <= 0)
        return WEIGHT_INPUT_ERROR + VERTEX_NUM_INPUT_ERROR;
    graph->matrix[src][dst] = value;
    graph->matrix[dst][src] = value;
    return 0;
}


int delete_path(graph_t *graph)
{
    printf("input vartexes\n");
    int src, dst;
    int rc = scanf("%d %d", &src, &dst);
    if (rc != 2)
        return WEIGHT_INPUT_ERROR + VERTEX_NUM_INPUT_ERROR;
    if (dst < 0 || dst >= graph->vertex_num || src < 0 || src >= graph->vertex_num)
        return WEIGHT_INPUT_ERROR + VERTEX_NUM_INPUT_ERROR;
    graph->matrix[src][dst] = 0;
    graph->matrix[dst][src] = 0;
    return 0;
}


void add_new_vertex(graph_t *graph)
{
    graph->vertex_num++;
}