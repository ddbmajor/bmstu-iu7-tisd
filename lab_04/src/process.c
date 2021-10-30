#include "process.h"

int add_to_stacks(Arr_Stack *arr_stack, List_Stack *list_stack)
{
    char word[MAX_WORD_LEN + 2];
    while (1)
    {
        printf("Введите слово: (без проблеов, максимум 30 символов)\n");
        fgets(word, MAX_WORD_LEN + 2, stdin);//остановился тут, надо выдялять память, потом не забудь очищать!!! Удачи друг я чилить

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