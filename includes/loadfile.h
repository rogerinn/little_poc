#ifndef LOAD_FILE_H
#define LOAD_FILE_H
#include "hashmap.h" 
#include "syntax.h" 
#include "callback_struct.h" 
#include "callback_params.h"

typedef struct {
    const char *filename;
    const char *delimiters;
    const char *keep_delimiters;
    KeywordEntry *table;
    int *continueTable;
    enum Tokens *tokens_enum;
    SyntaxRule *syntax_table;
    int *line_number;
    int *char_position;
} LoadFileContext;

void loadFile(LoadFileContext *context);


#endif 