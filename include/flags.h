#ifndef FLAGS_H
    #define FLAGS_H
    #include "stdbool.h"
    #include "huffman_tree.h"

typedef struct flags_s
{
    bool uncompress; // Will compress if false
} flags_t;

flags_t *get_flags(int ac, char **av, int *last_flag_i);
#endif