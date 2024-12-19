#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>

void list_files_in_directory(const char *path, char ***file_list, int *file_count);
char *read_file_content(const char *filepath);
void clear_screen();
void create_directory(const char *path);

#endif