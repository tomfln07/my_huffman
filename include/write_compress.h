#ifndef WRITE_COMPRESS_H
    #define WRITE_COMPRESS_H
    #include "binary_codes.h"

int write_compress(code_t **codes, unsigned char *file_buff, int buff_len, char_node_t **occurr_arr, int occurr_len);
#endif