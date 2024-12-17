#include "../include/tf-idf/tfidf_engine.h"
#include "../include/utils/utils.h"
#include <string.h>
#include <ctype.h>
#include <math.h>

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

    for (i = 0; i < file_count; i++) {
        engine->documents[i] = read_file_content(file_list[i]);
        free(file_list[i]);
    }

    free(file_list);
    engine->num_documents = file_count;
    printf("Carregados %d documentos\n", file_count);
}

void tfidf_calculate(TFIDFEngine *engine) {
    printf("Cálculo de TF-IDF ainda a ser implementado\n");
}

void tfidf_search(TFIDFEngine *engine, const char *query) {
    printf("Busca por '%s' ainda a ser implementada\n", query);
}

void tfidf_free(TFIDFEngine *engine) {
    int i;
    for (i = 0; i < engine->num_documents; i++) {
        free(engine->documents[i]);
    }
    free(engine->documents);
}