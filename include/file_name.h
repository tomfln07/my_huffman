#ifndef FILE_NAME_H
    #define FILE_NAME_H
	#define EXT ".lite" // Compressed file extension

int add_compression_ext(char *file_name, char *new_file_name);
int remove_compression_ext(char *file_name, char *new_file_name);
#endif
