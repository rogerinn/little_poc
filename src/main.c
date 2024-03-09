#include <stdio.h>
#include "struct_map.h"
#include "dot_verify.h"
#include "hashmap.h"
#include "loadfile.h"
#include "struct_callback.h"
#include "tokens_enum.h"
#include "syntax.h"
#include <string.h>
#include <time.h>

#define TAM 31

int (*FnInBlock)(const char *str, enum Tokens *tokens_enum, Syntax *Syntax);

const char specialChars[] = "\\\"\'`()<>|{}[];.=+-/*?%$&#@!_-:,~";

void resetSyntax(enum Tokens *fn, Syntax *syntax) {
    strcpy(syntax->let.token, "");
    strcpy(syntax->let.VariableExpr, "");
    strcpy(syntax->let.TermExpr, "");
    for (int i = 0; i < syntax->let.OperatorsCount; i++) {
        strcpy(syntax->let.Operators[i], ""); // Define cada elemento como uma string vazia
    }
    syntax->let.OperatorsCount = 0;
    syntax->let.count = 0;
    FnInBlock = NULL;
    *fn = BLANK;
}

struct SyntaxRule {
    char keyword[20];
    char next_tokens[10][20];
    int next_token_count;
};

struct SyntaxRule syntax_table[] = {
    {"let", {"VariableExpr", "=", "\"", "VariableExpr", "\""}, 5},
    {"const", {"VariableExpr", "=", "\"", "VariableExpr", "\""}, 5}
};

int FnLet(const char *str, enum Tokens *fn, Syntax *syntax) {
    static int counter, OpCodesCounter;
    if(*fn == BLANK) {
        *fn = LET;
        strcpy(syntax->let.token, syntax_table[LET].keyword);
        syntax->let.count = syntax_table[LET].next_token_count; 
        syntax->let.OperatorsCount = 3; 
        counter = 0;
        OpCodesCounter = 0;
        printf("OpBlock: %s\n", str);   
        return 0;
    } 
    if(counter >= (syntax->let.count -1)){ 
        resetSyntax(fn, syntax);
    }
    if (strcmp(syntax_table[LET].next_tokens[counter], "VariableExpr") == 0) { 
        if(strcspn(str, specialChars) != strlen(str)) {
            return 1;
        }
        strcpy(syntax->let.VariableExpr, str);
        printf("Opcode VariableExpr: %s\n", syntax->let.VariableExpr);
        counter++;
        return 0;
    }
    if (strcmp(str,syntax_table[LET].next_tokens[counter]) == 0) {
        strcpy(syntax->let.Operators[OpCodesCounter], str);
        printf("Opcode signal: %s\n", syntax->let.Operators[OpCodesCounter]);
        counter++;
        return 0;
    } 
}

int printCallback(const char *str, KeywordEntry tabela[], int *continueTabela, enum Tokens *tokens_enum, Syntax *Syntax) {
    if(*continueTabela) {
        void *funcao = busca(tabela, str);
        if(funcao != NULL && *tokens_enum == BLANK) {
            FnInBlock = &FnLet;
            int teste = FnLet(str, tokens_enum, Syntax); 
            if(teste) {
                *continueTabela = 0;
                printf("Nao e possivel usar caracteres especiais para nomear '%s' \n", str);
                return 1;
            }
            return 0;
        }
        if(funcao == NULL && *tokens_enum != BLANK) {
            int teste = FnInBlock(str, tokens_enum, Syntax);
            if(teste) {
                *continueTabela = 0;
                printf("Nao e possivel usar caracteres especiais para nomear '%s' \n", str);
                return 1;
            }
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
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    int result = dotVerify(argv[1]);
    if(result) { 
        createToken(tabela);
        loadFile(argv[1], " \t\n", specialChars, printCallback, tabela);
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\n ------->> Tempo: %f segundos -------\n", cpu_time_used);
    return result;
}