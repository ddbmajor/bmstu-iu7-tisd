#include "tree.h"


tree_node_t *create_tree_node(char *word)
{
    tree_node_t *node = malloc(sizeof(tree_node_t));
    if (node)
    {
        node->word = word;
        node->left = NULL;
        node->right = NULL;
        node->parent = NULL;
    }

    return node;
}


void destroy_tree_node(tree_node_t *node, void (*delete_content)(void *))
{
    if (delete_content != NULL)
        delete_content(node->word);
    free(node);
}


void destroy_tree(tree_node_t *tree, void (*delete_content)(void *))
{
    if (tree == NULL)
        return;
    destroy_tree(tree->left, delete_content);
    destroy_tree(tree->right, delete_content);
    destroy_tree_node(tree, delete_content);
}


tree_node_t *insert_tree(tree_node_t *tree, tree_node_t *node)
{
    int cmp;

    if (tree == NULL)
        return node;
    
    cmp = strcmp(node->word, tree->word);
    if (cmp == 0)
        return tree;
    else if (cmp < 0)
    {
        tree->left = insert_tree(tree->left, node);
        tree->left->parent = tree;
    }
    else
    {
        tree->right = insert_tree(tree->right, node);
        tree->right->parent = tree;
    }
    return tree;
}


tree_node_t *lookup_tree(tree_node_t *tree, char *word, int *n)
{
    int cmp;

    if (tree == NULL)
        return NULL;

    cmp = strcmp(word, tree->word);
    (*n)++;
    if (cmp == 0)
        return tree;
    else if (cmp < 0)
        return lookup_tree(tree->left, word, n);
    else
        return lookup_tree(tree->right, word, n);
}


tree_node_t *delete_tree_node(tree_node_t *tree, char *word, int *n)
{
    tree_node_t *deleting_node = lookup_tree(tree, word, n);
    if (deleting_node == NULL)
        return tree;

    if (deleting_node->parent == NULL && deleting_node->left == NULL && deleting_node->right == NULL)
    {
        destroy_tree_node(deleting_node, free);
        return NULL;
    }
    if (deleting_node->parent == NULL)
    {
        if (deleting_node->left && deleting_node->right == NULL)
        {
            tree = deleting_node->left;
            destroy_tree_node(deleting_node, free);
        }
        else if (deleting_node->left == NULL && deleting_node->right)
        {
            tree = deleting_node->right;
            destroy_tree_node(deleting_node, free);
        }
        else if (deleting_node->left && deleting_node->right)
        {
            tree_node_t *inserting_node = deleting_node;
            inserting_node = inserting_node->right;
            while (inserting_node->left)
            {
                inserting_node = inserting_node->left;
            }
            free(deleting_node->word);
            deleting_node->word = inserting_node->word;
            destroy_tree_node(inserting_node, NULL);
        }
        return tree;
    }

    if (deleting_node->left == NULL && deleting_node->right == NULL)
    {
        if (strcmp(deleting_node->parent->word, deleting_node->word) > 0)
        {
            deleting_node->parent->left = NULL;
            destroy_tree_node(deleting_node, free);
        }
        else
        {
            deleting_node->parent->right = NULL;
            destroy_tree_node(deleting_node, free);
        }
    }
    else if (deleting_node->left && deleting_node->right == NULL)
    {
        if (strcmp(deleting_node->parent->word, deleting_node->word) > 0)
        {
            deleting_node->parent->left = deleting_node->left;
            deleting_node->left->parent = deleting_node->parent;
            destroy_tree_node(deleting_node, free);
        }
        else
        {
            deleting_node->parent->right = deleting_node->left;
            deleting_node->left->parent = deleting_node->parent;
            destroy_tree_node(deleting_node, free);
        }
    }
    else if (deleting_node->left == NULL && deleting_node->right)
    {
        if (strcmp(deleting_node->parent->word, deleting_node->word) > 0)
        {
            deleting_node->parent->left = deleting_node->right;
            deleting_node->right->parent = deleting_node->parent;
            destroy_tree_node(deleting_node, free);
        }
        else
        {
            deleting_node->parent->right = deleting_node->right;
            deleting_node->right->parent = deleting_node->parent;
            destroy_tree_node(deleting_node, free);
        }
    }
    else if (deleting_node->left && deleting_node->right)
    {
        tree_node_t *inserting_node = deleting_node;
        inserting_node = inserting_node->right;
        while (inserting_node->left)
        {
            inserting_node = inserting_node->left;
        }
        free(deleting_node->word);
        deleting_node->word = inserting_node->word;
        destroy_tree_node(inserting_node, NULL);
    }
    return tree;
}


void bst_print_dot_null(char * word, int nullcount, FILE* stream)
{
    fprintf(stream, "    null%d [shape=point];\n", nullcount);
    fprintf(stream, "    %s -> null%d;\n", word, nullcount);
}

void bst_print_dot_aux(tree_node_t* node, FILE* stream)
{
    static int nullcount = 0;

    if (node->left)
    {
        fprintf(stream, "    %s -> %s;\n", node->word, node->left->word);
        bst_print_dot_aux(node->left, stream);
    }
    else
        bst_print_dot_null(node->word, nullcount++, stream);

    if (node->right)
    {
        fprintf(stream, "    %s -> %s;\n", node->word, node->right->word);
        bst_print_dot_aux(node->right, stream);
    }
    else
        bst_print_dot_null(node->word, nullcount++, stream);
}

void bst_print_dot(tree_node_t* tree, FILE* stream)
{
    fprintf(stream, "digraph BST {\n");
    fprintf(stream, "    node [fontname=\"Arial\"];\n");

    if (!tree)
        fprintf(stream, "\n");
    else if (!tree->right && !tree->left)
        fprintf(stream, "    %s;\n", tree->word);
    else
        bst_print_dot_aux(tree, stream);

    fprintf(stream, "}\n");
}
