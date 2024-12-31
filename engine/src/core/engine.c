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

#include "../include/core/engine.h"
#include "../include/utils/utils.h"
#include "../include/json/json.h"
#include "../include/sort/quick_sort.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void tfidf_initialize(TFIDFEngine *engine) {
    engine->documents = NULL;
    engine->num_documents = 0;
}

void tfidf_load_documents(TFIDFEngine *engine, const char *directory) {
    int i;
    char **file_list;
    int file_count = 0;

    list_files_in_directory(directory, &file_list, &file_count);
    engine->documents = malloc(file_count * sizeof(char *));
    if (!engine->documents) {
        perror("Erro ao alocar memória para documentos");
        exit(1);
    }

    for (i = 0; i < file_count; i++) {
        engine->documents[i] = read_file_content(file_list[i]);
        free(file_list[i]);
    }

    free(file_list);
    engine->num_documents = file_count;
    printf("Carregados %d documentos\n", file_count);
}

void tfidf_calculate(TFIDFEngine *engine) {
    printf("Iniciando o calculo de TF-IDF...\n");
}

void tfidf_search(TFIDFEngine *engine, const char *query) {
    printf("Iniciando busca para a query: %s\n", query);
}

void tfidf_free(TFIDFEngine *engine) {
    int i;
    for (i = 0; i < engine->num_documents; i++) {
        free(engine->documents[i]);
    }
    free(engine->documents);
}
