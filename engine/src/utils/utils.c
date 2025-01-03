/*
 * This file is part of TFIDF Search Engine.
 *
 * Copyright (C) 2024 Adriano Leão
 *
 * TFIDF Search Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * TFIDF Search Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with TFIDF Search Engine. If not, see <https://www.gnu.org/licenses/>.
 */

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
#include <ctype.h>

/**
 * @brief Lists all files in a directory.
 * 
 * Traverses the specified directory and collects the names of files
 * into a dynamically allocated array of strings. Works on both Windows and Unix-like systems.
 * 
 * @param path Path of the directory to list.
 * @param file_list Pointer to an array of strings to store file names.
 * @param file_count Pointer to an integer to store the number of files found.
 */
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
        perror("Error opening directory");
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
        perror("Error opening directory");
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

/**
 * @brief Reads the content of a file.
 * 
 * Reads the content of the specified file and returns it as a dynamically
 * allocated string. The caller is responsible for freeing the allocated memory.
 * 
 * @param filepath Path to the file to read.
 * @return Pointer to a string containing the file's content. Returns NULL on failure.
 */
char *read_file_content(const char *filepath) {
    FILE *file = fopen(filepath, "r");
    if (!file) {
        perror("Error opening file");
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

/**
 * @brief Clears the terminal screen.
 * 
 * Clears the terminal screen based on the operating system.
 */
void clear_screen() {
#if defined(_WIN32) || defined(_WIN64)
    system("cls");  
#else
    system("clear"); 
#endif
}

/**
 * @brief Creates a directory.
 * 
 * Creates a directory at the specified path. On Unix-like systems, permissions are set to 0777.
 * 
 * @param path Path of the directory to create.
 */
void create_directory(const char *path) {
    #if defined(_WIN32) || defined(_WIN64) 
        _mkdir(path); 
    #else
        mkdir(path, 0777); 
    #endif
}

/**
 * @brief Prints the current working directory.
 * 
 * Prints the absolute path of the current working directory to the standard output.
 */
void print_current_working_directory() {
    char cwd[512]; 
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working directory: %s\n", cwd);
    } else {
        perror("Error getting the current working directory");
    }
}

/**
 * @brief Extracts and normalizes the base name of a file.
 * 
 * Converts underscores to spaces and capitalizes the first letter of each word.
 * 
 * @param file_path The path to the file.
 * @return A dynamically allocated string containing the base name.
 */
char *get_file_basename(const char *file_path) {
    const char *base_name;

#ifdef _WIN32
    base_name = strrchr(file_path, '\\');
#else
    base_name = strrchr(file_path, '/');
#endif

    base_name = (base_name) ? base_name + 1 : file_path;

    char *result = strdup(base_name);
    if (!result) {
        perror("Error allocating memory for file name");
        exit(1);
    }

    char *dot = strrchr(result, '.');
    if (dot) {
        *dot = '\0';
    }

    int capitalize_next = 1;
    for (char *ptr = result; *ptr; ++ptr) {
        if (*ptr == '_') {
            *ptr = ' ';
            capitalize_next = 1;
        } else if (capitalize_next && isalpha((unsigned char)*ptr)) {
            *ptr = toupper((unsigned char)*ptr);
            capitalize_next = 0;
        } else {
            *ptr = tolower((unsigned char)*ptr);
        }
    }

    return result;
}

/**
 * @brief Normalizes a summary string.
 * 
 * Replaces invalid characters, trims spaces, and handles line breaks.
 * 
 * @param summary The string to normalize.
 */
void normalize_summary(char *summary) {
    char *src = summary;
    char *dest = summary;

    while (*src) {
        if (isalnum((unsigned char)*src) || ispunct((unsigned char)*src) || *src == ' ') {
            if (*src == '\n' || *src == '\r') {
                *dest++ = ' '; 
            } else if (*src == '"') {
                *dest++ = '\''; 
            } else if (*src == '\\') {
                *dest++ = '/'; 
            } else {
                *dest++ = *src; 
            }
        }
        src++;
    }
    *dest = '\0';

    char *start = summary;
    while (*start == ' ') start++;

    char *end = start + strlen(start) - 1;
    while (end > start && *end == ' ') end--;

    *(end + 1) = '\0'; 

    memmove(summary, start, strlen(start) + 1);
}

/**
 * @brief Appends a character to a string if there is space remaining.
 * 
 * @param dest Destination string.
 * @param ch Character to append.
 * @param remLen Remaining length of the buffer.
 * @return Pointer to the updated destination string.
 */
char* chtoa(char* dest, char ch, size_t* remLen) {
    if (*remLen > 0) {
        *dest++ = ch;
        *dest = '\0';
        (*remLen)--;
    }
    return dest;
}

/**
 * @brief Appends a source string to a destination string if there is space remaining.
 * 
 * @param dest Destination string.
 * @param src Source string.
 * @param remLen Remaining length of the buffer.
 * @return Pointer to the updated destination string.
 */
char* atoa(char* dest, const char* src, size_t* remLen) {
    while (*src && *remLen > 0) {
        *dest++ = *src++;
        (*remLen)--;
    }
    *dest = '\0';
    return dest;
}
