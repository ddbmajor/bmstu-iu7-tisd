#ifndef _LISTQUEUE_H_

#include "task.h"
#include "defines.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct node node_t;

struct node
{
    task_t *task;
    node_t *next;
};

typedef struct
{
    node_t *Pin;
    node_t *Pout;

    int amount;
    int maxamount;

} list_queue_t;


node_t *create_node(task_t *task);
list_queue_t *create_list_queue();
void delete_list_queue(list_queue_t *queue);
int push_list_queue(list_queue_t *queue, task_t *task);
task_t *pop_list_queue(list_queue_t *queue);
void print_list_queue(list_queue_t *queue);

#define _LISTQUEUE_H_
#endif