#include "syntax.h"

struct SyntaxRule syntax_table[] = {
    {
        "let", { 
                {{"IdentifierExpr", ",", "VariableExpr", ";"}, 3},
                {{"IdentifierExpr", "=", "VariableExpr", ";"}, 3},
                {{"IdentifierExpr", "=", "[", "]", ";"}, 4},
                {{"IdentifierExpr", "=", "{", "}", ";"}, 4}, 
                {{"IdentifierExpr", "=", "\"", "\"", ";"}, 4},
                {{"IdentifierExpr", "=", "\'", "\'", ";"}, 4},
                {{"IdentifierExpr", "=", "[", "VariableExpr", "]", ";"}, 5},
                {{"IdentifierExpr", "=", "{", "VariableExpr", "}", ";"}, 5},
                {{"IdentifierExpr", "=", "\"", "VariableExpr", "\"", ";"}, 5}, 
                {{"IdentifierExpr", "=", "\'", "VariableExpr", "\'", ";"}, 5},
                {{"IdentifierExpr", "=", "VariableExpr", "=", ">", "VariableExpr", ";"}, 6}, 
                {{"IdentifierExpr", "=", "VariableExpr", "=", ">", "{", "}", ";"}, 7},   
                {{"IdentifierExpr", "=", "VariableExpr", "=", ">", "{", "VariableExpr", "}", ";"}, 8},    
                {{"IdentifierExpr", "=", "(", "MultExpr", ")", "=", ">", "{", "MultExpr", "}", ";"}, 10}
            }, 14
    }
};
