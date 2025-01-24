#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include "bit_buffer.h"

bit_buffer_t *create_bit_buffer(unsigned int capacity)
{
    bit_buffer_t *bit_buff = malloc(sizeof(bit_buffer_t));

    if (!bit_buff) {
        perror("Could not allocate memory for bit buffer");
        return NULL;
    }
    bit_buff->buff = calloc(capacity, 1);
    if (!bit_buff->buff) {
        perror("Could not allocate memory for the buffer in bit_buffer");
        free(bit_buff);
        return NULL;
    }
    bit_buff->capacity = capacity;
    bit_buff->used_bits = 0;
    return bit_buff;
}

int add_bit(bit_buffer_t *bit_buff, int bit)
{
    if (!bit_buff) {
        perror("Could not add bit to NULL bit_buff");
        return EXIT_FAILURE;
    }
    if (bit != 0 && bit != 1) {
        perror("Invalid bit argument (not 1 or 0)");
        return EXIT_FAILURE;
    }
    if (bit_buff->used_bits == bit_buff->capacity * 8) {
        bit_buff->capacity *= SIZE_MULT;
        bit_buff->buff = realloc(bit_buff->buff, bit_buff->capacity);
        if (!bit_buff->buff) {
            perror("Could not resize the bit_buffer's array");
            return EXIT_FAILURE;
        }
    }
    int byte_i = bit_buff->used_bits / 8;
    int bit_i = 7 - (bit_buff->used_bits % 8);
    if (bit == 1) {
        bit_buff->buff[byte_i] |= (1 << bit_i);
    } else {
        bit_buff->buff[byte_i] &= ~(1 << bit_i);
    }
    bit_buff->used_bits++;
    return EXIT_SUCCESS;
}

void free_bit_buff(bit_buffer_t *bit_buff)
{
    if (bit_buff) {
        if (bit_buff->buff) {
            free(bit_buff->buff);
        }
        free(bit_buff);
    }
}
