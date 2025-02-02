// Functions used to help tree's main functions

#include <stdlib.h>
#include "huffman_tree.h"
#include "occurrences.h"

// for dbg
void print_tree(char_node_t *node, int depth)
{
    if (!node) {
        return;
    }
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
    if (node->type == LEAF) {
        printf("Leaf: '%c' (freq: %d)\n", node->c, node->freq);
    } else {
        printf("Internal (freq: %d, ascii: %c)\n", node->freq, node->c);
    }
    print_tree(node->l, depth + 1);
    print_tree(node->r, depth + 1);
}

void free_nodes_list(char_node_t **nodes, int len)
{
    if (!nodes) {
        return;
    }
    for (int i = 0; i < len; i++) {
        if (nodes[i]) {
            free(nodes[i]);
        }
    }
    free(nodes);
}

void free_tree(char_node_t *tree)
{
    if (!tree) {
        return;
    }
    if (tree->l) {
        free_tree(tree->l);
    }
    if (tree->r) {
        free_tree(tree->r);
    }
    free(tree);
}

/**
 * Creates a deep copy of the occurrence array 
 * that'll be used as a priority queue for the creation of the huffman tree
 * If it fails, the occurrence array won't be copied to dest
 * @param src The array to copy
 * @param dest Where to copy
 * @param len The size of the original array
 */
int occurr_cpy(char_node_t **src, char_node_t ***dest, int len)
{
    char_node_t **cpy_arr = NULL;

    if (!src) {
        perror("Invalid NULL occurrence array passed for deep copy");
        return EXIT_FAILURE;
    }
    cpy_arr = calloc(len + 1, sizeof(char_node_t *));
    if (!cpy_arr) {
        perror("Could not allocate memory for the deep copy of the occurrence array");
        return EXIT_FAILURE;
    }
    for (int i = 0; i < len; i++) {
        if (!src[i]) {
            cpy_arr[i] = NULL;
            continue;
        }
        cpy_arr[i] = malloc(sizeof(char_node_t));
        if (!cpy_arr[i]) {
            perror("Could not malloc for copied occurrecne node");
            free_nodes_list(cpy_arr, len);
            return EXIT_FAILURE;
        }
        cpy_arr[i]->c = src[i]->c;
        cpy_arr[i]->freq = src[i]->freq;
        cpy_arr[i]->l = NULL;
        cpy_arr[i]->r = NULL;
        cpy_arr[i]->type = src[i]->type;
    }
    cpy_arr[len] = NULL;
    *dest = cpy_arr;
    return EXIT_SUCCESS;
}

int get_tree_depth(char_node_t *tree)
{
    int l_depth = -1;
    int r_depth = -1;

    if (!tree) {
        printf("Invalid NULL tree argument for get tree depth");
        return -1;
    }
    if (tree->l) {
        l_depth = get_tree_depth(tree->l);
    }
    if (tree->r) {
        r_depth = get_tree_depth(tree->r);
    }
    if (l_depth > r_depth) {
        return l_depth + 1;
    }
    return r_depth + 1;
}


char_node_t *new_internal_node(char_node_t *node1, char_node_t *node2)
{
    char_node_t *new_node = NULL;

    if (!node1 || !node2) {
        perror("Invalid nodes args for new internal node");
        return NULL;
    }
    new_node = malloc(sizeof(char_node_t));
    if (!new_node) {
        perror("Could no allocate memory for a new internal node");
        return NULL;
    }
    new_node->freq = node1->freq + node2->freq;
    if (node1->freq <= node2->freq) {
        new_node->l = node1;
        new_node->r = node2;
    } else {
        new_node->l = node2;
        new_node->r = node1;
    }
    new_node->type = INTERNAL;
    return new_node;
}