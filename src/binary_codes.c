#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "binary_codes.h"
#include "huffman_tree.h"

void free_codes(code_t **codes)
{
    if (!codes) {
        return;
    }
    for (int i = 0; codes[i]; i++) {
        if (codes[i]->code) {
            free(codes[i]->code);
            free(codes[i]);
        }
    }
    free(codes);
}

code_t *new_code(char bin_path[], char c)
{
    code_t *new_code = malloc(sizeof(code_t));
    int bin_path_len = strlen(bin_path);
    char *final_path = malloc(bin_path_len + 1);

    strcpy(final_path, bin_path);
    final_path[bin_path_len] = '\0';

    if (!new_code) {
        perror("Could not alocate memory for a new code");
        return NULL;
    }
    new_code->c = c;
    new_code->code = final_path;
    return new_code;
}

int find_bin_codes(char_node_t *node, int tree_depth, code_t **codes, int *nbr_codes, char bin_path[])
{
    char path_l[tree_depth + 1];
    char path_r[tree_depth + 1];

    if (!node->l && !node->r) {
        codes[*nbr_codes] = new_code(bin_path, node->c);
        if (!codes[*nbr_codes]) {
            return EXIT_FAILURE;
        }
        (*nbr_codes)++;
        return EXIT_SUCCESS;
    }
    strcpy(path_l, bin_path);
    strcpy(path_r, bin_path);
    if (node->l) {
        strcat(path_l, "0");
        if (find_bin_codes(node->l, tree_depth, codes, nbr_codes, path_l) == EXIT_FAILURE) {
            return EXIT_FAILURE;
        }
    }
    if (node->r) {
        strcat(path_r, "1");
        if (find_bin_codes(node->r, tree_depth, codes, nbr_codes, path_r) == EXIT_FAILURE) {
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

code_t **get_codes(char_node_t *tree, int nbr_chars)
{
    code_t **codes = NULL;
    int nbr_codes = 0;
    int tree_depth = -1;

    if (!tree) {
        perror("Invalid NULL tree node from get_codes");
        return NULL;
    }
    tree_depth = get_tree_depth(tree);
    codes = calloc(nbr_chars + 1, sizeof(code_t *));
    if (!codes) {
        perror("Could not allocate memory for codes array");
        return NULL;
    }
    char path[tree_depth];
    memset(path, '\0', tree_depth);
    if (find_bin_codes(tree, tree_depth, codes, &nbr_codes, path) == EXIT_FAILURE) {
        return NULL;
    }
    return codes;
}