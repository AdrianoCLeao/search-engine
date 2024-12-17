#include <stdio.h>
#include "../include/parsing/parsing.h"

#if defined(_WIN32) || defined(_WIN64)
    #include <direct.h> 
#else
    #include <unistd.h>
#endif


int main() {
    const char *input_file = "../data/wikipedia_dump.xml";
    parse_wikipedia_dump(input_file);
    printf("Processamento concluído.\n");
    return 0;
}