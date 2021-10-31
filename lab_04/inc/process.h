#ifndef _PROCESS_H_

#include <stdio.h>
// #include <stdlib.h>
// #include <sys/types.h>
#include "arrstack.h"
#include "liststack.h"
#include "list.h"

int add_to_stacks(Arr_Stack *arr_stack, List_Stack *list_stack);
int remove_from_stacks(Arr_Stack *arr_stack, List **arr_deleted, List_Stack *list_stack, List **list_deleted);

#define _PROCESS_H_
#endif