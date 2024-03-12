#ifndef CONTROLLER_PARAMS_H
#define CONTROLLER_PARAMS_H
#include "hashmap.h"
#include "syntax.h"

typedef struct {
    char *token;
    KeywordEntry *table;
    const char *specialChars;
    int *line_number;
    int *char_position;
    int *continueTable;
    enum Tokens *tokens_enum;
    SyntaxRule *syntax_table;
} ProcessFileContext;

#endif