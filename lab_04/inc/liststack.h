#ifndef _LISTSTACK_H_
#include <stdio.h>
#include <stdlib.h>
#include "defines.h"

typedef struct Node Node;

struct Node
{
    char *word;
    Node *next;
};

typedef struct List_Stack List_Stack;

struct List_Stack
{
    int curr_size;
    int max_size;
    Node *Stack_Pointer;
};


List_Stack *create_list_stack();

Node *create_node(char *word);

int push_list_stack(List_Stack *stack, char *word);

Node *pop_list_stack(List_Stack *stack);

void free_list_stack(List_Stack *stack);

void free_node(Node *node);


#define _LISTSTACK_H_
#endif