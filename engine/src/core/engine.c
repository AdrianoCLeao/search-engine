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
#include "../include/core/tokenizer.h"
#include "../include/core/tf.h"
#include "../include/core/idf.h"
#include "../include/utils/utils.h"
#include "../include/json/json.h"
#include "../include/sort/quick_sort.h"
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_TERMS 10000
#define TEMP_FILENAME_FORMAT "temp_tokens_doc_%d.txt"

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
    int i, j;

    char temp_filename[256];
    char tf_file[512];
    char idf_file[512];
    char output_file[512];

    const char *base_dir_tf = "../data/tf";
    const char *base_dir_idf = "../data/idf";
    const char *output_dir = "../data/tfidf";

    create_directory(base_dir_tf);
    create_directory(base_dir_idf);
    create_directory(output_dir);

    snprintf(idf_file, sizeof(idf_file), "%s/idf_values.txt", base_dir_idf);
    snprintf(output_file, sizeof(output_file), "%s/tfidf_results.txt", output_dir);

    printf("Tokenizando documentos e calculando TF...\n");
    for (i = 0; i < engine->num_documents; i++) {
        snprintf(temp_filename, sizeof(temp_filename), TEMP_FILENAME_FORMAT, i + 1);
        tokenize_to_file(engine->documents[i], temp_filename);
        printf("Tokens do documento %d salvos em %s\n", i + 1, temp_filename);

        calculate_tf_from_file(temp_filename, i);
        printf("TF do documento %d salvo em data/tf/tf_doc_%d.txt\n", i + 1, i + 1);
    }

    printf("Coletando termos e calculando IDF...\n");
    TermData terms[MAX_TERMS];
    int term_count = 0;

    const char *base_dir = "../data/tokens";
    char full_path[512];

    for (i = 0; i < engine->num_documents; i++) {
        snprintf(temp_filename, sizeof(temp_filename), TEMP_FILENAME_FORMAT, i + 1);
        snprintf(full_path, sizeof(full_path), "%s/%s", base_dir, temp_filename); 

        FILE *file = fopen(full_path, "r"); 
        if (!file) {
            printf("Erro ao abrir arquivo de tokens: %s\n", full_path);
            exit(1);
        }

        char buffer[256];
        while (fgets(buffer, sizeof(buffer), file)) {
            buffer[strcspn(buffer, "\n")] = '\0';

            if (term_exists(buffer, terms, term_count) == -1 && term_count < MAX_TERMS) {
                terms[term_count].term = strdup(buffer);
                if (!terms[term_count].term) {
                    printf("Erro ao alocar memória para termo\n");
                    fclose(file);
                    exit(1);
                }
                term_count++;
            }
        }
        fclose(file);
    }

    calculate_idf(terms, term_count, engine->num_documents);

    printf("Calculando e salvando valores de TF-IDF...\n");
    FILE *idf_input = fopen(idf_file, "r");
    if (!idf_input) {
        printf("Erro ao abrir arquivo de IDF: %s\n", idf_file);
        exit(1);
    }

    FILE *result_file = fopen(output_file, "w");
    if (!result_file) {
        printf("Erro ao abrir arquivo para salvar resultados TF-IDF: %s\n", output_file);
        fclose(idf_input);
        exit(1);
    }

    char term[256];
    double idf_value;
    while (fscanf(idf_input, "%s %lf", term, &idf_value) == 2) {
        fprintf(result_file, "Termo: %s -> ", term);

        for (j = 0; j < engine->num_documents; j++) {
            snprintf(tf_file, sizeof(tf_file), "%s/tf_doc_%d.txt", base_dir_tf, j + 1);

            FILE *tf_input = fopen(tf_file, "r");
            if (!tf_input) {
                printf("Erro ao abrir arquivo de TF: %s\n", tf_file);
                fclose(result_file);
                fclose(idf_input);
                exit(1);
            }

            char tf_term[256];
            double tf_value = 0.0;

            while (fscanf(tf_input, "%s %lf", tf_term, &tf_value) == 2) {
                if (strcmp(tf_term, term) == 0) {
                    break;
                }
            }

            fclose(tf_input);

            double tfidf_value = tf_value * idf_value;
            fprintf(result_file, "[Doc %d: %.6f] ", j + 1, tfidf_value);
        }
        fprintf(result_file, "\n");
    }

    fclose(idf_input);
    fclose(result_file);

    for (i = 0; i < term_count; i++) {
        free(terms[i].term);
    }

    printf("Resultados de TF-IDF salvos em: %s\n", output_file);
}

void tfidf_search(TFIDFEngine *engine, const char *query) {
    char cwd[512];

    int i;
    const char *base_dir_tf = "../data/tf";
    const char *base_dir_idf = "../data/idf";
    const char *base_dir_temp = "";

    create_directory(base_dir_tf);
    create_directory(base_dir_idf);

    char tf_file[512];
    char idf_file[512];
    char query_token_file[512];

    snprintf(idf_file, sizeof(idf_file), "%s/idf_values.txt", base_dir_idf);
    snprintf(query_token_file, sizeof(query_token_file), "%s/temp_query_tokens.txt", base_dir_temp);

    create_directory("../data/tokens");

    tokenize_to_file(query, query_token_file);

    char **query_terms = NULL;
    int query_term_count = 0;

    FILE *query_file = fopen("../data/tokens/temp_query_tokens.txt", "r");
    if (!query_file) {
        printf("Erro ao abrir arquivo temporário de tokens da query: %s\n", query_token_file);
        exit(1);
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), query_file)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        query_terms = realloc(query_terms, (query_term_count + 1) * sizeof(char *));
        if (!query_terms) {
            printf("Erro ao alocar memória para os termos da query\n");
            fclose(query_file);
            exit(1);
        }
        query_terms[query_term_count] = strdup(buffer);
        query_term_count++;
    }
    fclose(query_file);

    FILE *idf_input = fopen(idf_file, "r");
    if (!idf_input) {
        printf("Erro ao abrir arquivo de IDF: %s\n", idf_file);
        exit(1);
    }

    double *idf_values = calloc(query_term_count, sizeof(double));
    if (!idf_values) {
        printf("Erro ao alocar memória para valores de IDF\n");
        fclose(idf_input);
        exit(1);
    }

    char term[256];
    double idf_value;

    while (fscanf(idf_input, "%s %lf", term, &idf_value) == 2) {
        for (i = 0; i < query_term_count; i++) {
            if (strcmp(term, query_terms[i]) == 0) {
                idf_values[i] = idf_value;
                break;
            }
        }
    }

    fclose(idf_input);

    int file_count = 0;
    char **file_list;
    list_files_in_directory("../data/wiki", &file_list, &file_count);

    double *document_scores = calloc(file_count, sizeof(double));
    if (!document_scores) {
        printf("Erro ao alocar memória para relevâncias dos documentos\n");
        free(idf_values);
        exit(1);
    }

    for (int doc_index = 0; doc_index < file_count; doc_index++) {
        snprintf(tf_file, sizeof(tf_file), "%s/tf_doc_%d.txt", base_dir_tf, doc_index + 1);

        FILE *tf_input = fopen(tf_file, "r");
        if (!tf_input) {
            printf("Erro ao abrir arquivo de TF: %s\n", tf_file);
            free(idf_values);
            free(document_scores);
            exit(1);
        }

        char tf_term[256];
        double tf_value;

        while (fscanf(tf_input, "%s %lf", tf_term, &tf_value) == 2) {
            for (i = 0; i < query_term_count; i++) {
                if (strcmp(tf_term, query_terms[i]) == 0) {
                    document_scores[doc_index] += tf_value * idf_values[i];
                }
            }
        }

        fclose(tf_input);
    }

    quickSort(document_scores, 0, file_count - 1, 0);

    char json_buffer[1024 * 50];
    size_t rem_len = sizeof(json_buffer);
    char *json_dest = json_buffer;

    json_dest = json_objOpen(json_dest, NULL, &rem_len);

    for (i = 0; i < file_count; i++) {
        char *file_basename = get_file_basename(file_list[i]);

        char file_summary[201] = {0};
        FILE *file_content = fopen(file_list[i], "r");
        if (file_content) {
            fread(file_summary, sizeof(char), 200, file_content);
            fclose(file_content);

            normalize_summary(file_summary);
        } else {
            snprintf(file_summary, sizeof(file_summary), "Resumo indisponível");
        }

        json_dest = json_objOpen(json_dest, file_basename, &rem_len);
        json_dest = json_nstr(json_dest, "summary", file_summary, &rem_len);
        json_dest = json_num(json_dest, "rank", document_scores[i], &rem_len);
        json_dest = json_objClose(json_dest, &rem_len);

        free(file_basename);
    }

    json_dest = json_objClose(json_dest, &rem_len);
    json_dest = json_finalize(json_dest, &rem_len);

    FILE *output_file = fopen("../results.json", "w");
    if (output_file) {
        fputs(json_buffer, output_file);
        fclose(output_file);
    } else {
        printf("Erro ao salvar resultados em JSON\n");
    }

    free(idf_values);
    free(document_scores);

    for (i = 0; i < query_term_count; i++) {
        free(query_terms[i]);
    }
    free(query_terms);

    for (i = 0; i < file_count; i++) {
        free(file_list[i]);
    }
    free(file_list);

    remove(query_token_file);
}

void tfidf_free(TFIDFEngine *engine) {
    int i;
    for (i = 0; i < engine->num_documents; i++) {
        free(engine->documents[i]);
    }
    free(engine->documents);
}
