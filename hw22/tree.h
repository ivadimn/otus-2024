#ifndef _TREE_H_
#define _TREE_H_

#include <stddef.h>
#include <string.h>
#include <stdio.h>

#define CMP_EQ(a, b)  (strcmp((char*)(a), (char*)(b)) == 0)
#define CMP(a, b)  strcmp((char*)(a), (char*)(b))

typedef struct {
    unsigned char* key;
    size_t value;
} data_t;


typedef struct node_t {
    data_t data;
    struct node_t* left;
    struct node_t* right;
    size_t count;
} node_t;


void tree_delete(node_t* tree);

void push(node_t** node, unsigned char* key, size_t value);
void print_tree(node_t* node);
size_t tree_count_nodes(node_t* tree);
void get_leafs(node_t *tree, unsigned char** keys, size_t* values);

#endif