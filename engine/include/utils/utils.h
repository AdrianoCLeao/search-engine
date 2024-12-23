#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Lists all files in a directory.
 * 
 * This function traverses the specified directory and collects the names of files
 * into a dynamically allocated array of strings.
 * 
 * @param path Path of the directory to be listed.
 * @param file_list Pointer to an array of strings to store file names.
 * @param file_count Pointer to an integer to store the number of files found.
 */
void list_files_in_directory(const char *path, char ***file_list, int *file_count);

/**
 * @brief Reads the content of a file.
 * 
 * This function reads the content of the specified file and returns it as a dynamically
 * allocated string. The caller is responsible for freeing the allocated memory.
 * 
 * @param filepath Path to the file to be read.
 * @return Pointer to a string containing the file's content. Returns NULL on failure.
 */
char *read_file_content(const char *filepath);

/**
 * @brief Clears the terminal screen.
 * 
 * Clears the terminal screen based on the operating system.
 */
void clear_screen();

/**
 * @brief Creates a directory.
 * 
 * Creates a directory at the specified path. On Unix-like systems, permissions are set to 0777.
 * 
 * @param path Path of the directory to be created.
 */
void create_directory(const char *path);

/**
 * @brief Prints the current working directory.
 * 
 * Prints the absolute path of the current working directory to the standard output.
 */
void print_current_working_directory();

#endif
