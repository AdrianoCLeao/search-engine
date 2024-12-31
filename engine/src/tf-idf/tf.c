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

#include "../include/tf-idf/tf.h"
#include "../include/utils/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void calculate_tf_from_file(const char *token_file, int doc_index) {
    const char *base_dir = "../data/tokens";
    char full_path[512];
    const char *base_dir_tf = "../data/tf";
    create_directory(base_dir_tf);

    char tf_output_file[512];
    snprintf(full_path, sizeof(full_path), "%s/%s", base_dir, token_file);
    snprintf(tf_output_file, sizeof(tf_output_file), "%s/tf_doc_%d.txt", base_dir_tf, doc_index + 1);

    FILE *input_file = fopen(full_path, "r");
    if (!input_file) {
        printf("Erro ao abrir arquivo de tokens: %s\n", token_file);
        exit(1);
    }

    FILE *output_file = fopen(tf_output_file, "w");
    if (!output_file) {
        printf("Erro ao abrir arquivo para salvar TF: %s\n", tf_output_file);
        fclose(input_file);
        exit(1);
    }

    char buffer[256];
    int total_tokens = 0;
    int term_count = 0;
    char **terms = NULL;
    int *term_frequencies = NULL;

    while (fgets(buffer, sizeof(buffer), input_file)) {
        buffer[strcspn(buffer, "\n")] = '\0'; 
        total_tokens++;

        int found = 0;
        for (int i = 0; i < term_count; i++) {
            if (strcmp(terms[i], buffer) == 0) {
                term_frequencies[i]++;
                found = 1;
                break;
            }
        }

        if (!found) {
            terms = realloc(terms, (term_count + 1) * sizeof(char *));
            term_frequencies = realloc(term_frequencies, (term_count + 1) * sizeof(int));
            if (!terms || !term_frequencies) {
                printf("Erro ao alocar memória para termos\n");
                fclose(input_file);
                fclose(output_file);
                exit(1);
            }

            terms[term_count] = strdup(buffer);
            if (!terms[term_count]) {
                printf("Erro ao alocar memória para termo\n");
                fclose(input_file);
                fclose(output_file);
                exit(1);
            }

            term_frequencies[term_count] = 1;
            term_count++;
        }
    }

    for (int i = 0; i < term_count; i++) {
        double tf = (double)term_frequencies[i] / total_tokens;
        fprintf(output_file, "%s %.6f\n", terms[i], tf);

        free(terms[i]); 
    }

    free(terms);
    free(term_frequencies);

    fclose(input_file);
    fclose(output_file);

    printf("TF salvo em: %s\n", tf_output_file);
}
