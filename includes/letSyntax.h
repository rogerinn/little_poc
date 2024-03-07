#ifndef LET_SYNTAX_H
#define LET_SYNTAX_H

#define MAX_TOKEN_LEN 50
#define MAX_EXPR_LEN 50
#define MAX_OPERATOR_LEN 1
#define MAX_OPERATORS 3

typedef struct LetSyntax1 {
    char token[MAX_TOKEN_LEN];
    char VariableExpr[MAX_EXPR_LEN];
    char Operators[MAX_OPERATORS][MAX_OPERATOR_LEN];
    char TermExpr[MAX_EXPR_LEN];
    int OperatorsCount;
    int *count;
} LetSyntax1;

typedef struct LetSyntax {
    char token[MAX_EXPR_LEN];
    char VariableExpr[MAX_EXPR_LEN];
    char operator[MAX_EXPR_LEN];
    char TermExpr[MAX_EXPR_LEN];
    int count;
    LetSyntax1 let;
} LetSyntax;

#endif