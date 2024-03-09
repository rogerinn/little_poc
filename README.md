## 🔧 Compilação e uso

Para rodar a aplicação, execulte os comandos abaixo:

1) Abra um terminal e copie este repositório com o comando
    ```bash
    git clone https://github.com/rogerinn/little_poc/
    ```
    ou use a opção de download.

2) Entre na pasta com
    ```bash
    cd little_poc
    ```

3) Rode a aplicação, docker irá instalar dependencias e configurações externas.
    ```bash
    ./start.sh nome_do_arquivo.js > opcodes.txt
    ```
    Ou 
    ```bash
    ./start.sh nome_do_arquivo.js > opcodes.rnc
    ```
    para linkar os `std`.
    
4) Atualmente o modelo é simples, já esta em refatoração para o pre-compiler JIT e AOT.
```c
   struct SyntaxRule syntax_table[] = {
    {
        "let", { 
                {{"VariableExpr", ",", "VariableExpr", ";"}, 4},
                {{"VariableExpr", "=", "VariableExpr", ";"}, 4},
                {{"VariableExpr", "=", "[", "]", ";"}, 5},
                {{"VariableExpr", "=", "{", "}", ";"}, 5}, 
                {{"VariableExpr", "=", "[", "VariableExpr", "]", ";"}, 6}, 
                {{"VariableExpr", "=", "[", "MultExpr", "]", ";"}, 6},
                {{"VariableExpr", "=", "{", "VariableExpr", "}", ";"}, 6}, 
                {{"VariableExpr", "=", "{", "MultExpr", "}", ";"}, 6},
                {{"VariableExpr", "=", "\"", "VariableExpr", "\"", ";"}, 6}, 
                {{"VariableExpr", "=", "\'", "VariableExpr", "\'", ";"}, 6},
                {{"VariableExpr", "=", "VariableExpr", "=", ">", "VariableExpr", ";"}, 7}, 
                {{"VariableExpr", "=", "VariableExpr", "=", ">", "{", "VariableExpr", "}", ";"}, 9},    
                {{"VariableExpr", "=", "(", "MultExpr", ")", "=", ">", "{", "MultExpr", "}", ";"}, 11}
            }, 13
    },
    {
        "const", { 
                {{"VariableExpr", ",", "VariableExpr", ";"}, 4},
                {{"VariableExpr", "=", "VariableExpr", ";"}, 4},
                {{"VariableExpr", "=", "[", "]", ";"}, 5},
                {{"VariableExpr", "=", "{", "}", ";"}, 5}, 
                {{"VariableExpr", "=", "[", "VariableExpr", "]", ";"}, 6}, 
                {{"VariableExpr", "=", "[", "MultExpr", "]", ";"}, 6},
                {{"VariableExpr", "=", "{", "VariableExpr", "}", ";"}, 6}, 
                {{"VariableExpr", "=", "{", "MultExpr", "}", ";"}, 6},
                {{"VariableExpr", "=", "\"", "VariableExpr", "\"", ";"}, 6}, 
                {{"VariableExpr", "=", "\'", "VariableExpr", "\'", ";"}, 6},
                {{"VariableExpr", "=", "VariableExpr", "=", ">", "VariableExpr", ";"}, 7}, 
                {{"VariableExpr", "=", "VariableExpr", "=", ">", "{", "VariableExpr", "}", ";"}, 9},    
                {{"VariableExpr", "=", "(", "MultExpr", ")", "=", ">", "{", "MultExpr", "}", ";"}, 11}
            }, 13
    },
    {
        "import", { 
                {{"\"", "VariableExpr", "\"", ";"}, 4},
                {{"\'", "VariableExpr", "\'", ";"}, 4},
                {{"{", "VariableExpr", "}", ";"}, 4},
            }, 3
    }
};
```
5) Base atual todo modelo de JIT e AOT funcionam independentes de SO, toda camanda de supervisionamento é feita de forma `rootless`, com `cgroups` e `namespaces` melhorando isolamento, capacidade de compilação e interpretação para micros compilações macros ou funções distribuidas. 
```javascript
OPBLOCK: import
OPCODE: "
VariableExpr: aa
OPCODE: "
OPCODE: ;
OPBLOCK: import
OPCODE: '
VariableExpr: aa
OPCODE: '
OPCODE: ;
OPBLOCK: let
VariableExpr: aa
OPCODE: =
OPCODE: (
MultExpr: a 
MultExpr: , 
MultExpr: a 
OPCODE: )
OPCODE: =
OPCODE: >
OPCODE: {
MultExpr: let 
MultExpr: name 
MultExpr: = 
MultExpr: " 
MultExpr: Rogerio 
MultExpr: " 
MultExpr: ; 
MultExpr: console 
MultExpr: . 
MultExpr: log 
MultExpr: ( 
MultExpr: " 
MultExpr: aaa 
MultExpr: " 
MultExpr: ) 
MultExpr: ; 
MultExpr: return 
MultExpr: name 
MultExpr: ; 
OPCODE: }
OPCODE: ;
OPBLOCK: let
VariableExpr: aa
OPCODE: =
OPCODE: (
MultExpr: a 
MultExpr: , 
MultExpr: a 
OPCODE: )
OPCODE: =
OPCODE: >
OPCODE: {
MultExpr: let 
MultExpr: name 
MultExpr: = 
MultExpr: " 
MultExpr: Rogerio 
MultExpr: " 
MultExpr: ; 
MultExpr: console 
MultExpr: . 
MultExpr: log 
MultExpr: ( 
MultExpr: " 
MultExpr: aaa 
MultExpr: " 
MultExpr: ) 
MultExpr: ; 
MultExpr: return 
MultExpr: name 
MultExpr: ; 
OPCODE: }
OPCODE: ;
OPBLOCK: let
VariableExpr: aa
OPCODE: =
OPCODE: (
MultExpr: a 
MultExpr: , 
MultExpr: a 
OPCODE: )
OPCODE: =
OPCODE: >
OPCODE: {
MultExpr: let 
MultExpr: name 
MultExpr: = 
MultExpr: " 
MultExpr: Rogerio 
MultExpr: " 
MultExpr: ; 
MultExpr: console 
MultExpr: . 
MultExpr: log 
MultExpr: ( 
MultExpr: " 
MultExpr: aaa 
MultExpr: " 
MultExpr: ) 
MultExpr: ; 
MultExpr: return 
MultExpr: name 
MultExpr: ; 
OPCODE: }
OPCODE: ;
OPBLOCK: let
VariableExpr: aa
OPCODE: =
OPCODE: (
MultExpr: a 
MultExpr: , 
MultExpr: a 
OPCODE: )
OPCODE: =
OPCODE: >
OPCODE: {
MultExpr: let 
MultExpr: name 
MultExpr: = 
MultExpr: " 
MultExpr: Rogerio 
MultExpr: " 
MultExpr: ; 
MultExpr: console 
MultExpr: . 
MultExpr: log 
MultExpr: ( 
MultExpr: " 
MultExpr: aaa 
MultExpr: " 
MultExpr: ) 
MultExpr: ; 
MultExpr: return 
MultExpr: name 
MultExpr: ; 
OPCODE: }
OPCODE: ;
OPBLOCK: let
VariableExpr: aa
OPCODE: =
OPCODE: (
MultExpr: a 
MultExpr: , 
MultExpr: a 
OPCODE: )
OPCODE: =
OPCODE: >
OPCODE: {
MultExpr: let 
MultExpr: name 
MultExpr: = 
MultExpr: " 
MultExpr: Rogerio 
MultExpr: " 
MultExpr: ; 
MultExpr: console 
MultExpr: . 
MultExpr: log 
MultExpr: ( 
MultExpr: " 
MultExpr: aaa 
MultExpr: " 
MultExpr: ) 
MultExpr: ; 
MultExpr: return 
MultExpr: name 
MultExpr: ; 
OPCODE: }
OPCODE: ;
OPBLOCK: let
VariableExpr: name
OPCODE: =
OPCODE: [
MultExpr: 1 
MultExpr: , 
MultExpr: 2 
OPCODE: ]
OPCODE: ;

 ------->> Tempo: 0.000066 segundos <<-------
```
