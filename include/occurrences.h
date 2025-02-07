#ifndef GET_OCCURR_H
    #define GET_OCCURR_H
    #include <stdio.h>
    #include "huffman_tree.h"

char_node_t **get_occurr(unsigned char *file_buff, long file_len);
int get_occurr_len(char_node_t **occurr);
void free_occurr_arr(char_node_t **occurr);
int dump_occurr_arr(char_node_t **occurr, int nbr_occurr, FILE *output);
char_node_t **read_header_occurrs(FILE *input);
char_node_t **add_new_char(char_node_t **occurr, int arr_len, unsigned char c); 
#endif