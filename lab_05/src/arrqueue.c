#include "arrqueue.h"


arr_queue_t *create_arr_queue()
{
    arr_queue_t *queue = malloc(sizeof(arr_queue_t));
    if (queue == NULL)
        return NULL;
    queue->data = malloc(sizeof(task_t*));
    if (queue->data == NULL)
    {
        free(queue);
        return NULL;
    }
    queue->allocated = 1;
    queue->amount = 0;
    queue->Pin = 0;
    queue->Pout = 0;
    queue->maxamount = MAXLEN;
    return queue;
}


void delete_arr_queue(arr_queue_t *queue)
{
    for (int i = 0; i < queue->amount; i++)
    {
        delete_task(queue->data[i]);
    }
    free(queue->data);
    free(queue);
}


int push_arr_queue(arr_queue_t *queue, task_t *task)
{
    if (queue->amount == queue->maxamount)
        return OVERFLOW_ERROR;

    if (queue->amount >= queue->allocated)
    {
        task_t **tmp = realloc(queue->data, queue->allocated * 2 * sizeof(task_t*));
        if (tmp == NULL)
            return ALLOC_ERROR;
        queue->data = tmp;
        queue->allocated *= 2;
    }
    queue->data[queue->Pin] = task;
    queue->Pin++;
    queue->amount++;
    return 0;
}

task_t *pop_arr_queue(arr_queue_t *queue)
{
    if (queue->amount == 0)
        return NULL;
    task_t *tmp = queue->data[queue->Pout];
    for (int i = queue->Pout; i < queue->Pin - 1; i++)
    {
        queue->data[i] = queue->data[i + 1];
    }
    queue->Pin--;
    queue->amount--;
    return tmp;
}


void print_arr_queue(arr_queue_t *queue)
{
    for (int i = 0; i < queue->amount; i++)
    {
        printf("Номер элемента: %d\n", i);
        printf("Адрес элемента: %p\n", (void *)(queue->data + i));
    }
    printf("\n");
}