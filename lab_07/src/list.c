#include "list.h"

node_t *create_node(int data)
{
    node_t *node = malloc(sizeof(node_t));
    if (!node)
        return NULL;
    node->data = data;
    node->next = NULL;
    return node;
}


void delete_node(node_t *node)
{
    free(node);
}


node_t *append_list(node_t *head, node_t *node)
{
    if (!head)
        return node;
    node_t *curr = head;
    while (curr->next)
    {
        curr = curr->next;
    }
    curr->next = node;
    return head;
}


void delete_list(node_t *head)
{
    if (head == NULL)
    {
        return;
    }
    if (head->next == NULL)
    {
        free(head);
        head = NULL;
        return;
    }
    node_t *tmp;
    while (head->next != NULL)
    {
        tmp = head;
        head = head->next;
        free(tmp);
        tmp = NULL;
    }
    free(head);
    head = NULL;
}