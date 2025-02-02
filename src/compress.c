#include <stdlib.h>
#include "readfile.h"
#include "occurrences.h"
#include "huffman_tree.h"
#include "binary_codes.h"
#include "write_compress.h"

void free_all(unsigned char *file_buff, char_node_t **occurr_arr, char_node_t *tree, code_t **codes)
{
    if (file_buff) {
        free(file_buff);
    }
    if (occurr_arr) {
        free_occurr_arr(occurr_arr);
    }
    if (tree) {
        free_tree(tree);
    }
    if (codes) {
        free_codes(codes);
    }
}

int compress(char *file)
{
    unsigned char *file_buff = NULL;
    long file_len = 0;
    char_node_t **occurr_arr = NULL;
    int occurr_len = 0;
    char_node_t *tree = NULL;
    code_t **codes = NULL;

    file_buff = readfile(file, &file_len);
    if (!file_buff) {
        return EXIT_FAILURE;
    }
    if (file_len == 0) {
        fprintf(stderr, "The given file in empty, there's nothing to compress.\n");
        return EXIT_SUCCESS;
    }
    occurr_arr = get_occurr(file_buff, file_len);
    if (!occurr_arr) {
        free_all(file_buff, occurr_arr, tree, codes);
        return EXIT_FAILURE;
    }
    occurr_len = get_occurr_len(occurr_arr);
    if (occurr_len == -1) {
        free_all(file_buff, occurr_arr, tree, codes);
        return EXIT_FAILURE;
    }
    tree = gen_tree(occurr_arr);
    if (!tree) {
        free_all(file_buff, occurr_arr, tree, codes);
        return EXIT_FAILURE;
    }
    codes = get_codes(tree, occurr_len);
    if (!codes) {
        free_all(file_buff, occurr_arr, tree, codes);
        return EXIT_FAILURE;
    }
    write_compress(codes, file_buff, file_len, occurr_arr, occurr_len);
    free_all(file_buff, occurr_arr, tree, codes);
    return EXIT_SUCCESS;
}