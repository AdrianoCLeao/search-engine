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

/**
 * @brief Calculates the TF-IDF values for all terms in the documents.
 * 
 * This function processes the documents loaded in the `TFIDFEngine` to calculate
 * the Term Frequency (TF), Inverse Document Frequency (IDF), and the resulting
 * TF-IDF values. The results are saved in the specified directories.
 * 
 * The process includes:
 * - Tokenizing the documents into individual terms and saving tokens to temporary files.
 * - Calculating the term frequency (TF) for each document.
 * - Identifying unique terms across all documents.
 * - Calculating the inverse document frequency (IDF) for all terms.
 * - Combining TF and IDF values to calculate TF-IDF for each term in each document.
 * 
 * @param engine Pointer to the `TFIDFEngine` structure containing the documents to process.
 */
void tfidf_calculate(TFIDFEngine *engine);

/**
 * @brief Performs a TF-IDF-based search for a query string.
 * 
 * This function calculates the relevance of each document in the `TFIDFEngine`
 * to the given query string using the TF-IDF model. It tokenizes the query,
 * calculates TF-IDF scores for each term in the query across all documents,
 * and ranks the documents based on their relevance. The results are saved
 * as a JSON file.
 * 
 * The process includes:
 * - Tokenizing the query string and extracting its terms.
 * - Matching query terms with precomputed TF-IDF values.
 * - Calculating a relevance score for each document.
 * - Sorting documents by their relevance scores in descending order.
 * - Generating a JSON file containing the ranked results.
 * 
 * @param engine Pointer to the `TFIDFEngine` structure containing the loaded documents.
 * @param query The query string to search for in the documents.
 */
void tfidf_search(TFIDFEngine *engine, const char *query);
void tfidf_free(TFIDFEngine *engine);

#endif
