#include <stdlib.h>
#include <string.h>
#include "file_name.h"

char *get_last_match(char *haystack, char *needle)
{
    char *last_occurrence = NULL;
    char *current_occur = haystack;
    int needle_len = 0;

	if (!haystack || !needle) {
		return NULL;
	}
	needle_len = strlen(needle);
    if (needle_len == 0) {
        return haystack + strlen(haystack);
    }
	current_occur = strstr(current_occur, needle);
    while (current_occur != NULL) {
        last_occurrence = current_occur;
        current_occur += needle_len;
		current_occur = strstr(current_occur, needle);
    }
    return last_occurrence;
}

/**
 * Add the compression file extention
 */
int add_compression_ext(char *file_name, char *new_file_name)
{
	char *ext_ptr = NULL;

	if (!file_name || !new_file_name) {
		return EXIT_FAILURE;
	}	
	strcpy(new_file_name, file_name);
	ext_ptr = get_last_match(file_name, EXT);
	if (ext_ptr && ext_ptr[strlen(EXT)] == '\0') {
		return EXIT_SUCCESS;
	}
	strcat(new_file_name, EXT);
	return EXIT_SUCCESS;
}

int remove_compression_ext(char *file_name, char *new_file_name)
{
	char *ext_ptr = NULL;

	if (!file_name || !new_file_name) {
		return EXIT_FAILURE;
	}
	ext_ptr = get_last_match(file_name, EXT);
	strcpy(new_file_name, file_name);
	if (!ext_ptr || ext_ptr[strlen(EXT)] != '\0') {
		return EXIT_SUCCESS;
	}
	new_file_name[ext_ptr - file_name] = '\0';
	return EXIT_SUCCESS;
}
