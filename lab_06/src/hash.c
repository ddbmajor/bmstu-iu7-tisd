#include "hash.h"


int hash_func(int curr_size, const char *value)
{
    int res = {0};
    for (int i = 0; value[i] != '\0'; ++i)
    {
        res += value[i];
    }
    return res % curr_size;
}

void add_ht(hash_table_t *table, char *word)
{
    int key = hash_func(table->size, word);
    List *node = create_list_node(word);
    table->bucket[key] = add_to_list(table->bucket[key], node);
}

hash_table_t *init_ht(int size)
{
    hash_table_t *ht = malloc(sizeof(hash_table_t));
    if (ht == NULL)
        return NULL;
    ht->bucket = calloc(size, sizeof(List *));
    if (ht->bucket == NULL)
    {
        free(ht);
        return NULL;
    }
    ht->size = size;

    return ht;
}

List *find_ht(hash_table_t *ht, char *word, int *n)
{
    return find_in_list(ht->bucket[hash_func(ht->size,  word)], word, n);
}

void destroy_ht(hash_table_t *ht)
{
    for (int i = 0; i < ht->size; ++i)
    {
        free_list(ht->bucket[i]);
    }
    free(ht->bucket);
    free(ht);
}

void print_ht(hash_table_t *ht)
{
    for (int i = 0; i < ht->size; ++i)
    {
        if (ht->bucket[i])
        {
            printf("%d: ", i);
            list_print(ht->bucket[i]);
            printf("\n");
        }
    }
}


void delete_from_ht(hash_table_t *ht, char *word, int *n)
{
    List *node = find_ht(ht, word, n);

    ht->bucket[hash_func(ht->size, word)] = delete_from_list(ht->bucket[hash_func(ht->size, word)], node);
}


void restruct(hash_table_t *dst, hash_table_t *src)
{
    for (int i = 0; i < src->size; i++)
    {
        if(src->bucket[i] != NULL)
        {
            List *list = src->bucket[i];
            for (; list; list = list->next)
            {
                add_ht(dst, list->word);
            }
        }
    }
}