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
        stack = (Stack){ .subStack = {NULL}, .count = 0 };
        *tokens_enum = convertIntToToken(-2);
    }
}

int handlerStack (char *token) { 
    if(stack.subStack[0].token == NULL && stack.count == 0){
        stack.count = 0;
        stack.subStack[stack.count].token = strdup(token);
        return 0;
    }
    stack.count++;
    stack.subStack[stack.count].token = strdup(token);
    return 0;
}

int isVariableExpr(ProcessFileContext context, int id) {
    for(int i = 0; i < syntax_table[id].totalSyntaxes; i++){
    int totalTokens = syntax_table[id].tokens[i].next_token_count;
        if (strcmp(syntax_table[id].tokens[i].next_tokens[stack.count], "VariableExpr") == 0) { 
            if(strcspn(context.token, context.specialChars) == strlen(context.token)) {
                LOG_MSG("VariableExpr", "found: %s", context.token);
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
    if(stack.subStack[0].token == NULL && stack.count == 0 && (*context.tokens_enum == BLANK || *context.tokens_enum == IGNORE)){
        return 0;
    }
    //Double declaration
    KeywordEntry *search = busca(context.table, stack.subStack[stack.count].token);
    if(stack.subStack[stack.count].token != NULL && search == NULL && *context.tokens_enum == BLANK) {
        LOG_MSG("Error", "Não foi possivel encontrar declarações iniciais ou de bloco: %s", stack.subStack[0].token);
        LOG_MSG("Error", "Erro na linha: %d elemento: %d { %s }" , *context.line_number, *context.char_position, stack.subStack[stack.count].token);
        *context.tokens_enum = convertIntToToken(-1);
        return 1;
    }
    if(stack.subStack[stack.count].token != NULL && search != NULL && *context.tokens_enum != BLANK) {
        LOG_MSG("Error", "Não é possivel usar duas declarações de bloco.");
        LOG_MSG("Error", "Erro na linha: %d elemento: %d { %s }" , *context.line_number, *context.char_position, stack.subStack[stack.count].token);
        *context.tokens_enum = convertIntToToken(-1);
        return 1;
    }
    //set block
    if(stack.subStack[stack.count].token != NULL && search != NULL && *context.tokens_enum == BLANK) {
        SPACING();
        LOG_MSG("Block", "found: %s", stack.subStack[stack.count].token);

        if(isVariableExpr(context, search->id)){
            *context.tokens_enum = convertIntToToken(search->id);
            return 0;
        }
        if(isOpcode(context, search->id)){
            *context.tokens_enum = convertIntToToken(search->id);
            return 0;
        }
        return 1;
    }
    //set opcode
    if(stack.subStack[stack.count].token != NULL && search == NULL && *context.tokens_enum != BLANK) {
        if(isVariableExpr(context, *context.tokens_enum)){
            *context.tokens_enum = convertIntToToken(*context.tokens_enum);
            return 0;
        }
        if(isOpcode(context, *context.tokens_enum)){
            *context.tokens_enum = convertIntToToken(*context.tokens_enum);
            return 0;
        }
        return 1;
    }
}

void callback (ProcessFileContext context) {
    int error = validate(context);
    if(error) {;
        LOG_MSG("Error", "Verifique os caracteres ou blocos.");
        LOG_MSG("Error", "Erro na linha: %d  elemento: %d { %s }" , *context.line_number, *context.char_position, context.token);
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
