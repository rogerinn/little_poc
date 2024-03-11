#ifndef TOKENS_ENUM_H 
#define TOKENS_ENUM_H

typedef enum Tokens {
    LET,
    CONST,
    IMPORT,
    IGNORE = -2,
    BLANK = -1,
}Tokens;

Tokens convertIntToToken(int value);

#endif