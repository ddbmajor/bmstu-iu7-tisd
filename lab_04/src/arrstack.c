#include "arrstack.h"

Arr_Stack *create_arr_stack()
{
    Arr_Stack *arr_stack = malloc(sizeof(Arr_Stack));
    if (arr_stack == NULL)
        return NULL;
    arr_stack->curr_size = 1;
    arr_stack->el_size = sizeof(char **);
    arr_stack->Stack_Start = malloc(arr_stack->curr_size * arr_stack->el_size);
    if (arr_stack->Stack_Start == NULL)
        return NULL;
    arr_stack->max_size = 4;
    arr_stack->curr = -1;
    return arr_stack;
}

int push_arr_stack(Arr_Stack *s, char *word)
{
    if (s->curr + 1 < s->curr_size)
    {
        s->curr++;
        s->Stack_Start[s->curr] = word;
        return 0;
    }
    if (s->curr + 1 == s->curr_size)
    {
        if(s->curr_size * STEP <= s->max_size)
        {
            void **tmp = realloc(s->Stack_Start, s->curr_size * s->el_size * STEP);
            if (tmp == NULL)
                return ALLOCATE_ERROR;
            s->Stack_Start = tmp;
            s->curr_size *= STEP;

            s->curr++;
            s->Stack_Start[s->curr] = word;
            return 0;
        }
        else
        {
            return STACK_OVERFLOW_ERROR;
        }
    }
    return 0;
}

char *pop_arr_stack(Arr_Stack *s)
{
    if(s->curr == -1)
        return NULL;
    char *tmp = s->Stack_Start[s->curr];
    s->curr--;
    return tmp;
}

void free_arr_stack(Arr_Stack *s)
{
    for (int i = 0; i <= s->curr; i++)
    {
        free(s->Stack_Start[s->curr]);
    }
    free(s->Stack_Start);
    free(s);
}