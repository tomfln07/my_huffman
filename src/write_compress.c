#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "write_compress.h"

/**
 * Takes a char, and get its binary represention
 * from the huffman tree and adds it to a buffer
 */
char *add_bin_code_to_buff(char c, code_t **codes, char *compress_buff, int *buff_size)
{
    char *code = NULL;
    int new_buff_size = -1;

    if (!codes || !compress_buff || *buff_size < 0) {
        perror("Invalid NULL arg(s)");
        return NULL;
    }
    for (int j = 0; codes[j]; j++) {
        if (c == codes[j]->c) {
            code = codes[j]->code;
            break;
        }
    }
    new_buff_size = *buff_size + strlen(code);
    compress_buff = realloc(compress_buff, new_buff_size + 1);
    if (!compress_buff) {
        perror("Realloc failed");
        return NULL;
    }
    strcat(compress_buff, code);
    compress_buff[new_buff_size] = '\0';
    *buff_size = new_buff_size;
    return compress_buff;
}

char *get_compress(code_t **codes, char *file_buff, int buff_len)
{
    int compress_buff_len = 0;
    char *compress_buff = NULL;

    if (!file_buff || !codes) {
        perror("Invalid NULL file_buff or codes for buff to binary conversion");
        return NULL;
    }
    compress_buff = calloc(1, 1);
    if (!compress_buff) {
        perror("Could not allocate memory for compress_buff");
        return NULL;
    }
    for (int i = 0; i < buff_len; i++) {
        compress_buff = add_bin_code_to_buff(file_buff[i], codes, compress_buff, &compress_buff_len);
        if (!compress_buff) {
            return NULL;
        }
    }
    printf("%i - %i\n", compress_buff_len, (8 - (compress_buff_len % 8)) % 8);
    return compress_buff;
}

int write_binary(char *compressed_buff, FILE *output)
{
    int bits_used = 0;
    char buff = 0;

    for (int i = 0; compressed_buff[i] != '\0'; i++) {
        buff = buff << 1 | (compressed_buff[i] - '0');
        bits_used++;

        if (bits_used == 8) {
            if (fwrite(&buff, 1, 1, output) != 1) {
                perror("Failed to write byte to file");
                return EXIT_FAILURE;
            }
            bits_used = 0;
            buff = 0;
        }
    }
    if (bits_used > 0) {
        buff = buff << (8 - bits_used);
        if (fwrite(&buff, 1, 1, output) != 1) {
            perror("Failed to write remaining byte to file");
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

int write_compress(code_t **codes, char *file_buff, int buff_len)
{
    FILE *output = NULL;
    char *compressed_buff = NULL;
    
    if (!file_buff || !codes) {
        perror("Invalid NULL file_buff or codes for buff to binary conversion");
        return EXIT_FAILURE;
    }
    output = fopen("test.lite", "wb");
    if (!output) {
        perror("Could not create a new file for the compressed data");
        return EXIT_FAILURE;
    }
    compressed_buff = get_compress(codes, file_buff, buff_len);
    if (!compressed_buff) {
        fclose(output);
        return EXIT_FAILURE;
    }
    return write_binary(compressed_buff, output);
}