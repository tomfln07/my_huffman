#ifndef WRITE_COMPRESS_H
    #define WRITE_COMPRESS_H
    #define HEM 0xFF // HEM = Header End Marker
    #define HEM_ARR_LENGHT 3 // Size of the byte sequence of the marker
    #include "binary_codes.h"

static const unsigned char HEADER_END_MARKER[HEM_ARR_LENGHT] = { HEM, HEM, HEM };

int write_compress(code_t **codes, char *file_buff, int buff_len, char_node_t **occurr_arr, int occurr_len);
#endif