#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

void get_wikipedia_dump_directory(char *full_path, size_t size);
void create_output_directory(const char *path);

#endif