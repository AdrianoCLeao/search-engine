#include "../include/tf-idf/tfidf_engine.h"
#include "../include/utils/utils.h"
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_TOKENS 10000
#define TOKEN_INCREMENT 10000

void tokenize_to_file(const char *content, const char *output_file);
int term_exists(const char *term, TermData terms[], int term_count);
void calculate_tf(TermData terms[], int term_count, char **tokens, int token_count, int doc_index);
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
    char **tokens = NULL;
    int token_count, term_count = 0;
    int i, j;

    printf("Iniciando calculo de TF-IDF...\n");

    for (i = 0; i < engine->num_documents; i++)
    {
        token_count = 0;

        tokenize(engine->documents[i], &tokens, &token_count);
        printf("Tokenizou documento %d com %d tokens\n", i + 1, token_count);

        for (j = 0; j < token_count; j++)
        {
            int idx = term_exists(tokens[j], terms, term_count);
            if (idx == -1)
            {
                terms[term_count].term = strdup(tokens[j]);
                terms[term_count].tfidf_values = calloc(engine->num_documents, sizeof(double));
                if (!terms[term_count].tfidf_values)
                {
                    printf("Erro ao alocar memória para TF-IDF\n");
                    exit(1);
                }
                idx = term_count++;
            }
        }

        calculate_tf(terms, term_count, tokens, token_count, i);

        for (j = 0; j < token_count; j++)
        {
            free(tokens[j]);
        }
        free(tokens);
        tokens = NULL;
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
    const char *delimiters = " \t\n\r,.!?;:\"()[]{}";
    char *copy = strdup(content);
    if (!copy) {
        printf("Erro ao alocar memória para cópia do conteúdo\n");
        exit(1);
    }

    FILE *file = fopen(output_file, "w");
    if (!file) {
        printf("Erro ao abrir arquivo para escrita: %s\n", output_file);
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

void calculate_tf(TermData terms[], int term_count, char **tokens, int token_count, int doc_index)
{
    int i, j;
    for (i = 0; i < term_count; i++)
    {
        int term_occurrences = 0;

        for (j = 0; j < token_count; j++)
        {
            if (strcmp(terms[i].term, tokens[j]) == 0)
            {
                term_occurrences++;
            }
        }

        terms[i].tfidf_values[doc_index] = (double)term_occurrences / token_count;
    }
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

void tfidf_search(TFIDFEngine *engine, const char *query)
{
    printf("Busca por '%s' ainda a ser implementada\n", query);
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
