#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "occurrences.h"

/**
 * Find the index of a char in the occurr arr
 * @return Returns -1 if it's not in the occurr arr
 */
int get_char_index(char_node_t **occurr, int arr_len, unsigned char c)
{
    for (int i = 0; i < arr_len; i++) {
        if (c == occurr[i]->c) {
            return i;
        }
    }
    return -1;
}

/**
 * Reads the occurrences array in the header of a compressed file
 */
char_node_t **read_header_occurrs(FILE *input)
{
    int nbr_nodes = 0;
    char_node_t **occurr = NULL;

    if (fread(&nbr_nodes, sizeof(int), 1, input) != 1) {
        perror("Could not read the number of different chars in compressed file's header");
        return NULL;
    }
    occurr = malloc((nbr_nodes + 1) * sizeof(char_node_t *));
    for (int i = 0; i < nbr_nodes; i++) {
        occurr[i] = malloc(sizeof(char_node_t));
        fread(&occurr[i]->c, sizeof(unsigned char), 1, input);
        fread(&occurr[i]->freq, sizeof(int), 1, input);
        occurr[i]->l = NULL;
        occurr[i]->r = NULL;
        occurr[i]->type = LEAF;
    }
    occurr[nbr_nodes] = NULL;
    return occurr;
}

char_node_t **get_occurr(unsigned char *file_buff, long file_len)
{
    int occurr_len = 0;
    char_node_t **occurr = calloc(1, sizeof(char_node_t *));
    int char_i = -1;
    long total = 0;

    if (!occurr) {
        perror("Could not allocate for occurrence array");
        return NULL;
    }
    occurr[0] = NULL;
    for (int i = 0; i < file_len; i++) {
        total++;
        char_i = get_char_index(occurr, occurr_len, file_buff[i]);

        if (char_i != -1) {
            occurr[char_i]->freq++;
            continue;
        }
        occurr = add_new_char(occurr, occurr_len, file_buff[i]);
        if (occurr == NULL) {
            return NULL;
        }
        occurr_len++;
    }
    return occurr;
}