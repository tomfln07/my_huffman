#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "flags.h"

flags_t *get_flags(int ac, char **av, int *last_flag_i)
{
    flags_t *flags = calloc(1, sizeof(flags_t));

    if (!flags) {
        perror("Could not alocate memory for flags");
        return NULL;
    }
    for (int i = 1; i < ac; i++) {
        if (strcmp(av[i], "-d") == 0) {
            flags->uncompress = true;
        } else {
            *last_flag_i = i - 1;
            return flags;
        }   
        (*last_flag_i)++;
    }
    return flags;
}