#include "../include/tf-idf/tfidf_engine.h"
#include "../include/utils/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    clear_screen();
    TFIDFEngine engine;

    /* tfidf_initialize(&engine);
    tfidf_load_documents(&engine, "data");

    tfidf_calculate(&engine); */

    char query[256];
    printf("Digite a consulta: ");
    fgets(query, sizeof(query), stdin);
    query[strcspn(query, "\n")] = '\0';

    tfidf_search(&engine, query);

/*     tfidf_free(&engine); */
    return 0;
}
