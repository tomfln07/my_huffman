#ifndef BIT_BUFFER_H
    #define BIT_BUFFER_H
    #define SIZE_MULT 2 // When the buffer is full, by how much should the list's size be realloc
    #include "huffman_tree.h"

/**
 * Represents the compressed content of a file
 */
typedef struct bit_buffer_s
{
    unsigned char *buff;
    unsigned int used_bits;
    unsigned int capacity; // in bytes
} bit_buffer_t;

bit_buffer_t *create_bit_buffer(unsigned int capacity);
int add_bit(bit_buffer_t *bit_buff, int bit);
void free_bit_buff(bit_buffer_t *bit_buff);
#endif