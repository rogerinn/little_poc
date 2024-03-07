#include <stdio.h>
#include "struct_map.h"
#include "dot_verify.h"
#include "hashmap.h"
#include "loadfile.h"
#include "struct_callback.h"
#include "tokens_enum.h"
#include "letSyntax.h"
#include <string.h>

#define TAM 31

void (*FnInBlock)(const char *str, enum Tokens *tokens_enum, LetSyntax *letSyntax);

void resetLetSyntax(enum Tokens *fn, LetSyntax *syntax) {
    strcpy(syntax->token, "");
    strcpy(syntax->VariableExpr, "");
    strcpy(syntax->operator, "");
    strcpy(syntax->TermExpr, "");
    syntax->count = 0;
    FnInBlock = NULL;
    *fn = BLANK;
}

void FnLet(const char *str, enum Tokens *fn, LetSyntax *letSyntax) {
    if(*fn == BLANK) {
        *fn = LET;
        letSyntax->count = 6; 
        printf("Alterando variavel para: %d\n", letSyntax->count);   
        return;
    } 
    resetLetSyntax(fn, letSyntax);
    printf("Sub %d:\n", letSyntax->count);   
    return;
}

int printCallback(const char *str, KeywordEntry tabela[], int *continueTabela, enum Tokens *tokens_enum, LetSyntax *letSyntax) {
    if(*continueTabela) {
        void *funcao = busca(tabela, str);
        if(funcao != NULL && *tokens_enum == BLANK) {
            FnInBlock = &FnLet;
            FnLet(str, tokens_enum, letSyntax); 
            return 0;
        }
        if(funcao == NULL && *tokens_enum != BLANK) {
            FnInBlock(str, tokens_enum, letSyntax);
            return 0;
        }
        if(funcao != NULL && *tokens_enum != BLANK) {
            *continueTabela = 0;
            printf("Nao e possivel usar duas instrucoes: %s\n", str);
            return 1;
        }
        if(funcao == NULL && *tokens_enum == BLANK) {
            *continueTabela = 0;
            printf("Nenhuma instrucao inicial foi encontrada\n");
            return 1;
        } 
    }
    return 1;
}

int main(int argc, char *argv[]) {
    KeywordEntry tabela[TAM], entrada;
    if (argc < 2) {
        printf("Uso: %s <arquivo>\n", argv[0]);
        return 1;
    }
    int result = dotVerify(argv[1]);
    if(result) { 
        createToken(tabela);
        loadFile(argv[1], " \t\n", "\\\"\'`()<>|{}[];.=+-/*?%$&#@!_-:,~", printCallback, tabela);
    }
    return result;
}