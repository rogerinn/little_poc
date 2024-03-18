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
    stack.subStack[stack.count].token = strdup(token);
    stack.count++;
    return 0;
}

int isBlock(ProcessFileContext context, int id) {
    SPACING();
    LOG_MSG("Block", "found: %s", context.token);
    *context.tokens_enum = convertIntToToken(id);
    return 0;    
}

int isIdentifierExpr(ProcessFileContext context, int id) {
    for(int i = 0; i < syntax_table[id].totalSyntaxes; i++){
    int totalTokens = syntax_table[id].tokens[i].next_token_count;
        if (strcmp(syntax_table[id].tokens[i].next_tokens[stack.count], "IdentifierExpr") == 0) { 
            if(strcspn(context.token, context.specialChars) == strlen(context.token)) {
                LOG_MSG("IdentifierExpr", "found: %s", context.token);
                resetStack(totalTokens, stack.count, context.tokens_enum);
                return 1;
            }
        }
    }
    return 0;
}

int isOpcode(ProcessFileContext context, int id) {
    for(int IteratorOpcode = 0; IteratorOpcode < syntax_table[id].totalSyntaxes; IteratorOpcode++) {
        int totalTokens = syntax_table[id].tokens[IteratorOpcode].next_token_count;
        if (strcmp(context.token,syntax_table[id].tokens[IteratorOpcode].next_tokens[stack.count]) == 0) {
            LOG_MSG("OPCODE", "found: %s", context.token);
            resetStack(totalTokens, stack.count, context.tokens_enum);
            return 1;
        }
    }
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
    //Set isIdentifierExpr
    if(stack.block != NULL && search == NULL){
        if(isIdentifierExpr(context, *context.tokens_enum))
            return 0;
    }
    //Set isOpCode
    if(stack.block != NULL && search == NULL){
        if(isOpcode(context, *context.tokens_enum))
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

    int verifyStack = handlerStack(context.token);
    if(verifyStack) {
        *context.continueTable = 0;
        return;
    }
}
