#include <stdio.h>
#include <stdlib.h>

long get_file_len(FILE *file)
{
    long file_len = 0;

    fseek(file, 0, SEEK_END);
    file_len = ftell(file);
    fseek(file, 0, SEEK_SET);

    return file_len;
}

unsigned char *readfile(char *path, long *len)
{
    unsigned char *file_content = NULL;
    FILE *file = fopen(path, "rb");
    long file_len = 0;
    long bytes_read = 0;

    if (file == NULL) {
        printf("%s\n", path);
        perror("Could not open file to compress");
        return NULL;
    }
    file_len = get_file_len(file);
    file_content = calloc(1, file_len + 1);
    if (!file_content) {
        perror("Could not calloc for file buffer");
        fclose(file);
        return NULL;
    }
    bytes_read = fread(file_content, 1, file_len, file);
    if (bytes_read != file_len) {
        perror("Could not read all the content of the file to compress");
        fclose(file);
        free(file_content);
        return NULL;
    }
    fclose(file);
    *len = file_len;
    return file_content;
}
