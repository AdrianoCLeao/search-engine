#if defined(_WIN32)
    #include <windows.h>
    #include <direct.h>
    #define getcwd _getcwd
#else       
    #include <dirent.h>
    #include <sys/types.h>
    #include <unistd.h>
#endif


#include "../include/utils/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void list_files_in_directory(const char *path, char ***file_list, int *file_count) {
    int count = 0;
    char **files = NULL;

#if defined(_WIN32) || defined(_WIN64) 
    WIN32_FIND_DATA findFileData;
    HANDLE hFind;

    char search_path[MAX_PATH];
    snprintf(search_path, sizeof(search_path), "%s\\*", path);

    hFind = FindFirstFile(search_path, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        perror("Erro ao abrir diretório");
        exit(1);
    }

    do {
        if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            files = realloc(files, (count + 1) * sizeof(char *));
            files[count] = malloc(strlen(path) + strlen(findFileData.cFileName) + 2);
            snprintf(files[count], MAX_PATH, "%s\\%s", path, findFileData.cFileName);
            count++;
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);

#else 
    DIR *dir = opendir(path);
    struct dirent *entry;

    if (!dir) {
        perror("Erro ao abrir diretório");
        exit(1);
    }

    while ((entry = readdir(dir))) {
        if (entry->d_type == DT_REG) {
            files = realloc(files, (count + 1) * sizeof(char *));
            files[count] = malloc(strlen(path) + strlen(entry->d_name) + 2);
            snprintf(files[count], PATH_MAX, "%s/%s", path, entry->d_name);
            count++;
        }
    }

    closedir(dir);
#endif

    *file_list = files;
    *file_count = count;
}

char *read_file_content(const char *filepath) {
    FILE *file = fopen(filepath, "r");
    if (!file) {
        perror("Erro ao abrir arquivo");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *content = malloc(size + 1);
    fread(content, 1, size, file);
    content[size] = '\0';

    fclose(file);
    return content;
}

void clear_screen() {
#if defined(_WIN32) || defined(_WIN64)
    system("cls");  
#else
    system("clear"); 
#endif
}

void create_directory(const char *path) {
    #if defined(_WIN32) || defined(_WIN64) 
        _mkdir(path); 
    #else
        mkdir(path, 0777); 
    #endif
}

void print_current_working_directory() {
    char cwd[512]; 
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Diretório de trabalho atual: %s\n", cwd);
    } else {
        perror("Erro ao obter o diretório de trabalho atual");
    }
}
