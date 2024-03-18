#include <stdio.h>
#include <string.h>
#include "controller_params.h"
#include "controller_struct.h"
#include "hashmap.h"
#include "tokens_enum.h"
#include "syntax.h"
#include "stack_struct.h"
#include "custom_logger.h"

void resetStack(int totalTokens, int count, Tokens *tokens_enum) {
    if(totalTokens <= count) {
        stack = (Stack){ .subStack = {NULL}, .block = NULL, .count = 0 };
        *tokens_enum = convertIntToToken(-2);
    }
}

int handlerStack (char *token) { 
    if(stack.block == NULL){
        stack.block = strdup(token);
        return 0;
    }
    stack.count++;
    stack.subStack[stack.count].token = strdup(token);
    return 0;
}

int isBlock(ProcessFileContext context, int id) {
    SPACING();
    LOG_MSG("Block", "found: %s", context.token);
    *context.tokens_enum = convertIntToToken(id);
    return 0;    
}

int validate (ProcessFileContext context) {
    KeywordEntry *search = busca(context.table, context.token);
    //None declaration
    if(stack.block == NULL && search == NULL) {
        LOG_MSG("Error", "Não foi possivel encontrar declarações iniciais ou de bloco: { %s }", context.token);
        LOG_MSG("Error", "Erro na linha: %d elemento: %d { %s }\n" , *context.line_number, *context.char_position, context.token);
        return 1;
    }
    //Doble declaration
    if(stack.block != NULL && search != NULL) {
        LOG_MSG("Error", "Não é possivel usar duas declarações de bloco.");
        LOG_MSG("Error", "Erro na linha: %d elemento: %d { %s }" , *context.line_number, *context.char_position, stack.subStack[stack.count].token);
        return 1;
    }
    //Set block 
    if(stack.block == NULL && search != NULL) {
        isBlock(context, search->id);
        return 0;
    }
    return 0;
}

void callback (ProcessFileContext context) {
    int error = validate(context);
    if(error) {;
        LOG_MSG("Error-validate", "Verifique os caracteres ou blocos.");
        LOG_MSG("Error-validate", "Erro na linha: %d  elemento: %d { %s }" , *context.line_number, *context.char_position, context.token);
        *context.continueTable = 0;
        return;
    }
    if (*context.tokens_enum == IGNORE) {
        *context.tokens_enum = convertIntToToken(-1);
        return;
    }

    int verifyStack = handlerStack(context.token);
    if(verifyStack) {
        *context.continueTable = 0;
        return;
    }
}
