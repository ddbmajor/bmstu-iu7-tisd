#include <stdio.h>
#include "graph.h"

enum choice
{
    exit_ = 0,
    add_vertex = 1,
    add_path = 2,
    delete_path_ = 3,
    print_graph = 4,
    find_path = 5
};

int main(int argc, char **argv)
{
    if (argc != 2)
        return -1;
    FILE *f = fopen(argv[1], "r");

    graph_t *gr = NULL;
    int rc = input_graph(&gr, f);
    if (rc != 0)
    {
        return rc;
    }
    fclose(f);
    node_t *res = NULL;
    int res_len;

    while (1)
    {
        printf("exit - 0, add_vertex - 1, add_path - 2, delete_path - 3, print_graph - 4, find_path - 5\n");
        enum choice ch;
        int tmpChoice = 0;
        while (scanf("%d", &tmpChoice) != 1 || tmpChoice < 0 || tmpChoice >= 6)
        {
            while (getchar() != '\n')
                ;
            if (tmpChoice < 0 || tmpChoice > 6)
            {
                printf("Нет такого пункта, попробуйте еще раз!\n");
            }
        }
        ch = tmpChoice;
        switch (ch)
        {
        case add_vertex:
            add_new_vertex(gr);
            break;
        case add_path:
            add_new_path(gr);
            break;
        case delete_path_:
            delete_path(gr);
            break;
        case print_graph:
            graphMatrixMakeDot(gr, "graph.txt", NULL);
            graphMatrixOpenDotFile("graph.txt");
            break;
        case find_path:
            res = longest_simple_path(gr, &res_len);
            graphMatrixMakeDot(gr, "res.txt", res);
            graphMatrixOpenDotFile("res.txt");
            printf("Summary len: %d\n", res_len);
            break;
        case exit_:
            delete_graph(gr);
            delete_list(res);
            return 0;
        default:
            break;
        }
    }
    return 0;
}