#include <stdio.h>
#include <stdlib.h>
#include "occurrences.h"
#include "huffman_tree.h"
#include "binary_codes.h"
#include "bit_buffer.h"
#include "write_uncompress.h"

/**
 * Get the accual compressed data
 * @param filepath The compressed file's path
 * @param start_i Index of the first compressed byte (after the compression header)
 */
unsigned char *extract_compressed(char *filepath, int start_i, int *nbr_bits)
{
    FILE *input = fopen(filepath, "rb");
    long compressed_bytes = 0;
    unsigned char *compressed_data = NULL;
    //char c = 0;

    if (!input) {
        perror("Could not open compressed file");
        return NULL;
    }
    fseek(input, 0, SEEK_END);
    compressed_bytes = ftell(input) - start_i;
    *nbr_bits = compressed_bytes;
    compressed_data = malloc(compressed_bytes * sizeof(char));
    if (!compressed_data) {
        fclose(input);
        return NULL;
    }
    fseek(input, start_i, SEEK_SET);
    fread(compressed_data, sizeof(char), compressed_bytes, input);
    return compressed_data;
}

char_node_t **read_header(char *filepath, int *end_header_i, int *nbr_bits_compress)
{
    char_node_t **occurr_arr = NULL;
    FILE *input = fopen(filepath, "rb");

    if (!input) {
        perror("Could not open compressed file");
        return NULL;
    }
    fread(nbr_bits_compress, sizeof(int), 1, input);
    occurr_arr = read_header_occurrs(input);
    if (!occurr_arr) {
        fclose(input);
        return NULL;
    }
    *end_header_i = ftell(input);
    fclose(input);
    return occurr_arr;
}

int uncompress(char *filepath)
{
    int end_header_i = 0;
    int bits_compress = 0;
    char_node_t **occurr_arr = read_header(filepath, &end_header_i, &bits_compress);
    char_node_t *tree = NULL;
    code_t **codes = NULL;
    unsigned char *compressed_bytes = NULL;
    int nbr_bytes = 0;

    if (!occurr_arr) {
        return EXIT_FAILURE;
    }
    tree = gen_tree(occurr_arr);
    if (!tree) {
        free_occurr_arr(occurr_arr);
        return EXIT_FAILURE;
    }
    codes = get_codes(tree, get_occurr_len(occurr_arr));
    if (!codes) {
        free_tree(tree);
        free_occurr_arr(occurr_arr);
        return EXIT_FAILURE;
    }
    compressed_bytes = extract_compressed(filepath, end_header_i, &nbr_bytes);
    if (!compressed_bytes) {
        free_tree(tree);
        free_occurr_arr(occurr_arr);
        free_codes(codes);
        return EXIT_FAILURE;
    }
    write_uncompressed(codes, compressed_bytes, nbr_bytes, get_tree_depth(tree), bits_compress);
    free_tree(tree);
    free_occurr_arr(occurr_arr);
    free_codes(codes);
    free(compressed_bytes);
    return EXIT_SUCCESS;
}