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

#include "../include/tf-idf/tokenizer.h"
#include "../include/utils/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void tokenize_to_file(const char *content, const char *output_file) {
    const char *base_dir = "../data/tokens";
    create_directory(base_dir);

    char full_path[512];
    snprintf(full_path, sizeof(full_path), "%s/%s", base_dir, output_file);

    const char *delimiters = " \t\n\r,.!?;:\"()[]{}";
    char *copy = strdup(content);
    if (!copy) {
        printf("Erro ao alocar memória para cópia do conteúdo\n");
        exit(1);
    }

    FILE *file = fopen(full_path, "w");
    if (!file) {
        printf("Erro ao abrir arquivo para escrita: %s\n", full_path);
        free(copy);
        exit(1);
    }

    char *token = strtok(copy, delimiters);
    while (token != NULL) {
        fprintf(file, "%s\n", token);
        token = strtok(NULL, delimiters);
    }

    fclose(file);
    free(copy);

    printf("Tokens salvos em: %s\n", full_path);
}

int term_exists(const char *term, TermData terms[], int term_count) {
    for (int i = 0; i < term_count; i++) {
        if (strcmp(terms[i].term, term) == 0) {
            return i;
        }
    }
    return -1;
}
