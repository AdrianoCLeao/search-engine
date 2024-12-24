#ifndef TFIDF_ENGINE_H
#define TFIDF_ENGINE_H

/**
 * @brief Structure to represent a TF-IDF engine.
 * 
 * This structure stores the list of documents and the total number of documents
 * loaded into the engine for TF-IDF calculations.
 */
typedef struct {
    char **documents;  
    int num_documents; 
} TFIDFEngine;

/**
 * @brief Structure to represent data for a specific term.
 * 
 * This structure stores the term itself, its corresponding TF-IDF values for
 * all documents, and its IDF (Inverse Document Frequency).
 */
typedef struct {
    char *term;
    double *tfidf_values;
    double idf;
} TermData;

#define MAX_TERMS 10000

/**
 * @brief Initializes the TF-IDF engine.
 * 
 * This function initializes the `TFIDFEngine` structure by setting its document
 * list to `NULL` and its document count to zero.
 * 
 * @param engine Pointer to the `TFIDFEngine` structure to be initialized.
 */
void tfidf_initialize(TFIDFEngine *engine);

/**
 * @brief Loads documents from a specified directory into the TF-IDF engine.
 * 
 * This function scans the specified directory for files, reads their content,
 * and stores the content in the `TFIDFEngine` structure. Each document's content
 * is dynamically allocated and stored as a string.
 * 
 * @param engine Pointer to the `TFIDFEngine` structure to store the documents.
 * @param directory Path to the directory containing the document files.
 */
void tfidf_load_documents(TFIDFEngine *engine, const char *directory);
void tfidf_calculate(TFIDFEngine *engine);
void tfidf_search(TFIDFEngine *engine, const char *query);
void tfidf_free(TFIDFEngine *engine);

#endif
