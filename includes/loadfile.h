#ifndef LOAD_FILE_h
#define LOAD_FILE_h
#include "struct_map.h" 
#include "struct_callback.h"

typedef struct {
    const char* filename;
    const char* delimiters;
    const char* keep_delimiters;
    PrintCallback callback;
    KeywordEntry* tabela;
    int *continueTabela;
    enum Tokens *tokens_enum;
    Syntax *teste; 
} LoadFileContext;


void loadFile(const char *filename, const char *delimiters, const char *keep_delimiters, PrintCallback callback, KeywordEntry tabela[]);
void processToken(const char* token, LoadFileContext* context);

#endif 