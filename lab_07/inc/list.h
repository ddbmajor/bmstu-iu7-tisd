#ifndef _LIST_H_

#include <stdlib.h>


typedef struct node node_t;

struct node
{
    int data;
    node_t *next;
};


node_t *create_node(int data);

void delete_node(node_t *node);

node_t *append_list(node_t *head, node_t *node);

void delete_list(node_t *head);

#define _LIST_H_
#endif