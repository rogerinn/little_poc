#ifndef STRUCT_CALLBACK_H
#define STRUCT_CALLBACK_H
#include "struct_map.h"
#include "tokens_enum.h"
#include "letSyntax.h"

typedef int (*PrintCallback)(const char *str, KeywordEntry tabela[], int *continueTabela, enum Tokens *tokens_enum, LetSyntax *letSyntax);

#endif