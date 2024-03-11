#ifndef SYNTAX_H
#define SYNTAX_H

typedef struct TokensRule {
    char next_tokens[50][20];
    int next_token_count;
} TokensRule;

typedef struct SyntaxRule {
    char keyword[20];
    TokensRule tokens[100];
    int totalSyntaxes;
} SyntaxRule;

extern SyntaxRule syntax_table[];

#endif