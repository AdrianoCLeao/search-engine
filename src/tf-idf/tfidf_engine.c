#include "../include/tf-idf/tfidf_engine.h"
#include "../include/utils/utils.h"
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_TERMS 10000
#define TEMP_FILENAME_FORMAT "temp_tokens_doc_%d.txt"

void tokenize_to_file(const char *content, const char *output_file);
int term_exists(const char *term, TermData terms[], int term_count);
void calculate_tf_from_file(const char *token_file, int doc_index);
void calculate_idf(TermData terms[], int term_count, int num_documents);

void tfidf_initialize(TFIDFEngine *engine)
{
    engine->documents = NULL;
    engine->num_documents = 0;
}

void tfidf_load_documents(TFIDFEngine *engine, const char *directory)
{
    int i;
    char **file_list;
    int file_count = 0;

    list_files_in_directory(directory, &file_list, &file_count);
    engine->documents = malloc(file_count * sizeof(char *));
    if (!engine->documents)
    {
        perror("Erro ao alocar memória para documentos");
        exit(1);
    }

    for (i = 0; i < file_count; i++)
    {
        engine->documents[i] = read_file_content(file_list[i]);
        free(file_list[i]);
    }

    free(file_list);
    engine->num_documents = file_count;
    printf("Carregados %d documentos\n", file_count);
}

void tfidf_calculate(TFIDFEngine *engine)
{
    TermData terms[MAX_TERMS];
    int term_count = 0;
    char temp_filename[256];
    int i, j;

    printf("Iniciando calculo de TF-IDF...\n");

    for (i = 0; i < engine->num_documents; i++)
    {
        snprintf(temp_filename, sizeof(temp_filename), TEMP_FILENAME_FORMAT, i + 1);
        tokenize_to_file(engine->documents[i], temp_filename);
        printf("Tokens do documento %d salvos em %s\n", i + 1, temp_filename);
    }

    for (i = 0; i < engine->num_documents; i++)
    {
        snprintf(temp_filename, sizeof(temp_filename), TEMP_FILENAME_FORMAT, i + 1);
        printf("Processando tokens do arquivo: %s\n", temp_filename);
        calculate_tf_from_file(temp_filename, i);
    }

    calculate_idf(terms, term_count, engine->num_documents);

    printf("\nResultados TF-IDF:\n");
    for (i = 0; i < term_count; i++)
    {
        printf("Termo: %s -> ", terms[i].term);
        for (j = 0; j < engine->num_documents; j++)
        {
            printf("[Doc %d: %.4f] ", j + 1, terms[i].tfidf_values[j]);
        }
        printf("\n");
    }

    for (i = 0; i < term_count; i++)
    {
        free(terms[i].term);
        free(terms[i].tfidf_values);
    }
}

void tokenize_to_file(const char *content, const char *output_file) {
    const char *base_dir = "data/tokens";
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

int term_exists(const char *term, TermData terms[], int term_count)
{
    int i;
    for (i = 0; i < term_count; i++)
    {
        if (strcmp(terms[i].term, term) == 0)
        {
            return i;
        }
    }
    return -1;
}

void calculate_tf_from_file(const char *token_file, int doc_index) {
    int i;
    const char *base_dir = "data/tokens";
    char full_path[512];
    const char *base_dir_tf = "data/tf";
    create_directory(base_dir_tf); 

    char tf_output_file[512];
    snprintf(full_path, sizeof(full_path), "%s/%s", base_dir, token_file);

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
        for (i = 0; i < term_count; i++) {
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

    for (i = 0; i < term_count; i++) {
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

void calculate_idf(TermData terms[], int term_count, int num_documents)
{
    int i, j;
    for (i = 0; i < term_count; i++)
    {
        int doc_count = 0;

        for (j = 0; j < num_documents; j++)
        {
            if (terms[i].tfidf_values[j] > 0.0)
            {
                doc_count++;
            }
        }

        double idf = log((double)num_documents / (1 + doc_count));
        for (j = 0; j < num_documents; j++)
        {
            terms[i].tfidf_values[j] *= idf;
        }
    }
}

void tfidf_search(TFIDFEngine *engine, const char *query) {
    printf("Busca por '%s' ainda não está implementada.\n", query);
}

void tfidf_free(TFIDFEngine *engine)
{
    int i;
    for (i = 0; i < engine->num_documents; i++)
    {
        free(engine->documents[i]);
    }
    free(engine->documents);
}
