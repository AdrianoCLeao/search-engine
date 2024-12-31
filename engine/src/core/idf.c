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

#include "../include/core/idf.h"
#include "../include/utils/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

void calculate_idf(TermData terms[], int term_count, int num_documents) {
    const char *base_dir_tf = "../data/tf";
    const char *base_dir_idf = "../data/idf";
    create_directory(base_dir_idf);

    char idf_file[512];
    snprintf(idf_file, sizeof(idf_file), "%s/idf_values.txt", base_dir_idf);

    FILE *idf_output = fopen(idf_file, "w");
    if (!idf_output) {
        printf("Erro ao criar arquivo para salvar IDF: %s\n", idf_file);
        exit(1);
    }

    char tf_file[512];
    for (int i = 0; i < term_count; i++) {
        int doc_count = 0;

        for (int j = 0; j < num_documents; j++) {
            snprintf(tf_file, sizeof(tf_file), "%s/tf_doc_%d.txt", base_dir_tf, j + 1);

            FILE *file = fopen(tf_file, "r");
            if (!file) {
                printf("Erro ao abrir arquivo de TF: %s\n", tf_file);
                fclose(idf_output);
                exit(1);
            }

            char buffer[256];
            while (fgets(buffer, sizeof(buffer), file)) {
                char term[256];
                double tf_value;

                if (sscanf(buffer, "%s %lf", term, &tf_value) == 2) {
                    if (strcmp(term, terms[i].term) == 0) {
                        doc_count++;
                        break; 
                    }
                }
            }

            fclose(file);
        }

        double idf = 0.0;
        if (doc_count > 0) {
            idf = log((double)num_documents / (1 + doc_count));
        }

        fprintf(idf_output, "%s %.6f\n", terms[i].term, idf);
    }

    fclose(idf_output);
    printf("Valores de IDF salvos em: %s\n", idf_file);
}
