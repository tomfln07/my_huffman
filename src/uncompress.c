#include <stdio.h>
#include <stdlib.h>
#include "occurrences.h"

int uncompress(char *filepath)
{
    char_node_t **occurr_arr = read_header_occurrs(filepath);

    if (!occurr_arr) {
        return EXIT_FAILURE;
    }
    printf("Extracted:\n");
    for (int i = 0; occurr_arr[i]; i++) {
        printf("%c: %i\n", occurr_arr[i]->c, occurr_arr[i]->freq);
    }
    return EXIT_SUCCESS;
}