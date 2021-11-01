#include "list.h"

List *create_list_node(char *word, void **ptr)
{
    List *node = malloc(sizeof(List));
    if (node == NULL)
        return NULL;
    node->next = NULL;
    node->ptr = ptr;
    node->word = word;
    return node;
}

List *add_to_list(List *head, List *node)
{
    node->next = head;
    return node;
}


void list_apply(List *head, void (*f)(List*, void*), void *arg)
{
    for ( ; head; head = head->next)
        f(head, arg);
}


void node_print(List *pers, void *arg)
{
    char *fmt = arg;
    printf(fmt, pers->ptr, pers->word);
}


void free_list(List *head)
{
    List *curr = head;

    while (curr)
    {
        curr = curr->next;
        free(head);
        head = curr;
    }
}
