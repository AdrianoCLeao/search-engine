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

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Lists all files in a directory.
 * 
 * Traverses the specified directory and collects the names of files
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
 * Reads the content of the specified file and returns it as a dynamically
 * allocated string. The caller is responsible for freeing the allocated memory.
 * 
 * @param filepath Path to the file to read.
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
 * @param path Path of the directory to create.
 */
void create_directory(const char *path);

/**
 * @brief Prints the current working directory.
 * 
 * Prints the absolute path of the current working directory to the standard output.
 */
void print_current_working_directory();

/**
 * @brief Extracts and normalizes the base name of a file.
 * 
 * Converts underscores to spaces and capitalizes the first letter of each word.
 * 
 * @param file_path The path to the file.
 * @return A dynamically allocated string containing the base name.
 */
char *get_file_basename(const char *file_path);

/**
 * @brief Normalizes a summary string.
 * 
 * Replaces invalid characters, trims spaces, and handles line breaks.
 * 
 * @param summary The string to normalize.
 */
void normalize_summary(char *summary);

/**
 * @brief Appends a character to a string if there is space remaining.
 * 
 * @param dest Destination string.
 * @param ch Character to append.
 * @param remLen Remaining length of the buffer.
 * @return Pointer to the updated destination string.
 */
char* chtoa(char* dest, char ch, size_t* remLen);

/**
 * @brief Appends a source string to a destination string if there is space remaining.
 * 
 * @param dest Destination string.
 * @param src Source string.
 * @param remLen Remaining length of the buffer.
 * @return Pointer to the updated destination string.
 */
char* atoa(char* dest, const char* src, size_t* remLen);

#endif
