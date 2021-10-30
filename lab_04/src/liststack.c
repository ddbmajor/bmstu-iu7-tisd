#include "liststack.h"

List_Stack *create_list_stack()
{
    List_Stack *list_stack = malloc(sizeof(List_Stack));
    if (list_stack == NULL)
        return NULL;
    list_stack->Stack_Pointer = NULL;
    list_stack->max_size = 1024;
    list_stack->curr_size = 0;
    return list_stack;
}

Node *create_node(char *word)
{
    Node *node = malloc(sizeof(Node));
    if (node == NULL)
        return NULL;
    node->word = word;
    node->next = NULL;
    return node;
}

int push_list_stack(List_Stack *stack, char *word)
{
    Node *node = create_node(word);
    if (node == NULL)
        return ALLOCATE_ERROR;

    if (stack->max_size > stack->curr_size + 1)
    {
        node->next = stack->Stack_Pointer;
        stack->Stack_Pointer = node;
        stack->curr_size += 1;
    }
    else
        return STACK_OVERFLOW_ERROR;
    return 0;
}

Node *pop_list_stack(List_Stack *stack)
{
    Node *tmp = stack->Stack_Pointer;
    if (stack->Stack_Pointer == NULL)
        return NULL;
    stack->Stack_Pointer = stack->Stack_Pointer->next;
    stack->curr_size -= 1;
    return tmp;
}

void free_list_stack(List_Stack *stack)
{
    Node *curr = stack->Stack_Pointer;

    while (curr)
    {
        curr = curr->next;
        free(stack->Stack_Pointer);
        stack->Stack_Pointer = curr;
    }

    stack->curr_size = 0;

    free(stack);
}

void free_node(Node *node)
{
    free(node);
}