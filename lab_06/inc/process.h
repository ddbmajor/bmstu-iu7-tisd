#ifndef PROCESS_H_

#define _POSIX_C_SOURCE 200809L

#include "tree.h"
#include "avl_tree.h"
#include "hash.h"
#include "list.h"
#include "defines.h"
#include <stdio.h>


int read_to_tree(tree_node_t **tree, FILE *f);
int read_to_avl_tree(avl_tree_node_t **tree, FILE *f);
void word_to_tree(tree_node_t **tree);
void word_to_avl_tree(avl_tree_node_t **tree);
int word_from_tree(tree_node_t **tree);
int word_from_avl_tree(avl_tree_node_t **tree);
void print_tree(tree_node_t *tree);
void print_avl_tree(avl_tree_node_t *tree);

int read_to_ht(hash_table_t *ht, FILE *f);
hash_table_t *new_ht_size(hash_table_t *ht);
void add_to_ht(hash_table_t *ht);
int delete_from_ht_word(hash_table_t *ht);


#define PROCESS_H_
#endif