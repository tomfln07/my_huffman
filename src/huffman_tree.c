#include <stdlib.h>
#include <stdio.h>
#include "huffman_tree.h"
#include "occurrences.h"

/**
 * For debug
 */
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

/**
 * Takes the two less frequent chars, merge them into an INTERNAL node,
 * and del them from the occurr array.
 * Meant to be called iterativelly to create the huffman tree.
 * @return Returns if the node creation what successful
 */
int node_creation_it(char_node_t **occurr, int occurr_len)
{
    int less_freq_i1 = -1;
    int less_freq_i2 = -1;
    char_node_t *new_internal = NULL;

    for (int i = 0; i < occurr_len; i++) {
        if (!occurr[i]) {
            continue;
        }
        if (less_freq_i1 == -1 || occurr[i]->freq < occurr[less_freq_i1]->freq) {
            less_freq_i2 = less_freq_i1;
            less_freq_i1 = i;
        } else if (less_freq_i2 == -1 || occurr[i]->freq < occurr[less_freq_i2]->freq) {
            less_freq_i2 = i;
        }
    }
    if (less_freq_i1 == -1 || less_freq_i2 == -1) {
        fprintf(stderr, "Not enough nodes to create a new internal node\n");
        return EXIT_FAILURE;
    }
    new_internal = new_internal_node(occurr[less_freq_i1], occurr[less_freq_i2]);
    if (!new_internal) {
        return EXIT_FAILURE;
    }
    occurr[less_freq_i1] = new_internal;
    occurr[less_freq_i2] = NULL;
    return EXIT_SUCCESS;
}

char_node_t *gen_tree(char_node_t **occurr)
{
    int internal_nodes_ceated = 0;
    int occurr_len = -1;
    char_node_t **prio_queue = NULL;
    char_node_t *root_node = NULL;

    if (!occurr) {
        perror("Passed NULL chars occurrence array");
        return NULL;
    }
    occurr_len = get_occurr_len(occurr);
    if (occurr_cpy(occurr, &prio_queue, occurr_len)) {
        return NULL;
    }
    while (internal_nodes_ceated < occurr_len - 1) {
        if (node_creation_it(prio_queue, occurr_len) == EXIT_FAILURE) {
            free_nodes_list(prio_queue, occurr_len);
            return NULL;
        }
        internal_nodes_ceated++;
    }
    for (int i = 0; i < occurr_len; i++) {
        if (prio_queue[i]) {
            root_node = prio_queue[i];
            free(prio_queue);
            return root_node;
        }
    }
    return NULL;
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
