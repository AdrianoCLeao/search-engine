#include <stdio.h>
#include "../include/parsing/parsing.h"
#include "../include/utils/utils.h"
#include <limits.h>

#define MAX_PATH_LENGTH 1024

int main() {
    char full_path[MAX_PATH_LENGTH];

    get_wikipedia_dump_directory(full_path, sizeof(full_path));
    printf("Lendo arquivo: %s\n", full_path);

    parse_wikipedia_dump(full_path);

    printf("Processamento concluído.\n");
    return 0;
}