#ifndef _HASH_H_

#include "list.h"
#include <string.h>

#define START_SIZE 13

typedef struct
{
    List **bucket;
    int size;
}hash_table_t;

// Хеш-функция
int hash_func(int curr_size, const char *value);

// Добавление слова в ХТ
void add_ht(hash_table_t *table, char *word);

// Инициализация ХТ
hash_table_t *init_ht(int size);

// Поиск слова в ХТ
List *find_ht(hash_table_t *ht, char *word, int *n);

// Удаление слова из ХТ
void delete_from_ht(hash_table_t *ht, char *word, int *n);

// Очистка ХТ
void destroy_ht(hash_table_t *ht);

// Вывод ХТ 
void print_ht(hash_table_t *ht);

// Реструктуризация ХТ
void restruct(hash_table_t *dst, hash_table_t *src);


#define _HASH_H_
#endif