#include <stdio.h>
#include <stdlib.h>

#include "util.h"

char* read_file(const char* file_path)
{
    FILE* file;
    if ((file = fopen(file_path, "r")) == NULL)
    {
        perror("Error reading file");
        return false;
    }

    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file); 
    fseek(file, 0, SEEK_SET);

    char* file_contents = (char *)malloc(file_size + 1);

    if (file_contents == NULL)
    {
        perror("Error allocating memory");
        fclose(file);
        return false;
    }

    size_t bytes_read = fread(file_contents, 1, file_size, file);
    (file_contents)[bytes_read] = '\0';

    fclose(file);
    return file_contents;
}
