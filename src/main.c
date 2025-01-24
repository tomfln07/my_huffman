#include <stdio.h>
#include <stdlib.h>
#include "readfile.h"
#include "occurrences.h"
#include "huffman_tree.h"
#include "binary_codes.h"
#include "write_compress.h"

int main(int ac, char **av)
{
    char *file_buff = NULL;
    long file_len = 0;
    char_node_t **occurr_arr = NULL;
    int occurr_len = 0;
    char_node_t *tree = NULL;
    code_t **codes = NULL;

    if (ac != 2) {
        printf("USAGE:\n\t./huffman [file_path]\n");
        return EXIT_SUCCESS;
    }
    file_buff = readfile(av[1], &file_len);
    if (!file_buff) {
        return EXIT_FAILURE;
    }
    occurr_arr = get_occurr(file_buff, file_len);
    if (!occurr_arr) {
        free(file_buff);
        return EXIT_FAILURE;
    }
    occurr_len = get_occurr_len(occurr_arr);
    tree = gen_tree(occurr_arr);
    codes = get_codes(tree, occurr_len);
    write_compress(codes, file_buff, file_len);
    free(file_buff);
    free_occurr_arr(occurr_arr);
    free_tree(tree);
    free_codes(codes);
    return EXIT_SUCCESS;
}
