#ifndef _ARRQUEUE_H_

#include <stdlib.h>
#include <stdio.h>
#include "task.h"
#include "defines.h"


typedef struct
{
    task_t **data;
    int Pin;
    int Pout;
    int amount;
    int maxamount;
    int allocated;

} arr_queue_t;


arr_queue_t *create_arr_queue();
void delete_arr_queue(arr_queue_t *queue);
int push_arr_queue(arr_queue_t *queue, task_t *task);
task_t *pop_arr_queue(arr_queue_t *queue);
void print_arr_queue(arr_queue_t *queue);

#define _ARRQUEUE_H_
#endif