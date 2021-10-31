#include "process.h"

int add_to_stacks(Arr_Stack *arr_stack, List_Stack *list_stack)
{
    char *word = NULL;
    size_t len = 0;
    ssize_t read;
    while (1)
    {
        printf("Введите слово: (без проблеов, максимум 30 символов)\n");
        read = getline(&word, &len, stdin);//остановился тут, надо выдялять память, потом не забудь очищать!!! Удачи друг я чилить
        if (read)
        {
            if (word[0] == '\n' || word[0] == '\r' || word[0] == '\0')
            {
                printf("Пустое слово!\n");
                continue;
            }
            if (word[MAX_WORD_LEN + 1] != '\0')
            {
                printf("Слишком большое слово!\n");
                continue;
            }
            int flag = 0;
            for (int i = 0; word[i] != '\0'; i++)
            {
                if (word[i] == '\n')
                    word[i] = '\0';
                if (word[i] == ' ')
                    flag = 1;
            }
            if (flag == 1)
            {
                printf("В слове есть пробелы!\n");
                continue;
            }
            
            break;
        }
        else
        {
            continue;
        }
    }
    int rc;
    rc = push_arr_stack(arr_stack, word);
    if (rc != 0)
    {
        free_arr_stack(arr_stack);
        return rc;
    }
    rc = push_list_stack(list_stack, word);
    if (rc != 0)
    {
        free_arr_stack(arr_stack);
        free_list_stack(list_stack);
        return rc;
    }

    return 0;
}


int remove_from_stacks(Arr_Stack *arr_stack, List **arr_deleted, List_Stack *list_stack, List **list_deleted)
{
    void *ptr = (void *)(arr_stack->Stack_Start + arr_stack->curr);
    char *word = pop_arr_stack(arr_stack);
    if (word == NULL)
        return EMPTY_STACK_ERROR;
    List *arr_node = create_list_node(word, ptr);
    *arr_deleted = add_to_list(*arr_deleted, arr_node);

    Node *node = pop_list_stack(list_stack);
    if (node == NULL)
        return EMPTY_STACK_ERROR;
    List *list_node = create_list_node(node->word, (void *)node);
    *list_deleted = add_to_list(*list_deleted, list_node);

    return 0;
}