#include "process.h"

int add_to_stacks(Arr_Stack *arr_stack, List_Stack *list_stack)
{
    char *word = NULL;
    char *wordcopy = NULL;
    size_t len = 0;
    ssize_t read;
    while (1)
    {
        printf("Введите слово: (без проблеов, максимум 30 символов)\n");
        read = getline(&word, &len, stdin);
        if (read)
        {
            if (word[0] == '\n' || word[0] == '\r' || word[0] == '\0')
            {
                printf("Пустое слово!\n");
                continue;
            }
            if (read > MAX_WORD_LEN + 1)
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
    wordcopy = strdup(word);
    int rc;
    rc = push_arr_stack(arr_stack, word);
    if (rc != 0)
    {
        free_arr_stack(arr_stack);
        return rc;
    }
    rc = push_list_stack(list_stack, wordcopy);
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
    printf("Из стека массивом удален элемент с адресом: %p и словом: %s\n", ptr, word);
    if (word == NULL)
        return EMPTY_STACK_ERROR;
    List *arr_node = create_list_node(word, ptr);
    *arr_deleted = add_to_list(*arr_deleted, arr_node);

    Node *node = pop_list_stack(list_stack);
    printf("Из стека списоком удален элемент с адресом: %p и словом: %s\n", (void *)node, node->word);
    if (node == NULL)
        return EMPTY_STACK_ERROR;
    List *list_node = create_list_node(node->word, (void *)node);
    *list_deleted = add_to_list(*list_deleted, list_node);

    return 0;
}


int print_backwards_word(char *word)
{
    if (word[0] == '\0')
        return -1;
    for (int i = strlen(word) - 1; i >= 0; i--)
    {
        printf("%c", word[i]);
    }
    printf(" ");
    return 0;
}


int print_backwards(Arr_Stack *arr_stack, List **arr_deleted, List_Stack *list_stack, List **list_deleted)
{
	int64_t sum_simple;
	struct timeval tv_start, tv_stop;

    int rc;
    if (arr_stack->curr == -1 || list_stack->Stack_Pointer == NULL)
        return EMPTY_STACK_ERROR;
    
    printf("Вывод из стека массивом:\n");

    gettimeofday(&tv_start, NULL);
    while (arr_stack->curr > -1)
    {
        void *ptr = (void *)(arr_stack->Stack_Start + arr_stack->curr);
        char *word = pop_arr_stack(arr_stack);

        rc = print_backwards_word(word);
        if (rc != 0)
            return rc;
        List *arr_node = create_list_node(word, ptr);
        *arr_deleted = add_to_list(*arr_deleted, arr_node);
    }
    gettimeofday(&tv_stop, NULL);
    sum_simple = (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
    printf("\n");
    printf("Время (мс): %" PRId64 "\n", sum_simple);
    printf("Требуемая память: %ld\n\n", sizeof(char *) * arr_stack->curr_size + 4 * sizeof(int));

    printf("Вывод из стека списоком:\n");
    gettimeofday(&tv_start, NULL);
    while (list_stack->Stack_Pointer)
    {
        Node *node = pop_list_stack(list_stack);
        rc = print_backwards_word(node->word);
        if (rc != 0)
            return rc;
        List *list_node = create_list_node(node->word, (void *)node);
        *list_deleted = add_to_list(*list_deleted, list_node);
    }
    gettimeofday(&tv_stop, NULL);
    sum_simple = (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
    printf("\n");
    printf("Время (мс): %" PRId64 "\n", sum_simple);
    printf("Требуемая память: %ld\n\n", sizeof(Node) * list_stack->curr_size + 2 * sizeof(int));
    return 0;
}


int print_current(Arr_Stack *arr_stack, List *arr_deleted, List_Stack *list_stack, List *list_deleted)
{
    printf("Cтек массивом:\n");
    if (arr_stack->curr == -1)
        printf("Стек пуст\n");
    else
    {
        for (int i = 0; i <= arr_stack->curr; i++)
        {
            printf("Указатель: %p\n", (void *)(arr_stack->Stack_Start + i));
            printf("Слово: %s\n", (char *)arr_stack->Stack_Start[i]);
        }
    }
    if (arr_deleted != NULL)
    {
        printf("Удаленные(свободные элементы):\n");
        list_apply(arr_deleted, node_print, "Указатель: %p\nСлово: %s\n");
    }
    printf("\n");


    printf("Стек списком\n");
    if (list_stack->Stack_Pointer == NULL)
        printf("Cтек пуст\n");
    else
    {
        Node *curr = list_stack->Stack_Pointer;
        while (curr != NULL)
        {
            printf("Указатель: %p\n", (void *)curr);
            printf("Слово: %s\n", curr->word);
            curr = curr->next;
        }
    }
    if (list_deleted != NULL)
    {
        printf("Удаленные(свободные элементы):\n");
        list_apply(list_deleted, node_print, "Указатель: %p\nСлово: %s\n");
    }

    return 0;
}
