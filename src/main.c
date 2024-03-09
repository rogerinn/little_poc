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

void resetSyntax(enum Tokens *fn, Syntax *syntax) {
    memset(syntax, 0, sizeof(Syntax));
    FnInBlock = NULL;
    *fn = BLANK;
}

const char specialChars[] = "\\\"\'`()<>|{}[];.=+-/*?%$&#@!_-:,~";

typedef struct TokensRule {
    char next_tokens[10][20];
    int next_token_count;
} TokensRule;

struct SyntaxRule {
    char keyword[20];
    TokensRule tokens[100];
    int totalSyntaxes;
};

struct SyntaxRule syntax_table[] = {
    {
        "let", {
                {{"VariableExpr", "=", "VariableExpr", "=", ">", "{", "VariableExpr", "}"}, 8},
                {{"VariableExpr", "=", "VariableExpr", "=", ">", "{", "}"}, 7},
                {{"VariableExpr", "=", "VariableExpr"}, 3}, 
                {{"VariableExpr", "=", "\"", "VariableExpr", "\""}, 5}, 
                {{"VariableExpr", "=", "\'", "VariableExpr", "\'"}, 5}, 
                {{"VariableExpr", "=", "[", "VariableExpr", "]"}, 5},
                {{"VariableExpr", "=", "[", "]"}, 4}
            }, 7
    },
    {
        "const", {
            {{"VariableExpr", "=", "\"", "VariableExpr", "\""}, 5}, 
            {{"VariableExpr", "=", "\'", "VariableExpr", "\'"}, 5}
        }, 2
    }
};

void checkCounter (int count, int syntaxCount, enum Tokens *fn, Syntax *syntax) {
    if(count == syntaxCount){ 
        resetSyntax(fn, syntax);
    }
}

int FnLet(const char *str, enum Tokens *fn, Syntax *syntax) {
    for(int i = 0; i < syntax_table[LET].totalSyntaxes; i++) {
        static int counter, OpCodesCounter;
        if(*fn == BLANK) {
            *fn = LET;
            syntax->let.OperatorsCount = 3; 
            syntax->let.count = syntax_table[LET].tokens[i].next_token_count;
            counter = 0;
            OpCodesCounter = 0;
            printf("OpBlock: %s\n", str);   
            return 0;
        }
        if(*fn == LET) {
            strcpy(syntax->let.token, syntax_table[LET].keyword);
            syntax->let.count = syntax_table[LET].tokens[i].next_token_count; 
        }
        if (strcmp(syntax_table[LET].tokens[i].next_tokens[counter], "VariableExpr") == 0) { 
            if(strcspn(str, specialChars) != strlen(str)) {
                if (i < syntax_table[LET].totalSyntaxes) {
                    continue;
                }
                return 1;
            }
            strcpy(syntax->let.VariableExpr, str);
            printf("Opcode VariableExpr: %s\n", syntax->let.VariableExpr);
            counter++;
            checkCounter(counter, syntax->let.count, fn, syntax);
            return 0;
        }
        if (strcmp(str,syntax_table[LET].tokens[i].next_tokens[counter]) == 0) {
            strcpy(syntax->let.Operators[OpCodesCounter], str);
            printf("Opcode signal: %s\n", syntax->let.Operators[OpCodesCounter]);
            counter++;
            checkCounter(counter, syntax->let.count, fn, syntax);
            return 0;
        }
    }
}

void printCallback(const char *str, KeywordEntry tabela[], int *continueTabela, enum Tokens *tokens_enum, Syntax *syntax) {
    if(*continueTabela) {
        void *funcao = busca(tabela, str);
        if(funcao == NULL && *tokens_enum == BLANK) {
            *continueTabela = 0;
            printf("Nenhuma instrucao inicial foi encontrada\n");
        } 
        if(funcao != NULL && *tokens_enum != BLANK) {
            *continueTabela = 0;
            printf("Nao e possivel usar duas instrucoes: %s\n", str);
        }
        if(funcao != NULL && *tokens_enum == BLANK) {
            FnInBlock = &FnLet;
            int teste = FnLet(str, tokens_enum, syntax); 
            if(teste) {
                *continueTabela = 0;
                printf("Nao e possivel usar caracteres especiais para nomear '%s' \n", str);
            }
        }
        if(funcao == NULL && *tokens_enum != BLANK) {
            int teste = FnInBlock(str, tokens_enum, syntax);
            if(teste) {
                *continueTabela = 0;
                printf("Nao e possivel usar caracteres especiais para nomear '%s' \n", str);
            }
        }
    }
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