#include "tokens_enum.h"

Tokens convertIntToToken(int value) {
    switch(value) {
        case 0:
            return LET;
        case 1:
            return CONST;
        case 2:
            return IMPORT;
        case -3:
            return EXPR;
        case -2:
            return IGNORE;
        case -1:
            return BLANK;
        default:
            return BLANK;
    }
}