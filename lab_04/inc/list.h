#ifndef _LIST_H_
#include <stdio.h>
#include <stdlib.h>

typedef struct List List;

struct List
{
    char *word;
    void *ptr;
    List *next;
};

List *create_list_node(char *word, void **ptr);

List *add_to_list(List *head, List *node);

void list_apply(List *head, void (*f)(List*, void*), void *arg);

void node_print(List *pers, void *arg);

void free_list(List *head);

#define _LIST_H_
#endif