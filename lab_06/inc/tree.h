#ifndef _TREE_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct tree_node tree_node_t;

struct tree_node
{
    char *word;
    tree_node_t *left;
    tree_node_t *right;
    tree_node_t *parent;
};

// Создание узла дерева
tree_node_t * create_tree_node(char *word);

// Удаление узла дерева
void destroy_tree_node(tree_node_t *node, void delete_content(void *));

// Удаление всего дерева
void destroy_tree(tree_node_t *tree, void (*delete_content)(void *));

// Вставка узла в дерево
tree_node_t *insert_tree(tree_node_t *tree, tree_node_t *node);

// Поиск слова в дереве
tree_node_t* lookup_tree(tree_node_t *tree, char *word, int *n);

// Удаление слова из дерева
tree_node_t *delete_tree_node(tree_node_t *tree, char *word, int *n);

// Функции для вывода дерева на экран
void bst_print_dot_null(char * word, int nullcount, FILE* stream);
void bst_print_dot_aux(tree_node_t* node, FILE* stream);
void bst_print_dot(tree_node_t* tree, FILE* stream);

#define _TREE_H_
#endif