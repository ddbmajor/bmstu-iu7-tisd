#ifndef _TASK_H_

#include <stdlib.h>

typedef struct
{
    int n;
} task_t;

task_t *create_task();
void delete_task(task_t *task);

#define _TASK_H_
#endif