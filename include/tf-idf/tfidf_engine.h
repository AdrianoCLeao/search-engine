#ifndef TFIDF_ENGINE_H
#define TFIDF_ENGINE_H

typedef struct {
    char **documents;  
    int num_documents; 
} TFIDFEngine;

void tfidf_initialize(TFIDFEngine *engine);
void tfidf_load_documents(TFIDFEngine *engine, const char *directory);
void tfidf_calculate(TFIDFEngine *engine);
void tfidf_search(TFIDFEngine *engine, const char *query);
void tfidf_free(TFIDFEngine *engine);

#endif
