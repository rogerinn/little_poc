#include <stdio.h>
#include <string.h>
#include "struct_callback.h"
#include "struct_map.h"
#include "tokens_enum.h"
#include "syntax.h"
#include "loadfile.h"

void processToken(const char* token, LoadFileContext* context) {
    if (strlen(token) == 1 && strchr(context->keep_delimiters, token[0]) != NULL) {
        context->callback(token, context->tabela, context->continueTabela, context->tokens_enum, context->teste);
        return;
    }
    
    char buffer_token[strlen(token)];
    int buffer_index = 0;
    for (int i = 0; i < strlen(token); i++) {
        if (strchr(context->keep_delimiters, token[i]) == NULL) {
            buffer_token[buffer_index++] = token[i];
        }
        if (strchr(context->keep_delimiters, token[i]) != NULL) {
            if (buffer_index > 0) {
                buffer_token[buffer_index] = '\0';
                context->callback(buffer_token, context->tabela, context->continueTabela, context->tokens_enum, context->teste);
                buffer_index = 0;
            }
            char temp[2] = {token[i], '\0'};
            context->callback(temp, context->tabela, context->continueTabela, context->tokens_enum, context->teste);
        }
    }
    if (buffer_index > 0) {
        buffer_token[buffer_index] = '\0';
        context->callback(buffer_token, context->tabela, context->continueTabela, context->tokens_enum, context->teste);
    }
}

void loadFile(const char* filename, const char* delimiters, const char* keep_delimiters, PrintCallback callback, KeywordEntry tabela[]) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    int continueTabela = 1;
    enum Tokens tokens_enum = BLANK;
    Syntax teste;
    teste.count = 0;
    LoadFileContext context = {filename, delimiters, keep_delimiters, callback, tabela, &continueTabela, &tokens_enum, &teste};

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file) != NULL && continueTabela == 1) {
        int i = 0;
        int start = 0;
        int length = strlen(buffer);
        while (i < length && continueTabela == 1) {
            if (strchr(delimiters, buffer[i]) != NULL) {
                if (start != i) {
                    buffer[i] = '\0';
                    processToken(&buffer[start], &context);
                }
                start = i + 1;
            }
            i++;
        }
        if (start != i && continueTabela == 1) {
            buffer[i] = '\0';
            processToken(&buffer[start], &context);
        }
    }

    fclose(file);
}
