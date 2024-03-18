#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "loadfile.h"
#include "parser.h"
#include "hashmap.h"
#include "tokens_enum.h"
#include "syntax.h"
#include "controller_struct.h"
#include "controller_params.h"

#define INITIAL_BUFFER_SIZE 1024
#define INITIAL_BUFFER_SIZE 1024
#define BUFFER_INCREMENT 1024

void loadFile(LoadFileContext *context) {
    FILE* file = fopen(context->filename, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    char *buffer = (char *)malloc(INITIAL_BUFFER_SIZE * sizeof(char));
    if (buffer == NULL) {
        printf("Erro ao alocar memória para o buffer.\n");
        fclose(file);
        return;
    }

    size_t buffer_size = INITIAL_BUFFER_SIZE;
    char *line = NULL;
    size_t length = 0;
    int line_number = 1;
    while ((*context->continueTable == 1) && fgets(buffer, buffer_size, file) != NULL) {
        length = strlen(buffer);
        while (length == buffer_size - 1 && buffer[length - 1] != '\n') {
            buffer_size += BUFFER_INCREMENT;
            buffer = realloc(buffer, buffer_size * sizeof(char));
            if (buffer == NULL) {
                printf("Erro ao realocar memória para o buffer.\n");
                fclose(file);
                free(buffer);
                return;
            }
            if (fgets(buffer + length, BUFFER_INCREMENT, file) == NULL)
                break;
            length = strlen(buffer);
        }

        int i = 0;
        int start = 0;
        int char_position = 0;

        while (i < length && *context->continueTable == 1) {
            if (strchr(context->delimiters, buffer[i]) != NULL) {
                if (start != i) {
                    buffer[i] = '\0';
                    char_position++;
                    context->line_number = &line_number;
                    context->char_position = &char_position;
                    parser(&buffer[start], context);
                }
                start = i + 1;
            }
            i++;
        }
        if (start != i && *context->continueTable == 1) {
            buffer[i] = '\0';
            char_position++;
            context->line_number = &line_number;
            context->char_position = &char_position;
            parser(&buffer[start], context);

        }
        line_number++;
    }
    fclose(file);
    free(buffer);
}
