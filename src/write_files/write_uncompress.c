#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "binary_codes.h"
#include "file_name.h"

int find_code_i(char *code, code_t **codes)
{
    for (int i = 0; codes[i]; i++) {
        if (strcmp(code, codes[i]->code) == 0) {
            return i;
        }
    }
    return -1;
}

int write_uncompressed(
    code_t **codes,
    unsigned char *compressed_bytes, int nbr_bytes,
    int max_code_len, int bits_to_read,
	char *input_file_name)
{
    char code[max_code_len];
    int code_i = -1;
    unsigned char bit = 0;
    char bit_str[2] = "0";
    FILE *fd = NULL;
    int bits_read = 0;
	int output_file_len = input_file_name ? strlen(input_file_name) : 0;
	char output_file_name[output_file_len];

	if (!compressed_bytes || nbr_bytes < 0
		|| max_code_len < 0 || bits_to_read < 0
		|| !input_file_name) {
		return EXIT_FAILURE;
	}
	if (remove_compression_ext(input_file_name, output_file_name) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}
	fd = fopen(output_file_name, "wb");
    if (!fd) {
        perror("Could not create uncompressed file");
        return EXIT_FAILURE;
    }
    memset(code, 0, max_code_len);        
    for (int i = 0; i < nbr_bytes; i++) {
        for (int j = 7; j >= 0; j--) {
            bit = (compressed_bytes[i] >> j) & 1;
            bit_str[0] = bit + '0';
            
            strcat(code, bit_str);
            code_i = find_code_i(code, codes);
            if (code_i != -1) {
                memset(code, 0, max_code_len);
                fwrite(&codes[code_i]->c, sizeof(char), 1, fd);
            }
            bits_read++;
            if (bits_read == bits_to_read) {
                return EXIT_SUCCESS;
            }
        }
    }
    return EXIT_SUCCESS;
}