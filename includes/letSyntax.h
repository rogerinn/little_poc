#ifndef LET_SYNTAX_H
#define LET_SYNTAX_H

#define MAX_KEYWORD_LEN 50

typedef struct LetSyntax {
    char token[MAX_KEYWORD_LEN];
    char VariableExpr[MAX_KEYWORD_LEN];
    char operator[MAX_KEYWORD_LEN];
    char TermExpr[MAX_KEYWORD_LEN];
    int count;
} LetSyntax;

#endif