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

int isInsideMultExpr = 0;
char multExprValues[50][100];
int multExprValueCount = 0;

const char specialChars[] = "\\\"\'`()<>|{}[];.=+-/*?%$&#@!_-:,~";

typedef struct TokensRule {
    char next_tokens[50][20];
    int next_token_count;
} TokensRule;

struct SyntaxRule {
    char keyword[20];
    TokensRule tokens[100];
    int totalSyntaxes;
};

struct Map {
    char *lastToken[100];
};

struct Map map;

struct SyntaxRule syntax_table[] = {
    {
        "let", { 
                {{"VariableExpr", ",", "VariableExpr", ";"}, 4},
                {{"VariableExpr", "=", "VariableExpr", ";"}, 4},
                {{"VariableExpr", "=", "[", "]", ";"}, 5},
                {{"VariableExpr", "=", "{", "}", ";"}, 5}, 
                {{"VariableExpr", "=", "{", "VariableExpr", "}", ";"}, 6}, 
                {{"VariableExpr", "=", "\"", "VariableExpr", "\"", ";"}, 6}, 
                {{"VariableExpr", "=", "\'", "VariableExpr", "\'", ";"}, 6}, 
                {{"VariableExpr", "=", "[", "VariableExpr", "]", ";"}, 6}, 
                {{"VariableExpr", "=", "[", "MultExpr", "]", ";"}, 5},
                {{"VariableExpr", "=", "VariableExpr", "=", ">", "{", "}", ";"}, 8},  
                {{"VariableExpr", "=", "VariableExpr", "=", ">", "{", "VariableExpr", "}", ";"}, 9},    
                {{"VariableExpr", "=", "(", "MultExpr", ")", "=", ">", "{", "MultExpr", "}", ";"}, 11}
            }, 12
    },
    {
        "const", { 
                {{"VariableExpr", "=", "VariableExpr", ";"}, 4},
                {{"VariableExpr", "=", "[", "]", ";"}, 5},
                {{"VariableExpr", "=", "{", "}", ";"}, 5}, 
                {{"VariableExpr", ",", "VariableExpr", ",", "VariableExpr", ";"}, 6},
                {{"VariableExpr", "=", "{", "VariableExpr", "}", ";"}, 6}, 
                {{"VariableExpr", "=", "\"", "VariableExpr", "\"", ";"}, 6}, 
                {{"VariableExpr", "=", "\'", "VariableExpr", "\'", ";"}, 6}, 
                {{"VariableExpr", "=", "[", "VariableExpr", "]", ";"}, 6}, 
                {{"VariableExpr", "=", "VariableExpr", "=", ">", "{", "}", ";"}, 8},  
                {{"VariableExpr", "=", "VariableExpr", "=", ">", "{", "VariableExpr", "}", ";"}, 9},    
                {{"VariableExpr", "=", "(", "VariableExpr", ",", "VariableExpr", ")", "=", ">", "{", "VariableExpr", "}", ";"}, 13},
            }, 11
    }
};

Tokens convertIntToToken(int value) {
    switch(value) {
        case 0:
            return LET;
        case 1:
            return CONST;
        case 2:
            return IMPORT;
        case -1:
            return BLANK;
        default:
            return BLANK;
    }
}

int FnLet(char *str, enum Tokens *fn, Syntax *syntax, int *id, char *lastToken) {
        static int counter, OpCodesCounter;
        if(*fn == BLANK) {
            *fn = convertIntToToken(*id);
            counter = 0;
            OpCodesCounter = 0;
            printf("OPBLOCK: %s\n", str); 
            return 0;
        }
        if (isInsideMultExpr) {
            if(strcmp(map.lastToken[0], "(") == 0) {
                map.lastToken[1] = ")";
            }
            if(strcmp(map.lastToken[0], "{") == 0) {
                map.lastToken[1] = "}";
            }
            if(strcmp(map.lastToken[0], "[") == 0) {
                map.lastToken[1] = "]";
            }
            if (strcmp(str, map.lastToken[1]) != 0) {
                strcpy(multExprValues[multExprValueCount], str);
                multExprValueCount++;
                return 0;
            }
            for (int i = 0; i < multExprValueCount; i++) {
                printf("MultExpr %s \n", multExprValues[i]);
            }
            isInsideMultExpr = 0;
            multExprValueCount = 0;
        }
        for(int i = 0; i < syntax_table[*id].totalSyntaxes; i++){
            int totalTokens = syntax_table[*id].tokens[i].next_token_count;
            if (strcmp(syntax_table[*id].tokens[i].next_tokens[counter], "MultExpr") == 0) { 
                if(strcspn(str, specialChars) != strlen(str)) {
                   continue;
                }
                isInsideMultExpr = 1;
                strcpy(multExprValues[multExprValueCount], str); 
                multExprValueCount++;
                counter++;
                OpCodesCounter++;
                return 0;
            }
        }
        for(int IteratorOpcode = 0; IteratorOpcode < syntax_table[*id].totalSyntaxes; IteratorOpcode++) {
            int totalTokens = syntax_table[*id].tokens[IteratorOpcode].next_token_count;
            if (strcmp(str,syntax_table[*id].tokens[IteratorOpcode].next_tokens[counter]) == 0) {
                map.lastToken[0] = syntax_table[*id].tokens[IteratorOpcode].next_tokens[counter];
                printf("OPCODE: %s\n", syntax_table[*id].tokens[IteratorOpcode].next_tokens[counter]);
                counter++;
                OpCodesCounter++;
                if(counter == totalTokens) {
                    counter = 0;
                    OpCodesCounter = 0;
                    resetSyntax(fn, syntax);
                }
                return 0;
            }
        }
        for(int i = 0; i < syntax_table[*id].totalSyntaxes; i++){
            int totalTokens = syntax_table[*id].tokens[i].next_token_count;
            if (strcmp(syntax_table[*id].tokens[i].next_tokens[counter], "VariableExpr") == 0) { 
                if(strcspn(str, specialChars) != strlen(str)) {
                   continue;
                }
                printf("VariableExpr: %s\n", str);
                counter++;
                OpCodesCounter++;
                if(counter == totalTokens) {
                    counter = 0;
                    OpCodesCounter = 0;
                    resetSyntax(fn, syntax);
                    return 0;
                }
                return 0;
            }
        }
        if(isInsideMultExpr <= 0) {
            return 1;
        } 
        return 0;
}

void printCallback(char *str, KeywordEntry tabela[], int *continueTabela, enum Tokens *tokens_enum, Syntax *syntax, int *inBlock, char *lastToken) {
    if(*continueTabela) {
        KeywordEntry *funcao = busca(tabela, str);
        if(funcao == NULL && *tokens_enum == BLANK) {
            *continueTabela = 0;
            printf("Nenhuma instrucao inicial foi encontrada\n");
        } 
        if(funcao != NULL && *tokens_enum != BLANK) {
            *inBlock = funcao->id;
            FnLet(str, tokens_enum, syntax, inBlock, lastToken); 
            if(isInsideMultExpr <= 0) {
                *continueTabela = 0;
                printf("Nao e possivel usar mais de uma declaracao '%s' \n", str);
            }
        }
        if(funcao != NULL && *tokens_enum == BLANK) {
            *inBlock = funcao->id;
            int teste = FnLet(str, tokens_enum, syntax, inBlock, lastToken); 
            if(teste) {
                *continueTabela = 0;
                printf("Nao e possivel usar caracteres especiais para nomear '%s' \n", str);
            }
        }
        if(funcao == NULL && *tokens_enum != BLANK) {
            int teste = FnLet(str, tokens_enum, syntax, inBlock, lastToken);
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