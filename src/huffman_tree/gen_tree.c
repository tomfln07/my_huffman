#include <stdlib.h>
#include <stdio.h>
#include "occurrences.h"

/**
 * Takes the two less frequent chars, merge them into an INTERNAL node,
 * and del them from the occurr array.
 * Meant to be called iterativelly to create the huffman tree.
 * @return Returns if the node creation what successful
 */
int node_creation_it(char_node_t **occurr, int occurr_len)
{
    int less_freq_i1 = -1;
    int less_freq_i2 = -1;
    char_node_t *new_internal = NULL;

    for (int i = 0; i < occurr_len; i++) {
        if (!occurr[i]) {
            continue;
        }
        if (less_freq_i1 == -1 || occurr[i]->freq < occurr[less_freq_i1]->freq) {
            less_freq_i2 = less_freq_i1;
            less_freq_i1 = i;
        } else if (less_freq_i2 == -1 || occurr[i]->freq < occurr[less_freq_i2]->freq) {
            less_freq_i2 = i;
        }
    }
    if (less_freq_i1 == -1 || less_freq_i2 == -1) {
        fprintf(stderr, "Not enough nodes to create a new internal node\n");
        return EXIT_FAILURE;
    }
    new_internal = new_internal_node(occurr[less_freq_i1], occurr[less_freq_i2]);
    if (!new_internal) {
        return EXIT_FAILURE;
    }
    occurr[less_freq_i1] = new_internal;
    occurr[less_freq_i2] = NULL;
    return EXIT_SUCCESS;
}

char_node_t *gen_tree(char_node_t **occurr)
{
    int internal_nodes_ceated = 0;
    int occurr_len = -1;
    char_node_t **prio_queue = NULL;
    char_node_t *root_node = NULL;

    if (!occurr) {
        perror("Passed NULL chars occurrence array");
        return NULL;
    }
    occurr_len = get_occurr_len(occurr);
    if (occurr_cpy(occurr, &prio_queue, occurr_len)) {
        return NULL;
    }
    while (internal_nodes_ceated < occurr_len - 1) {
        if (node_creation_it(prio_queue, occurr_len) == EXIT_FAILURE) {
            free_nodes_list(prio_queue, occurr_len);
            return NULL;
        }
        internal_nodes_ceated++;
    }
    for (int i = 0; i < occurr_len; i++) {
        if (prio_queue[i]) {
            root_node = prio_queue[i];
            free(prio_queue);
            return root_node;
        }
    }
    return NULL;
}