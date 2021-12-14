#define _POSIX_C_SOURCE 200809L
#include "tree.h"
#include "avl_tree.h"
#include "hash.h"
#include "defines.h"
#include "process.h"
#include <stdio.h>
#include <sys/time.h>
#include <inttypes.h>


void hash_menu(FILE *f)
{
    char *hash_menu = "0 - возврат в главное меню\n1 - прочитать файл в таблицу\n2 - задать размер таблицы\n"
    "3 - добавить слово в таблицу\n4 - удалить слово из таблицы + вывод кол-ва сравнений\n"
    "5 - вывести на экран таблицу\n6 - очистить avl-дерево\n";

    int choice;
    int rc;
    int size;
    while (1)
    {
        while (getchar() != '\n');
        printf("Введите размер таблицы\n");
        rc = scanf("%d", &size);
        if (rc != 1 || size <= 0)
        {
            printf("Неверно введен размер таблицы\n");
            continue;
        }
        break;
    }
    hash_table_t *ht = init_ht(size);
    while (1)
    {
        printf("Выберите пункт меню:\n");
        printf("%s", hash_menu);
        rc = scanf("%d", &choice);
        if (rc != 1)
        {
            printf("Не выбран пункт меню/n");
            while (getchar() != '\n');
            continue;
        }
        switch (choice)
        {
            case 1:
                if (read_to_ht(ht, f) != 0)
                {
                    printf("Ошибка чтения файла\n");
                    destroy_ht(ht);
                    return;
                }
                printf("ФАЙЛ ПРОЧТЕН\n");
                break;
            case 2:
                ht = new_ht_size(ht);
                printf("ТАБЛИЦА РЕСТРУКТУРИЗИРОВАНА\n");
                break;
            case 3:
                add_to_ht(ht);
                break;
            case 4:
                rc = delete_from_ht_word(ht);
                if (rc == 0)
                    printf("СЛОВО УДАЛЕНО\n");
                else
                    printf("СЛОВА НЕТ\n");
                break;
            case 5:
                print_ht(ht);
                break;
            case 6:
                destroy_ht(ht);
                ht = init_ht(size);
                printf("ТАБЛИЦА ОЧИЩЕНА И ИНИЦИАЛИЗИРОВАННА НАЧАЛЬНЫМ РАЗМЕРОМ\n");
                break;
            case 0:
                return ;
            default:
                printf("Такой пункт меню отсутствует\n");
                break;
        }
    }
}


void tree_menu(FILE *f)
{
    char *tree_menu = "0 - возврат в главное меню\n1 - прочитать файл в дерево\n2 - прочитать файл в avl-дерево\n"
    "3 - добавить слово в дерево\n4 - добавить слово в avl-дерево\n5 - удалить слово из дерева + вывод кол-ва сравнений\n"
    "6 - удалить слово из avl-дерева + вывод кол-ва сравнений\n7 - вывести на экран дерево\n8 - вывести на экран avl-дерево\n"
    "9 - очистить дерево\n10 - очистить avl-дерево\n";

    int choice;
    int rc;

    tree_node_t *tree = NULL;
    avl_tree_node_t *avl_tree = NULL;
    while (1)
    {
        printf("Выберите пункт меню:\n");
        printf("%s", tree_menu);
        rc = scanf("%d", &choice);
        if (rc != 1)
        {
            printf("Не выбран пункт меню/n");
            while (getchar() != '\n');
            continue;
        }
        switch (choice)
        {
            case 1:
                if (read_to_tree(&tree, f) != 0)
                {
                    printf("Ошибка чтения файла\n");
                    destroy_tree(tree, free);
                    return;
                }
                printf("ФАЙЛ ПРОЧТЕН\n");
                break;
            case 2:
                if (read_to_avl_tree(&avl_tree, f) != 0)
                {
                    printf("Ошибка чтения файла\n");
                    destroy_avl_tree(avl_tree, free);
                    return;
                }
                printf("ФАЙЛ ПРОЧТЕН\n");
                break;
            case 3:
                word_to_tree(&tree);
                printf("СЛОВО ЗАПИСАНО\n");
                break;
            case 4:
                word_to_avl_tree(&avl_tree);
                printf("СЛОВО ЗАПИСАНО\n");
                break;
            case 5:
                rc = word_from_tree(&tree);
                if (rc == 0)
                    printf("СЛОВО УДАЛЕНО\n");
                else
                    printf("СЛОВА НЕТ\n");
                break;
            case 6:
                rc = word_from_avl_tree(&avl_tree);
                if (rc == 0)
                    printf("СЛОВО УДАЛЕНО\n");
                else
                    printf("СЛОВА НЕТ\n");
                break;
            case 7:
                if (tree == NULL)
                {
                    printf("Дерево пусто\n");
                    break;
                }
                print_tree(tree);
                break;
            case 8:
                if (avl_tree == NULL)
                {
                    printf("Дерево пусто\n");
                    break;
                }
                print_avl_tree(avl_tree);
                break;
            case 9:
                destroy_tree(tree, free);
                tree = NULL;
                printf("ДЕРЕВО ОЧИЩЕНО\n");
                break;
            case 10:
                destroy_avl_tree(avl_tree, free);
                avl_tree = NULL;
                printf("AVL-ДЕРЕВО ОЧИЩЕНО\n");
                break;
            case 0:
                return ;
            default:
                printf("Такой пункт меню отсутствует\n");
                break;
        }
    }
}


void efficiency()
{
    int WORDS;
    scanf("%d", &WORDS);
    char command[40] = " ";
    snprintf(command, 40, "python3 wordsgen.py %d", WORDS);
    system(command);

    FILE *in = fopen("in.txt", "r");

    tree_node_t *tree = NULL;
    avl_tree_node_t *avl_tree = NULL;
    hash_table_t *ht = init_ht((int)(WORDS * 1.25));

    read_to_tree(&tree, in);
    read_to_avl_tree(&avl_tree, in);
    read_to_ht(ht, in);

    int64_t sum_simple, sum = 0;;
	struct timeval tv_start, tv_stop;
    int count = 50;

    int cmprs = 0;
    for (int i = 0; i < count; i++)
    {
        cmprs = 0;
        destroy_tree(tree, free);
        tree = NULL;
        read_to_tree(&tree, in);
        gettimeofday(&tv_start, NULL);
        delete_tree_node(tree, "sdcsdc", &cmprs);
        gettimeofday(&tv_stop, NULL);
        sum_simple = (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
        sum += sum_simple;
    }
    printf("Время для ДДП (мс): %" PRId64 "\n", sum / count);
    printf("Память для ДДП (байт): %ld\n", sizeof(tree_node_t) * WORDS);
    printf("Кол-во сравнений для ДДП: %d\n\n", cmprs);

    sum = 0;
    for (int i = 0; i < count; i++)
    {
        cmprs = 0;
        destroy_avl_tree(avl_tree, free);
        avl_tree = NULL;
        read_to_avl_tree(&avl_tree, in);
        gettimeofday(&tv_start, NULL);
        delete_avl_tree_node(avl_tree, "sdcsdc", &cmprs);
        gettimeofday(&tv_stop, NULL);
        sum_simple = (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
        sum += sum_simple;
    }
    printf("Время для АВЛ (мс): %" PRId64 "\n", sum / count);
    printf("Память для АВЛ (байт): %ld\n", sizeof(avl_tree_node_t) * WORDS);
    printf("Кол-во сравнений для АВЛ: %d\n\n", cmprs);


    sum = 0;
    for (int i = 0; i < count; i++)
    {
        cmprs = 0;
        destroy_ht(ht);
        ht = NULL;
        ht = init_ht((int)(WORDS * 1.25));
        read_to_ht(ht, in);
        gettimeofday(&tv_start, NULL);
        delete_from_ht(ht, "sdcsdc", &cmprs);
        gettimeofday(&tv_stop, NULL);
        sum_simple = (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
        sum += sum_simple;
    }
    printf("Время для ХТ (мс): %" PRId64 "\n", sum / count);
    printf("Память для ХТ (байт): %ld\n", sizeof(List) * WORDS);
    printf("Кол-во сравнений для ХТ: %d\n\n", cmprs);


    sum = 0;
    cmprs = 0;
    for (int i = 0; i < 1; i++)
    {
        rewind(in);
        char str[20] = " ";
        gettimeofday(&tv_start, NULL);
        while (fgets(str, 20, in))
        {
            cmprs++;
            if (strcmp(str, "sdcsdc\n") == 0)
                break;
        }
        gettimeofday(&tv_stop, NULL);
        sum_simple = (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
        sum += sum_simple;
    }
    printf("Время для файла (мс): %" PRId64 "\n", sum / count);
    printf("Кол-во сравнений для файла: %d\n\n", cmprs);

    fclose(in);
}


int main(int argc, char **argv)
{
    if (argc == 1)
    {
        printf("No input file\n");
        return FILE_ERROR;
    }
    if (argc == 2)
    {
        FILE *f = fopen(argv[1], "r");
        if (f == NULL)
        {
            printf("No input file\n");
            return FILE_ERROR;
        }
        char *main_menu = "0 - выход из программы\n1 - работа с бинарными деревьями\n2 - работа с хеш-таблицей\n3 - оценка эффективности\n";
        int choice;
        int rc;
        while (1)
        {
            printf("Выберите пункт меню:\n");
            printf("%s", main_menu);
            rc = scanf("%d", &choice);
            if (rc != 1)
            {
                printf("Не выбран пункт меню/n");
                while (getchar() != '\n');
                continue;
            }
            switch (choice)
            {
                case 1:
                    tree_menu(f);
                    break;
                case 2:
                    hash_menu(f);
                    break;
                case 3:
                    efficiency(f);
                    break;
                case 0:
                    fclose(f);
                    return 0;
                default:
                    printf("Такой пункт меню отсутствует\n");
                    break;
            }
        }
    }
    return 0;
}