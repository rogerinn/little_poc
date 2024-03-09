## 🔧 Compilação e uso

Para rodar a aplicação, execute os comandos abaixo:

1) Abra um terminal e copie este repositório com o comando
    ```bash
    git clone https://github.com/rogerinn/little_poc/
    ```
    ou use a opção de download.

2) Entre na pasta com
    ```bash
    cd little_poc
    ```

3) Rode a aplicação
    ```bash
    ./start.sh nome_do_arquivo.js > opcodes.txt
    ```
    Ou 
    ```bash
    ./start.sh nome_do_arquivo.js > opcodes.rnc
    ```
    para linkar os `std`.
    
Arquitetura base: <img src="https://media.discordapp.net/attachments/638203747608166442/1216137973943369749/Untitled-2024-02-27-0216.png?ex=65ff4bbf&is=65ecd6bf&hm=309be9571e9216371bd63f4aa546b9700ce5ea98ebcfca275863978a179f158e&=&format=webp&quality=lossless&width=1440&height=518">
5) Atualmente o modelo é simples, já está em refatoração para o pre-compiler JIT e AOT.
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
5) Base atual todo modelo de JIT e AOT funcionam independentes de SO, toda camada de supervisionamento é feita de forma rootless, com cgroups e namespaces melhorando isolamento, capacidade de compilação e interpretação para micro ou macros compilações e funções distribuídas.
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
