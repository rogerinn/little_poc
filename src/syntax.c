#include "syntax.h"

struct SyntaxRule syntax_table[] = {
    {
        "let", { 
                {{"VariableExpr", ",", "VariableExpr", ";"}, 3},
                {{"VariableExpr", "=", "VariableExpr", ";"}, 3},
                {{"VariableExpr", "=", "[", "]", ";"}, 4},
                {{"VariableExpr", "=", "{", "}", ";"}, 4}, 
                {{"VariableExpr", "=", "\"", "\"", ";"}, 4},
                {{"VariableExpr", "=", "\'", "\'", ";"}, 4},
                {{"VariableExpr", "=", "[", "VariableExpr", "]", ";"}, 5},
                {{"VariableExpr", "=", "{", "VariableExpr", "}", ";"}, 5},
                {{"VariableExpr", "=", "\"", "VariableExpr", "\"", ";"}, 5}, 
                {{"VariableExpr", "=", "\'", "VariableExpr", "\'", ";"}, 5},
                {{"VariableExpr", "=", "VariableExpr", "=", ">", "VariableExpr", ";"}, 6}, 
                {{"VariableExpr", "=", "VariableExpr", "=", ">", "{", "}", ";"}, 7},   
                {{"VariableExpr", "=", "VariableExpr", "=", ">", "{", "VariableExpr", "}", ";"}, 8},    
                {{"VariableExpr", "=", "(", "VariableExpr", ")", "=", ">", "{", "VariableExpr", "}", ";"}, 10}
            }, 14
    },
    {
        "const", { 
                {{"VariableExpr", ",", "VariableExpr", ";"}, 4},
                {{"VariableExpr", "=", "VariableExpr", ";"}, 4},
                {{"VariableExpr", "=", "[", "]", ";"}, 5},
                {{"VariableExpr", "=", "{", "}", ";"}, 5}, 
                {{"VariableExpr", "=", "[", "VariableExpr", "]", ";"}, 6}, 
                {{"VariableExpr", "=", "[", "MultExpr", "]", ";"}, 6},
                {{"VariableExpr", "=", "{", "MultExpr", "}", ";"}, 6},
                {{"VariableExpr", "=", "\"", "VariableExpr", "\"", ";"}, 6}, 
                {{"VariableExpr", "=", "\'", "VariableExpr", "\'", ";"}, 6},
                {{"VariableExpr", "=", "VariableExpr", "=", ">", "VariableExpr", ";"}, 7}, 
                {{"VariableExpr", "=", "VariableExpr", "=", ">", "{", "VariableExpr", "}", ";"}, 9},     
                {{"VariableExpr", "=", "(", "MultExpr", ")", "=", ">", "{", "MultExpr", "}", ";"}, 11}
            }, 12
    },
        {
        "import", { 
                {{"\"", "VariableExpr", "\"", ";"}, 4},
                {{"\'", "VariableExpr", "\'", ";"}, 4},
                {{"{", "VariableExpr", "}", ";"}, 4},
            }, 3
    }
};
