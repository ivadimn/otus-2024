#include "tree.h"
#include "utils.h"
#include "sort.h"
#include <stdlib.h>
#include <stddef.h>

static size_t count_nodes = 0;
static size_t node_index = 0;

static node_t*  _create_node(unsigned char* key, size_t value) {
    node_t* node = (node_t*) alloc(sizeof(node_t));
    node->data.key = (unsigned char*) alloc(sizeof(unsigned char*) * (strlen((char*)key) + 1));
    strcpy((char*)node->data.key, (char*)key);
    node->data.value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}
void tree_delete(node_t* tree) {
    if ( tree == NULL)
        return;
    if (tree->left)
        tree_delete(tree->left);
    if (tree->right)
        tree_delete(tree->right);
    free(tree);
}

void push(node_t** tree, unsigned char* key, size_t value) {
    
    node_t *tmp = *tree;
    int rc;

    if (tmp == NULL) {
        *tree = _create_node(key, value);
        return;
    }
    rc = CMP(tmp->data.key, key);
    if (rc == 0) {
        tmp->data.value += value;
    } else if (rc < 0) {
        if (tmp->left == NULL)
           tmp->left = _create_node(key, value);
        else
            push(&(tmp->left), key, value);
    } else {
        if (tmp->right == NULL)
           tmp->right = _create_node(key, value);
        else
            push(&(tmp->right), key, value);
    }
}

void print_tree(node_t* tree) {
    
    if (tree == NULL)
        return;
    print_tree(tree->left);
    print_tree(tree->right);
    printf("Key = %s, Value = %ld\n", tree->data.key, tree->data.value);

}
static void _calc_count(node_t* tree) {
    if (tree == NULL)
        return;
    if (tree->left)
        _calc_count(tree->left);
    if (tree->right)
        _calc_count(tree->right);
    count_nodes++;    
}

size_t tree_count_nodes(node_t* tree) {
    _calc_count(tree);
    size_t count = count_nodes;
    count_nodes = 0;
    node_index = 0;
    return count;
}

void get_leafs(node_t *tree, unsigned char** keys, size_t* values) {
    
    if (tree == NULL)
        return;
    if (tree->left)
        get_leafs(tree->left, keys, values);
    if (tree->right)
        get_leafs(tree->right, keys, values);
    keys[node_index] = tree->data.key;
    values[node_index] = tree->data.value;
    node_index++;
}
