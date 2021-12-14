#include "avl_tree.h"


avl_tree_node_t *create_avl_tree_node(char *word)
{
    avl_tree_node_t *node = malloc(sizeof(avl_tree_node_t));
    if (node)
    {
        node->word = word;
        node->height = 1;
        node->left = NULL;
        node->right = NULL;
        node->parent = NULL;
    }

    return node;
}


void destroy_avl_tree_node(avl_tree_node_t *node, void (*delete_content)(void *))
{
    if (delete_content != NULL)
        delete_content(node->word);
    free(node);
}


void destroy_avl_tree(avl_tree_node_t *tree, void (*delete_content)(void *))
{
    if (tree == NULL)
        return;
    destroy_avl_tree(tree->left, delete_content);
    destroy_avl_tree(tree->right, delete_content);
    destroy_avl_tree_node(tree, delete_content);
}


int height(avl_tree_node_t *node)
{
    return node ? node->height : 0;
}


void fix_height(avl_tree_node_t *node)
{
    int hl = height(node->left);
    int hr = height(node->right);
    node->height = (hl > hr ? hl : hr) + 1;
}


avl_tree_node_t *insert_avl_tree(avl_tree_node_t *tree, avl_tree_node_t *node)
{
    int cmp;

    if (tree == NULL)
        return node;
    
    cmp = strcmp(node->word, tree->word);
    if (cmp == 0)
        return tree;
    else if (cmp < 0)
    {
        tree->left = insert_avl_tree(tree->left, node);
        tree->left->parent = tree;
    }
    else
    {
        tree->right = insert_avl_tree(tree->right, node);
        tree->right->parent = tree;
    }
    tree = balance(tree);
    return tree;
}


avl_tree_node_t *lookup_avl_tree(avl_tree_node_t *tree, char *word, int *n)
{
    if (!word || !tree)
        return NULL;
    avl_tree_node_t *res = NULL;
    int cmp = strcmp(word, tree->word);
    if (n != NULL)
        (*n)++;
    if (cmp < 0)
    {
        res = lookup_avl_tree(tree->left, word, n);
    }
    else if (cmp > 0)
    {
        res = lookup_avl_tree(tree->right, word, n);
    }
    else
        res = tree;
    return res;
}


void apply(avl_tree_node_t *tree, void (*f)(avl_tree_node_t *node))
{
    if (tree == NULL)
        return;
    
    apply(tree->left, f);
    apply(tree->right, f);
    f(tree);
}


avl_tree_node_t *delete_avl_tree_node(avl_tree_node_t *tree, char *word, int *n)
{
    avl_tree_node_t *deleting_node = lookup_avl_tree(tree, word, n);
    if (deleting_node == NULL)
        return tree;

    if (deleting_node->parent == NULL && deleting_node->left == NULL && deleting_node->right == NULL)
    {
        destroy_avl_tree_node(deleting_node, free);
        return NULL;
    }
    if (deleting_node->parent == NULL)
    {
        if (deleting_node->left && deleting_node->right == NULL)
        {
            tree = deleting_node->left;
            apply(tree, fix_height);
            destroy_avl_tree_node(deleting_node, free);
        }
        else if (deleting_node->left == NULL && deleting_node->right)
        {
            tree = deleting_node->right;
            apply(tree, fix_height);
            destroy_avl_tree_node(deleting_node, free);
        }
        else if (deleting_node->left && deleting_node->right)
        {
            avl_tree_node_t *inserting_node = deleting_node;
            inserting_node = inserting_node->right;
            while (inserting_node->left)
            {
                inserting_node = inserting_node->left;
            }
            free(deleting_node->word);
            deleting_node->word = inserting_node->word;
            if (strcmp(inserting_node->parent->word, inserting_node->word) > 0)
            {
                inserting_node->parent->left = NULL;
                apply(deleting_node, fix_height);
                destroy_avl_tree_node(inserting_node, NULL);
            }
            else
            {
                inserting_node->parent->right = NULL;
                apply(deleting_node, fix_height);
                destroy_avl_tree_node(inserting_node, NULL);
            }
        }
        tree = balance(tree);
        return tree;
    }

    if (deleting_node->left == NULL && deleting_node->right == NULL)
    {
        if (strcmp(deleting_node->parent->word, deleting_node->word) > 0)
        {
            deleting_node->parent->left = NULL;
            fix_height(deleting_node->parent);
            destroy_avl_tree_node(deleting_node, free);
        }
        else
        {
            deleting_node->parent->right = NULL;
            fix_height(deleting_node->parent);
            destroy_avl_tree_node(deleting_node, free);
        }
    }
    else if (deleting_node->left && deleting_node->right == NULL)
    {
        if (strcmp(deleting_node->parent->word, deleting_node->word) > 0)
        {
            deleting_node->parent->left = deleting_node->left;
            deleting_node->left->parent = deleting_node->parent;
            apply(deleting_node->parent, fix_height);
            destroy_avl_tree_node(deleting_node, free);
        }
        else
        {
            deleting_node->parent->right = deleting_node->left;
            deleting_node->left->parent = deleting_node->parent;
            apply(deleting_node->parent, fix_height);
            destroy_avl_tree_node(deleting_node, free);
        }
    }
    else if (deleting_node->left == NULL && deleting_node->right)
    {
        if (strcmp(deleting_node->parent->word, deleting_node->word) > 0)
        {
            deleting_node->parent->left = deleting_node->right;
            deleting_node->right->parent = deleting_node->parent;
            apply(deleting_node, fix_height);
            destroy_avl_tree_node(deleting_node, free);
        }
        else
        {
            deleting_node->parent->right = deleting_node->right;
            deleting_node->right->parent = deleting_node->parent;
            apply(deleting_node, fix_height);
            destroy_avl_tree_node(deleting_node, free);
        }
    }
    else if (deleting_node->left && deleting_node->right)
    {
        avl_tree_node_t *inserting_node = deleting_node;
        inserting_node = inserting_node->right;
        while (inserting_node->left)
        {
            inserting_node = inserting_node->left;
        }
        free(deleting_node->word);
        deleting_node->word = inserting_node->word;
        if (strcmp(inserting_node->parent->word, inserting_node->word) > 0)
        {
            inserting_node->parent->left = NULL;
            apply(deleting_node, fix_height);
            destroy_avl_tree_node(inserting_node, NULL);
        }
        else
        {
            inserting_node->parent->right = NULL;
            apply(deleting_node, fix_height);
            destroy_avl_tree_node(inserting_node, NULL);
        }
    }
    tree = balance(tree);
    return tree;
}


int bfactor(avl_tree_node_t *node)
{
    return height(node->right) - height(node->left);
}


avl_tree_node_t *rotate_right(avl_tree_node_t *node)
{
    avl_tree_node_t *tmp = node->left;
    node->left = tmp->right;
    tmp->right = node;
    fix_height(node);
    fix_height(tmp);
    return tmp;
}

avl_tree_node_t *rotateleft(avl_tree_node_t *node)
{
    avl_tree_node_t *rmp = node->right;
    node->right = rmp->left;
    rmp->left = node;
    fix_height(node);
    fix_height(rmp);
    return rmp;
}


avl_tree_node_t *balance(avl_tree_node_t *node)
{
    fix_height(node);
    if (bfactor(node) == 2)
    {
        if (bfactor(node->right) < 0)
            node->right = rotate_right(node->right);
        return rotateleft(node);
    }
    if (bfactor(node) == -2)
    {
        if (bfactor(node->left) > 0)
            node->left = rotateleft(node->left);
        return rotate_right(node);
    }
    return node;
}


void avl_bst_print_dot_null(char * word, int nullcount, FILE* stream)
{
    fprintf(stream, "    null%d [shape=point];\n", nullcount);
    fprintf(stream, "    %s -> null%d;\n", word, nullcount);
}

void avl_bst_print_dot_aux(avl_tree_node_t* node, FILE* stream)
{
    static int nullcount = 0;

    if (node->left)
    {
        fprintf(stream, "    %s -> %s;\n", node->word, node->left->word);
        avl_bst_print_dot_aux(node->left, stream);
    }
    else
        avl_bst_print_dot_null(node->word, nullcount++, stream);

    if (node->right)
    {
        fprintf(stream, "    %s -> %s;\n", node->word, node->right->word);
        avl_bst_print_dot_aux(node->right, stream);
    }
    else
        avl_bst_print_dot_null(node->word, nullcount++, stream);
}

void avl_bst_print_dot(avl_tree_node_t* tree, FILE* stream)
{
    fprintf(stream, "digraph BST {\n");
    fprintf(stream, "    node [fontname=\"Arial\"];\n");

    if (!tree)
        fprintf(stream, "\n");
    else if (!tree->right && !tree->left)
        fprintf(stream, "    %s;\n", tree->word);
    else
        avl_bst_print_dot_aux(tree, stream);

    fprintf(stream, "}\n");
}