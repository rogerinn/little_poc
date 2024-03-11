#include <stdio.h>
#include <time.h>
#include "dot_verify.h"
#include "hashmap.h"
#include "loadfile.h"
#include "tokens_enum.h"
#include "syntax.h"

#define TAM 11

const char specialChars[] = "\\\"\'`()<>|{}[];.=+-/*?%$&#@!_-:,~";

int main(int argc, char *argv[]) {
    KeywordEntry table[TAM], entrada;
    if (argc < 2) {
        printf("Uso: %s <arquivo>\n", argv[0]);
        return 1;
    }
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    int result = dotVerify(argv[1]);
    if(result) { 
        int continueTable = 1;
        enum Tokens tokens_enum = BLANK;
        SyntaxRule *syntax = syntax_table;
        LoadFileContext context = {argv[1], " \t\n", specialChars, table, &continueTable, &tokens_enum, syntax};
        createToken(table);
        loadFile(&context);
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\n ------->> Tempo: %f segundos <<-------\n", cpu_time_used);
    return 1;
}