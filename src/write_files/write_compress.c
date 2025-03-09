#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "write_compress.h"
#include "occurrences.h"
#include "bit_buffer.h"
#include "file_name.h"

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

bit_buffer_t *get_compress(code_t **codes, unsigned char *file_buff, int buff_len)
{
    bit_buffer_t *bit_buff = create_bit_buffer(128);
    char *code = NULL;
    int code_len = 0;

    if (!file_buff || !codes) {
        perror("Invalid NULL file_buff or codes for buff to binary conversion");
        return NULL;
    }
    if (!bit_buff) {
        return NULL;
    }
    for (int i = 0; i < buff_len; i++) {
        for (int j = 0; codes[j]; j++) {
            if (file_buff[i] == codes[j]->c) {
                code = codes[j]->code;
                break;
            }
        }
        code_len = strlen(code);
        for (int j = 0; j < code_len; j++) {
			if (add_bit(bit_buff, code[j] - '0') == EXIT_FAILURE) {
				free_bit_buff(bit_buff);
				return NULL;
			}
        }
    }
    return bit_buff;
}

int write_binary(bit_buffer_t *compressed_buff, FILE *output)
{
    unsigned int bytes_wrote = 0;

    if (!compressed_buff || !output) {
        if (!compressed_buff) {
            printf("Invaid NULL bit_buffer");
        } else {
            free_bit_buff(compressed_buff);
        }
        if (!output) {
            printf("Invaid NULL output file");
        } else {
            fclose(output);
        }
        return EXIT_FAILURE;
    }
    bytes_wrote = fwrite(compressed_buff->buff, 1, compressed_buff->used_bits / 8, output);
    if (compressed_buff->used_bits / 8.0 > 0) {
        bytes_wrote += fwrite(compressed_buff->buff + compressed_buff->used_bits / 8, 1, 1, output);
    }
    if ((compressed_buff->used_bits / 8.0 > 0 && bytes_wrote != compressed_buff->used_bits / 8 + 1)
        || (compressed_buff->used_bits / 8.0 < 0 && bytes_wrote != compressed_buff->used_bits / 8)) {
        perror("Could not write the entirety of the compressed data");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
/**
 * Header: <huffman compressed file signature><nbr of bits><nbr of chars>[<char><occurrence>...]
 */
int write_header(char_node_t **occurr, int nbr_occurr, bit_buffer_t *compressed_buff, FILE *output)
{
	uint32_t magic = FILE_SIG;

    fwrite(&magic, sizeof(unsigned int), 1, output);
    fwrite(&compressed_buff->used_bits, sizeof(unsigned int), 1, output);
    return dump_occurr_arr(occurr,  nbr_occurr, output);
}

int write_bit_buff(
	bit_buffer_t *compressed_buff,
	char_node_t **occurr_arr, int occurr_len,
	char *output_file_name)
{
    FILE *output = NULL;
	int write_status = EXIT_SUCCESS;
	
	if (!compressed_buff || !occurr_arr 
		|| occurr_len < 0 || !output_file_name) {
		return EXIT_FAILURE;
	}
	output = fopen(output_file_name, "wb");
    if (!output) {
        perror("Could not create a new file for the compressed data");
        return EXIT_FAILURE;
    }
    if (write_header(
			occurr_arr, occurr_len,
			compressed_buff, output) == EXIT_FAILURE) {
        fclose(output);
		return EXIT_FAILURE;
    }
	write_status = write_binary(compressed_buff, output);
	fclose(output);
	return write_status;
}

int write_compress(
    code_t **codes,
    unsigned char *file_buff, int buff_len,
    char_node_t **occurr_arr, int occurr_len,
	char *input_file_name)
{
    bit_buffer_t *compressed_buff = NULL;
	int write_status = EXIT_SUCCESS;
	int output_file_len = input_file_name ? strlen(input_file_name) + strlen(EXT) : 0;
	char output_file_name[output_file_len];

    if (!file_buff || !codes) {
        perror("Invalid NULL file_buff or codes for buff to binary conversion");
        return EXIT_FAILURE;
    }
	if (add_compression_ext(input_file_name, output_file_name) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}
    compressed_buff = get_compress(codes, file_buff, buff_len);
    if (!compressed_buff) {
        return EXIT_FAILURE;
    }
	write_status = write_bit_buff(
		compressed_buff,
		occurr_arr, occurr_len,
		output_file_name);
	free_bit_buff(compressed_buff);
    return write_status;
}
