#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "controller_params.h"
#include "controller_struct.h"
#include "hashmap.h"
#include "tokens_enum.h"
#include "syntax.h"
#include "stack_struct.h"
#include "custom_logger.h"

void resetStack(int totalTokens, int count, Tokens *tokens_enum) {
    if(totalTokens <= count && *tokens_enum != EXPR) {
        LOG_MSG("rrrrr", "%s", count);
        stack = (Stack){ .subStack = {NULL}, .block = NULL, .count = 0 };
        *tokens_enum = IGNORE;
    }
}

int handlerStack (char *token, Tokens tokens_enum) { 
    if(stack.block == NULL){
        stack.block = strdup(token);
        stack.idBlock = tokens_enum;
        return 0;
    }
    stack.count++;
    stack.subStack[stack.count].token = strdup(token);
    return 0;
}

int isBlock(ProcessFileContext context, int id) {
    SPACING();
    *context.tokens_enum = convertIntToToken(id);
    return 0;    
}

bool isIdentifierExpr(ProcessFileContext context, int id) {
    for(int i = 0; i < syntax_table[id].totalSyntaxes; i++){
    int totalTokens = syntax_table[id].tokens[i].next_token_count;
        if (strcmp(syntax_table[id].tokens[i].next_tokens[stack.count], "IdentifierExpr") == 0) { 
            if(strcspn(context.token, context.specialChars) == strlen(context.token)) {
                return true;
            }
        }
    }
    return false;
}

bool isOpcode(ProcessFileContext context, int id, int count) {\
    if(strcspn(context.token, context.specialChars) != strlen(context.token)) {
        for(int i = 0; i < syntax_table[id].totalSyntaxes; i++) {
            int totalTokens = syntax_table[id].tokens[i].next_token_count;
            if (strcmp(context.token,syntax_table[id].tokens[i].next_tokens[count]) == 0) {
                resetStack(totalTokens, stack.count, context.tokens_enum);
                return true;
            }
        }
    }
    return false;
}

bool isMultExpr(ProcessFileContext context, int id) {
    for(int i = 0; i < syntax_table[id].totalSyntaxes; i++){
    int totalTokens = syntax_table[id].tokens[i].next_token_count;
        if (strcmp(syntax_table[id].tokens[i].next_tokens[stack.count], "MultExpr") == 0) { 
            Stack *subStack = malloc(sizeof(Stack));
            if (subStack == NULL) {
                LOG_MSG("ERROR", "Erro ao realocar memória");
                *context.continueTable = 0;
                return false;
            }
            subStack->block = strdup("MultExpr");
            subStack->subStack[0].token = context.token;
            subStack->count = 1;
            stack.subBlock[stack.count] = subStack;
            *context.tokens_enum = convertIntToToken(-3);
            stack.count++;
            return true;
        }
    }
    return false;
}

bool isVariableExpr(ProcessFileContext context, int id) {
    if(strcspn(context.token, context.specialChars) == strlen(context.token)) { 
        for(int i = 0; i < syntax_table[id].totalSyntaxes; i++){
        int totalTokens = syntax_table[id].tokens[i].next_token_count;
            if (strcmp(syntax_table[id].tokens[i].next_tokens[stack.count], "VariableExpr") == 0) { 
                    if(isMultExpr(context, id)) {
                        return true;
                    }
                    LOG_MSG("VariableExpr", "found: %s", context.token);
                    return true;
            }
        }
    }
    return false;
}

int validate (ProcessFileContext context) {
    KeywordEntry *search = busca(context.table, context.token);
    //None declaration
    if(stack.block == NULL && search == NULL && *context.tokens_enum != EXPR) {
        LOG_MSG("Error", "Não foi possivel encontrar declarações iniciais ou de bloco: { %s }", context.token);
        LOG_MSG("Error", "Erro na linha: %d elemento: %d { %s }\n" , *context.line_number, *context.char_position, context.token);
        return 1;
    }
    //Doble declaration
    if(stack.block != NULL && search != NULL && *context.tokens_enum != EXPR) {
        LOG_MSG("Error", "Não é possivel usar duas declarações de bloco.");
        LOG_MSG("Error", "Erro na linha: %d elemento: %d { %s }" , *context.line_number, *context.char_position, stack.subStack[stack.count].token);
        return 1;
    }
    //Set block 
    if(stack.block == NULL && search != NULL && *context.tokens_enum != EXPR) {
        isBlock(context, search->id);
        LOG_MSG("Block", "found: %s", context.token);
        return 0;
    }
    //Set OpCodesExpr
    if(stack.block != NULL && search == NULL && *context.tokens_enum != EXPR){
        if(isIdentifierExpr(context, *context.tokens_enum)) {
            LOG_MSG("IdentifierExpr", "found: %s", context.token);
            return 0;
        }
        if(isOpcode(context, *context.tokens_enum, stack.count)) {
            LOG_MSG("OPCODE", "found: %s", context.token);
            return 0;
        }
        if(isVariableExpr(context, *context.tokens_enum)) {
            return 0;
        }
        if(isMultExpr(context, *context.tokens_enum)) {
            return 0;
        }
    }  
    //Set MultOpCodesExpr
    if(stack.block != NULL && search == NULL && *context.tokens_enum == EXPR) {
        if(isOpcode(context, stack.idBlock, stack.count) && stack.subBlock[stack.count -1]->count == 1) {
            LOG_MSG("VariableExpr", "found: %s", stack.subBlock[stack.count -1]->subStack[0].token);
            LOG_MSG("OPCODE", "found: %s", context.token);
            LOG_MSG("rrrrr", "%d", stack.count);
            stack = (Stack){ .subStack = {NULL}, .block = NULL, .count = 0 };
            *context.tokens_enum = IGNORE;
            free(stack.subBlock[stack.count]);
            return 0;
        }
        if(stack.subBlock[stack.count -1]->count == 1) {
            LOG_MSG("MultExpr", "found: %s", stack.subBlock[stack.count -1]->subStack[0].token);
            (stack.subBlock[stack.count -1]->count)++; 
        }
        if(isOpcode(context, stack.idBlock, stack.count) && stack.subBlock[stack.count -1]->count != 1) {
            LOG_MSG("OPCODE", "found: %s", context.token);
            stack = (Stack){ .subStack = {NULL}, .block = NULL, .count = 0 };
            *context.tokens_enum = IGNORE;
            free(stack.subBlock[stack.count]);
            return 0;
        }
        LOG_MSG("MultExpr", "found: %s", context.token);
        free(stack.subBlock[stack.count]);
        return 0;
    } 
    return 1;
}

void callback (ProcessFileContext context) {
    int error = validate(context);
    if(error) {
        SPACING();
        LOG_MSG("Validate", "Verifique os caracteres ou blocos.");
        LOG_MSG("Validate", "Erro na linha: %d  elemento: %d { %s }" , *context.line_number, *context.char_position, context.token);
        *context.continueTable = 0;
        return;
    }
    if (*context.tokens_enum == IGNORE) {
        *context.tokens_enum = convertIntToToken(-1);
        return;
    }
    if (*context.tokens_enum == EXPR) {
        return;
    }

    int verifyStack = handlerStack(context.token, *context.tokens_enum);
    if(verifyStack) {
        *context.continueTable = 0;
        return;
    }
}
