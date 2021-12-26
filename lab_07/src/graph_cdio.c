#include "graph_cdio.h"

graph_t *create_graph(int vertex_num)
{
    graph_t *graph = malloc(sizeof(graph_t));
    if (!graph)
        return NULL;
    graph->vertex_num = vertex_num;
    graph->visited = calloc(2 * vertex_num, sizeof(int));
    if (!graph->visited)
    {
        free(graph);
        return NULL;
    }
    graph->matrix = calloc(2 * vertex_num, sizeof(int *));
    if (!graph->matrix)
    {
        free(graph->visited);
        free(graph);
        return NULL;
    }
    for (int i = 0; i < 2 * vertex_num; i++)
    {
        graph->matrix[i] = calloc(2 * vertex_num, sizeof(int));
        if (!graph->matrix[i])
        {
            delete_graph(graph);
            return NULL;
        }
    }
    return graph;
}


void delete_graph(graph_t *graph)
{
    if (!graph)
        return;
    for (int i = 0; i < graph->vertex_num; i++)
    {
        free(graph->matrix[i]);
    }
    free(graph->matrix);
    free(graph->visited);
    free(graph);
}


int input_graph(graph_t **graph, FILE *f)
{
    int rc;
    int vertex_num;
    int weight;
    rc = fscanf(f, "%d", &vertex_num);
    if (rc != 1)
        return VERTEX_NUM_INPUT_ERROR;
    *graph = create_graph(vertex_num);
    if (!*graph)
        return ALLOC_ERROR;

    for (int i = 0; i < vertex_num; i++)
    {
        for (int j = 0; j < vertex_num; j++)
        {
            rc = fscanf(f, "%d", &weight);
            if (rc != 1)
            {
                delete_graph(*graph);
                return WEIGHT_INPUT_ERROR;
            }
            if (weight < 0)
            {
                delete_graph(*graph);
                return WEIGHT_INPUT_ERROR;
            }
            (*graph)->matrix[i][j] = weight;
        }
    }
    return 0;
}


static void graphMatrixStartingDot(FILE *outputFile)
{
    fprintf(outputFile, "graph \"GraphMatrix\" {\n");
}

static void graphMatrixEndingDot(FILE *outputFile)
{
    fprintf(outputFile, "}\n");
}

static int if_in_res(int a, int b, node_t *res)
{
    if (!res)
        return 0;
    if (!res->next)
        return 0;
    node_t *l = res;
    node_t *r = res->next;
    while (r)
    {
        if ((l->data == a && r->data == b) || (l->data == b && r->data == a))
            return 1;
        l = l->next;
        r = r->next;
    }
    return 0;
}

static void graphMatrixPrintNodeDot(int *currentNode, int currentInd, FILE *outputFile, int size, node_t *res)
{
    for (int i = 0; i < size; ++i)
    {
        if (currentNode[i] != 0 && i > currentInd)
        {
            if (res == NULL)
                fprintf(outputFile, "%d -- %d [label=%d];\n", currentInd, i,
                        currentNode[i]);
            else
            {
                if (if_in_res(currentInd, i, res))
                    fprintf(outputFile, "%d -- %d [label=%d] [color=red];\n", currentInd, i,
                        currentNode[i]);
                else
                    fprintf(outputFile, "%d -- %d [label=%d];\n", currentInd, i,
                        currentNode[i]);
            }
        }
    }
}

void graphMatrixMakeDot(graph_t *src, char *fileName, node_t *res)
{
    FILE *outputFile = fopen(fileName, "w");
    graphMatrixStartingDot(outputFile);
    for (int i = 0; i < src->vertex_num; ++i)
    {
        graphMatrixPrintNodeDot(src->matrix[i], i, outputFile, src->vertex_num, res);
    }
    graphMatrixEndingDot(outputFile);
    fclose(outputFile);
}


void graphMatrixOpenDotFile(char *fileName)
{
    char command[30] = "\0";

    sprintf(command, "xdot %s &", fileName);
    system(command);
}