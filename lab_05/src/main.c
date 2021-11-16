#include "process.h"

int main(void)
{
    int choice;
    int rc;
    char *menu = "0 - выход из меню\n1 - запустить симуляцию для очереди массивом\n2 - запустить симуляцию для очереди списком\n"
    "3 - оценка эффективности\n4 - перейти в ручной режим\n";

    while (1)
    {
        printf("Выберите пункт меню:\n");
        printf("%s", menu);
        rc = scanf("%d", &choice);
        if (rc != 1)
        {
            printf("Неверно введен пункт меню\n");
            while (getchar() != '\n')
                ;
            continue;
        }
        else
        {
            switch (choice)
            {
            case 0:
                return 0;
                break;
            case 1:
                simulate_arr();
                break;
            case 2:
                simulate_list();
                break;
            case 3:
                efficiency();
                break;
            case 4:
                hand_mode();
                break;
            default:
                printf("Неверно введен пункт меню\n");
                break;
            }
        }
    }
    return 0;
}