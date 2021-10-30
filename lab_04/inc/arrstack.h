#ifndef _ARRSTACK_H_
#include <stdio.h>
#include <stdlib.h>
#include "defines.h"


typedef struct Arr_Stack Arr_Stack;

struct Arr_Stack
{
    void **Stack_Start;
    int max_size;
    int curr_size;
    int curr;
    size_t el_size;
};


Arr_Stack *create_arr_stack();

int push_arr_stack(Arr_Stack *s, char *word);

char *pop_arr_stack(Arr_Stack *s);

void free_arr_stack(Arr_Stack *s);


#define _ARRSTACK_H_
#endif