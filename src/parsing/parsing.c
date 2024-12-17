#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#if defined(_WIN32) || defined(_WIN64)
    #define PATH_SEPARATOR "\\"
#else
    #define PATH_SEPARATOR "/"
#endif

#define BUFFER_SIZE 4096

void sanitize_title(char *title) {
    int i;
    for (i = 0; title[i]; i++) {
        if (!isalnum(title[i]) && title[i] != ' ') {
            title[i] = '_';
        }
    }
}

void parse_wikipedia_dump(const char *input_file) {
    FILE *file = fopen(input_file, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }

    char line[BUFFER_SIZE];
    char title[BUFFER_SIZE] = {0};
    char content[BUFFER_SIZE * 10] = {0};
    int in_text = 0, in_page = 0;

    FILE *output_file = NULL;

    while (fgets(line, BUFFER_SIZE, file)) {
        if (strstr(line, "<page>")) {
            in_page = 1;
            memset(title, 0, sizeof(title));
            memset(content, 0, sizeof(content));
        }

        if (in_page) {
            if (strstr(line, "<title>")) {
                char *start = strstr(line, "<title>") + 7;
                char *end = strstr(line, "</title>");
                if (start && end) {
                    strncpy(title, start, end - start);
                    sanitize_title(title);
                }
            }

            if (strstr(line, "<text>")) {
                in_text = 1;
                char *start = strstr(line, "<text>") + 6;
                strcat(content, start);
            } else if (in_text) {
                if (strstr(line, "</text>")) {
                    in_text = 0;
                    char *end = strstr(line, "</text>");
                    strncat(content, line, end - line);
                } else {
                    strcat(content, line);
                }
            }

            if (strstr(line, "</page>")) {
                in_page = 0;

                if (strlen(title) > 0 && strlen(content) > 0) {
                    char filename[BUFFER_SIZE];
                    snprintf(filename, sizeof(filename), "%s.txt", title);

                    output_file = fopen(filename, "w");
                    if (output_file) {
                        fprintf(output_file, "%s", content);
                        fclose(output_file);
                        printf("Arquivo criado: %s\n", filename);
                    } else {
                        perror("Erro ao criar o arquivo de saída");
                    }
                }
            }
        }
    }

    fclose(file);
}