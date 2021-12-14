#ifndef _LIST_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct List List;

struct List
{
    char *word;
    List *next;
};

List *create_list_node(char *word);

List *add_to_list(List *head, List *node);

List *delete_from_list(List *head, List *node);

void list_apply(List *head, void (*f)(List*, void*), void *arg);

void free_list(List *head);

void list_print(List *list);

List *find_in_list(List *list, char *word, int *n);

#define _LIST_H_
#endif