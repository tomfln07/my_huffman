#ifndef BINARY_CODES_H
    #define BINARY_CODES_H
    #include "huffman_tree.h"

typedef struct code_s
{
    char c;
    char *code;
} code_t;

/**
 * Retrieve the code for every chars based on the constructed binary tree
 */
code_t **get_codes(char_node_t *tree, int nbr_chars);
void free_codes(code_t **codes);
#endif