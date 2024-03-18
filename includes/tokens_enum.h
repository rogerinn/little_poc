#ifndef TOKENS_ENUM_H 
#define TOKENS_ENUM_H

typedef enum Tokens {
    LET,
    CONST,
    IMPORT,
    BLANK = -1,
    IGNORE = -2,
    EXPR = -3
} Tokens;

Tokens convertIntToToken(int value);

#endif