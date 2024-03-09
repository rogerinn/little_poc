#include <stdio.h>
#include <string.h>
#include "struct_callback.h"
#include "struct_map.h"
#include "tokens_enum.h"
#include "syntax.h"
#include "loadfile.h"

void processToken(char* token, LoadFileContext* context) {
    if (strlen(token) == 1 && strchr(context->keep_delimiters, token[0]) != NULL) {
        context->callback(token, context->tabela, context->continueTabela, context->tokens_enum, context->teste, context->inBlock, context->lastToken);
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
                context->callback(buffer_token, context->tabela, context->continueTabela, context->tokens_enum, context->teste, context->inBlock, context->lastToken);
                buffer_index = 0;
            }
            char temp[2] = {token[i], '\0'};
            context->callback(temp, context->tabela, context->continueTabela, context->tokens_enum, context->teste, context->inBlock, context->lastToken);
        }
    }
    if (buffer_index > 0) {
        buffer_token[buffer_index] = '\0';
        context->callback(buffer_token, context->tabela, context->continueTabela, context->tokens_enum, context->teste, context->inBlock, context->lastToken);
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
<<<<<<< HEAD
    int inBlock = -1;
    char lastToken;
    LoadFileContext context = {filename, delimiters, keep_delimiters, callback, tabela, &continueTabela, &tokens_enum, &teste, &inBlock, &lastToken};
=======
    teste.let.count = 0;
    LoadFileContext context = {filename, delimiters, keep_delimiters, callback, tabela, &continueTabela, &tokens_enum, &teste};

>>>>>>> dbfc296 (update)
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
