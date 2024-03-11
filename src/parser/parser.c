#include <stdio.h>
#include "parser.h"
#include <string.h>
#include "tokens_enum.h"
#include "custom_logger.h"
#include "syntax.h"

void parser(char *token, LoadFileContext *context) {
    if (strlen(token) == 1 && strchr(context->keep_delimiters, token[0]) != NULL && *context->continueTable == 1) {
        ProcessFileContext params;
        params.token = token; 
        params.table = context->table;
        params.specialChars = context->keep_delimiters;
        params.line_number = context->line_number;
        params.char_position = context->char_position;
        params.continueTable = context->continueTable;
        params.tokens_enum = context->tokens_enum;
        params.syntax_table = context->syntax_table;
        callback(params);
        return;
    }
    char buffer_token[strlen(token)];
    int buffer_index = 0;
    for (int i = 0; i < strlen(token); i++) {
        if (strchr(context->keep_delimiters, token[i]) == NULL && *context->continueTable == 1) {
            buffer_token[buffer_index++] = token[i];
        }
        if (strchr(context->keep_delimiters, token[i]) != NULL && *context->continueTable == 1) {
            if (buffer_index > 0) {
                buffer_token[buffer_index] = '\0';
                ProcessFileContext params;
                params.token = buffer_token; 
                params.table = context->table;
                params.line_number = context->line_number;
                params.char_position = context->char_position;
                params.continueTable = context->continueTable;
                params.tokens_enum = context->tokens_enum;
                params.syntax_table = context->syntax_table;
                callback(params);
                buffer_index = 0;
            }
            char temp[2] = {token[i], '\0'};
            ProcessFileContext params;
            params.token = temp; 
            params.table = context->table;
            params.line_number = context->line_number;
            params.char_position = context->char_position;
            params.continueTable = context->continueTable;
            params.tokens_enum = context->tokens_enum;
            params.syntax_table = context->syntax_table;
            callback(params);
        }
    }
    if (buffer_index > 0 && *context->continueTable == 1) {
        buffer_token[buffer_index] = '\0';
        ProcessFileContext params;
        params.token = buffer_token; 
        params.table = context->table;
        params.line_number = context->line_number;
        params.char_position = context->char_position;
        params.continueTable = context->continueTable;
        params.tokens_enum = context->tokens_enum;
        params.syntax_table = context->syntax_table;
        callback(params);
    }
}
