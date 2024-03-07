#ifndef LOAD_FILE_h
#define LOAD_FILE_h
#include "struct_map.h" 
#include "struct_callback.h"

void loadFile(const char *filename, const char *delimiters, const char *keep_delimiters, PrintCallback callback, KeywordEntry tabela[]);

#endif 