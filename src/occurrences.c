#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "occurrences.h"

void free_occurr_arr(char_node_t **occurr)
{
    if (!occurr) {
        return;
    }
    for (int i = 0; occurr[i]; i++) {
        free(occurr[i]);
    }
    free(occurr);
}

int get_occurr_len(char_node_t **occurr)
{
    static int occurr_len = 0;
    static int has_been_called = 0;

    if (has_been_called) {
        return occurr_len;
    }
    while (occurr[occurr_len]) {
        occurr_len++;
    }
    has_been_called = 1;
    return occurr_len;
}

/**
 * @return Returns the new ptr to the arr, -1 if the realloc failed
 */
char_node_t **realloc_occur_arr(char_node_t **occurr, int curr_len, int new_len)
{
    char_node_t **new_arr = realloc(occurr, (new_len + 1) * sizeof(char_node_t *));

    if (!new_arr) {
        perror("Could not realloc occurr arr");
        return NULL;
    }
    for (int i = curr_len; i < new_len + 1; i++) {
        new_arr[i] = NULL;
    }
    return new_arr;
}

char_node_t **add_new_char(char_node_t **occurr, int arr_len, char c)
{
    char_node_t **new_arr = realloc_occur_arr(occurr, arr_len, arr_len + 1);

    if (!new_arr) {
        return NULL;
    }
    new_arr[arr_len] = malloc(sizeof(char_node_t));
    if (!new_arr[arr_len]) {
        free_occurr_arr(new_arr);
        return NULL;
    }
    new_arr[arr_len]->c = c;
    new_arr[arr_len]->freq = 1;
    new_arr[arr_len]->l = NULL;
    new_arr[arr_len]->r = NULL;
    new_arr[arr_len]->type = LEAF;
    return new_arr;
}

/**
 * Find the index of a char in the occurr arr
 * @return Returns -1 if it's not in the occurr arr
 */
int get_char_index(char_node_t **occurr, int arr_len, char c)
{
    for (int i = 0; i < arr_len; i++) {
        if (c == occurr[i]->c) {
            return i;
        }
    }
    return -1;
}

char_node_t **get_occurr(char *file_buff, long file_len)
{
    int occurr_len = 0;
    char_node_t **occurr = calloc(1, sizeof(char_node_t *));
    int char_i = -1;

    if (!occurr) {
        perror("Could not allocate for occurrence array");
        return NULL;
    }
    occurr[0] = NULL;
    for (int i = 0; i < file_len; i++) {
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