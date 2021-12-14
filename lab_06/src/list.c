#include "list.h"

List *create_list_node(char *word)
{
    List *node = malloc(sizeof(List));
    if (node == NULL)
        return NULL;
    node->next = NULL;
    node->word = word;
    return node;
}

List *add_to_list(List *head, List *node)
{
    node->next = head;
    return node;
}


List *delete_from_list(List *head, List *node)
{
    List *tmp = head;
    if (head == node)
    {
        head = head->next;
        free(node->word);
        free(node);
        return head;
    }
    while (tmp->next != node);
    if (tmp->next == NULL)
        return head;
    
    List *del = tmp->next;
    tmp->next = head->next->next;
    free(del->word);
    free(del);
    return head; 
}


void list_apply(List *head, void (*f)(List*, void*), void *arg)
{
    for ( ; head; head = head->next)
        f(head, arg);
}


void free_list(List *head)
{
    List *curr = head;

    while (curr)
    {
        curr = curr->next;
        free(head->word);
        free(head);
        head = curr;
    }
}


void list_print(List *list)
{
    for ( ; list; list = list->next)
    {
        printf("%s ", list->word);
    }
}


List *find_in_list(List *list, char *word, int *n)
{
    for ( ; list; list = list->next)
    {
        if (n != NULL)
            (*n)++;
        if (strcmp(list->word, word) == 0)
            return list;
    }
    return NULL;
}


