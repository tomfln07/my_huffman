#include <stdio.h>
#include <stdlib.h>
#include "flags.h"
#include "compress.h"
#include "uncompress.h"

int main(int ac, char **av)
{
    int last_flag_i = 0;
    flags_t *flags = get_flags(ac, av, &last_flag_i);
    
    if (ac == 1 || last_flag_i + 1 == ac) {
        printf("usage: ./huffman [-d] file_path\n\t-d:\tDecompress (compress by default)\n");
        return EXIT_SUCCESS;
    }
    if (!flags->uncompress) {
        return compress(av[last_flag_i + 1]);
    }
    return uncompress(av[last_flag_i + 1]);
}
