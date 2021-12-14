#include "process.h"


int read_to_tree(tree_node_t **tree, FILE *f)
{
    rewind(f);
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    tree_node_t *node;

    int flag = 0;
    while ((read = getline(&line, &len, f)) != -1)
    {
        flag = 1;
        char *word = strdup(line);
        if (word[strlen(word) - 1] == '\n')
        {
            word[strlen(word) - 1] = '\0';
        }
        node = create_tree_node(word);
        *tree = insert_tree(*tree, node);
    }
    if (flag == 0)
        return EMPTY_FILE_ERROR;
    
    return 0;
}


int read_to_avl_tree(avl_tree_node_t **tree, FILE *f)
{
    rewind(f);
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    avl_tree_node_t *node;

    int flag = 0;
    while ((read = getline(&line, &len, f)) != -1)
    {
        flag = 1;
        char *word = strdup(line);
        if (word[strlen(word) - 1] == '\n')
        {
            word[strlen(word) - 1] = '\0';
        }
        node = create_avl_tree_node(word);
        *tree = insert_avl_tree(*tree, node);
    }
    if (flag == 0)
        return EMPTY_FILE_ERROR;
    
    return 0;
}


void word_to_tree(tree_node_t **tree)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    tree_node_t *node;
    while (getchar() != '\n');
    while (1)
    {
        read = getline(&line, &len, stdin);
        if (read != -1)
        {
            if (line[strlen(line) - 1] == '\n')
            {
                line[strlen(line) - 1] = '\0';
            }
            node = create_tree_node(line);
            *tree = insert_tree(*tree, node);
            return;
        }
    }
}


void word_to_avl_tree(avl_tree_node_t **tree)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    avl_tree_node_t *node;
    while (getchar() != '\n');
    while (1)
    {
        read = getline(&line, &len, stdin);
        if (read != -1)
        {
            if (line[strlen(line) - 1] == '\n')
            {
                line[strlen(line) - 1] = '\0';
            }
            node = create_avl_tree_node(line);
            *tree = insert_avl_tree(*tree, node);
            return;
        }
    }
}


int word_from_tree(tree_node_t **tree)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    tree_node_t *node = NULL;

    while (1)
    {   
        while (getchar() != '\n');
        read = getline(&line, &len, stdin);
        if (read != -1)
        {
            if (line[strlen(line) - 1] == '\n')
            {
                line[strlen(line) - 1] = '\0';
            }
            int n = 0;
            node = lookup_tree(*tree, line, NULL);
            if (node == NULL)
                return NO_WORD_ERROR;
            *tree = delete_tree_node(*tree, line, &n);
            printf("Кол-во сравнений - %d\n", n);
            return 0;
        }
    }
    return 0;
}


int word_from_avl_tree(avl_tree_node_t **tree)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    avl_tree_node_t *node = NULL;

    while (1)
    {
        while (getchar() != '\n');
        read = getline(&line, &len, stdin);
        if (read != -1)
        {
            if (line[strlen(line) - 1] == '\n')
            {
                line[strlen(line) - 1] = '\0';
            }
            int n = 0;
            node = lookup_avl_tree(*tree, line, NULL);
            if (node == NULL)
                return NO_WORD_ERROR;
            *tree = delete_avl_tree_node(*tree, line, &n);
            printf("Кол-во сравнений - %d\n", n);
            return 0;
        }
    }
    return 0;
}


void print_tree(tree_node_t *tree)
{
    FILE *f = fopen("tmp.txt", "w");
    if (f == NULL)
        return;
    bst_print_dot(tree, f);
    fclose(f);

    system("xdot tmp.txt");
}


void print_avl_tree(avl_tree_node_t *tree)
{
    FILE *f = fopen("tmp.txt", "w");
    if (f == NULL)
        return;
    avl_bst_print_dot(tree, f);
    fclose(f);

    system("xdot tmp.txt");
}


int read_to_ht(hash_table_t *ht, FILE *f)
{
    rewind(f);
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    int flag = 0;
    while ((read = getline(&line, &len, f)) != -1)
    {
        flag = 1;
        char *word = strdup(line);
        if (word[strlen(word) - 1] == '\n')
        {
            word[strlen(word) - 1] = '\0';
        }
        add_ht(ht, word);
    }
    if (flag == 0)
        return EMPTY_FILE_ERROR;
    
    return 0;
}


hash_table_t *new_ht_size(hash_table_t *ht)
{
    int rc;
    int new_size;

    while (1)
    {
        while (getchar() != '\n');
        rc = scanf("%d", &new_size);
        if (rc != 1 || new_size <= 0)
        {
            printf("Размер введен неверно\n");
            continue;
        }
        break;
    }

    hash_table_t *res = init_ht(new_size);
    restruct(res, ht);
    return res;
}


void add_to_ht(hash_table_t *ht)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while (getchar() != '\n');
    
    while (1)
    {
        read = getline(&line, &len, stdin);
        if (read != -1)
        {
            if (line[strlen(line) - 1] == '\n')
            {
                line[strlen(line) - 1] = '\0';
            }
            add_ht(ht, line);
            return;
        }
    }
}


int delete_from_ht_word(hash_table_t *ht)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    List *node;

    while (1)
    {
        while (getchar() != '\n');
        read = getline(&line, &len, stdin);
        if (read != -1)
        {
            if (line[strlen(line) - 1] == '\n')
            {
                line[strlen(line) - 1] = '\0';
            }
            int n = 0;
            node = find_ht(ht, line, NULL);
            if (node == NULL)
                return NO_WORD_ERROR;
            delete_from_ht(ht, line, &n);
            printf("Кол-во сравнений - %d\n", n);
            return 0;
        }
    }
    return 0;
}