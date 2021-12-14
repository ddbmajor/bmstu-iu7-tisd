#ifndef AVL_TREE_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct avl_tree_node avl_tree_node_t;

struct avl_tree_node
{
    char *word;
    int height;
    avl_tree_node_t *left;
    avl_tree_node_t *right;
    avl_tree_node_t *parent;
};

// Создание узла дерева
avl_tree_node_t *create_avl_tree_node(char *word);

// Удаление узла дерева
void destroy_avl_tree_node(avl_tree_node_t *node, void (*delete_content)(void *));

// Удаление всего дерева
void destroy_avl_tree(avl_tree_node_t *tree, void (*delete_content)(void *));

// Получение высоты узла
int height(avl_tree_node_t *node);

// Расчет высоты узла
void fix_height(avl_tree_node_t *node);

// Вставка узла в дерево
avl_tree_node_t *insert_avl_tree(avl_tree_node_t *tree, avl_tree_node_t *node);

// Поиск слова в дереве
avl_tree_node_t *lookup_avl_tree(avl_tree_node_t *tree, char *word, int *n);

// Обход дерева(обратный)
void apply(avl_tree_node_t *tree, void (*f)(avl_tree_node_t *node));

// Удаление слова из дерева
avl_tree_node_t *delete_avl_tree_node(avl_tree_node_t *tree, char *word, int *n);

// Нахождение разницы в высотах
int bfactor(avl_tree_node_t *node);

// Правый поворот
avl_tree_node_t *rotate_right(avl_tree_node_t *node);

// Левый поворот
avl_tree_node_t *rotateleft(avl_tree_node_t *node);

// Балансировка
avl_tree_node_t *balance(avl_tree_node_t *node);

// Функции дл явывода дерева на экран
void avl_bst_print_dot_null(char * word, int nullcount, FILE* stream);
void avl_bst_print_dot_aux(avl_tree_node_t* node, FILE* stream);
void avl_bst_print_dot(avl_tree_node_t* tree, FILE* stream);


#define AVL_TREE_H_
#endif