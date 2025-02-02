// Manipulation of the occurrence array

#include <stdlib.h>
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
    int occurr_len = 0;
    //int has_been_called = 0;

    /*if (has_been_called) {
        return occurr_len;
    }*/
    if (!occurr) {
        perror("Cannot get invalid NULL occurr lenght");
        return -1;
    }
    while (occurr[occurr_len]) {
        occurr_len++;
    }
    //has_been_called = 1;
    return occurr_len;
}

/**
 * @return Returns the new ptr to the arr, -1 if the realloc failed
 */
char_node_t **resize_occur_arr(char_node_t **occurr, int curr_len, int new_len)
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

char_node_t **add_new_char(char_node_t **occurr, int arr_len, unsigned char c)
{
    char_node_t **new_arr = resize_occur_arr(occurr, arr_len, arr_len + 1);

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
 * Put the occurrences array into the specified file
 */
int dump_occurr_arr(char_node_t **occurr, int nbr_occurr, FILE *output)
{
    if (!output || !occurr) {
        return EXIT_FAILURE;
    }
    fwrite(&nbr_occurr, sizeof(int), 1, output);
    for (int i = 0; i < nbr_occurr; i++) {
        fwrite(&occurr[i]->c, sizeof(char), 1, output);
        fwrite(&occurr[i]->freq, sizeof(int), 1, output);
    }
    return EXIT_SUCCESS;
}
