#include <stdio.h>
#include <stdlib.h>
#include "flags.h"
#include "compress.h"
#include "uncompress.h"

int main(int ac, char **av)
{
    int last_flag_i = 0;
    flags_t *flags = get_flags(ac, av, &last_flag_i);
    int status_code = 0;
    
    if (ac == 1 || last_flag_i + 1 == ac) {
        printf("usage: ./huffman [-d] file_path\n\t-d:\tDecompress (compress by default)\n");
        free(flags);
        return EXIT_SUCCESS;
    }
    if (!flags->uncompress) {
        status_code = compress(av[last_flag_i + 1]);
    } else {
        status_code = uncompress(av[last_flag_i + 1]);
    }
    free(flags);
    return status_code;
}
