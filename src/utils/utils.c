#include "../include/utils/utils.h"
#if defined(_WIN32) || defined(_WIN64)
    #include <direct.h> 
#else
    #include <unistd.h> 
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_PATH_LENGTH 1024

void get_wikipedia_dump_directory(char *full_path, size_t size) {
    char cwd[MAX_PATH_LENGTH];

#if defined(_WIN32) || defined(_WIN64)
    if (_getcwd(cwd, sizeof(cwd)) != NULL) {
        snprintf(full_path, size, "%s\\data\\wikipedia_dump.xml", cwd);
    } else {
        perror("Erro ao obter o diretório atual (Windows)");
        exit(1);
    }
#else
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        snprintf(full_path, size, "%s/data/wikipedia_dump.xml", cwd);
    } else {
        perror("Erro ao obter o diretório atual (POSIX)");
        exit(1);
    }
#endif
}

void create_output_directory(const char *path) {
#if defined(_WIN32) || defined(_WIN64)
    _mkdir(path);
#else
    mkdir(path, 0777);
#endif
}