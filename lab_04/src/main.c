#include <stdio.h>
#include "arrstack.h"
#include "liststack.h"
#include "process.h"
#include "defines.h"


int main(void)
{
    int rc;
    int choice;
    char *menu = "0 - выход из программы\n1 - добавить слово\n2 - удалить слово\n3 - вывести в обратном порядке\n"
    "4 - вывести текущее состояние стеков\n";


    Arr_Stack *arr_stack = create_arr_stack();
    if (arr_stack == NULL)
        return ALLOCATE_ERROR;
    List_Stack *list_stack = create_list_stack();
    if (list_stack == NULL)
    {
        free_arr_stack(arr_stack);
        return ALLOCATE_ERROR;
    }
    
    rc = 0;
    while (rc == 0)
    {
        char tmp[5];
        printf("%s", menu);
        printf("Выберите пункт меню:\n");
        rc = scanf("%d", &choice);
        fgets(tmp, 5, stdin);
        if (rc != 1)
            return CHOICE_ERROR;
        switch (choice)
        {
        case 0:
            return 0;
        case 1:
            rc = add_to_stacks(arr_stack, list_stack);
            if (rc != 0)
                return rc;
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        default:
            printf("Неверный пункт меню\n");
            break;
        }
    }
}