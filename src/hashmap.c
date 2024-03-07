#include <stdio.h>
#include <string.h>
#include "hashmap.h"

#define TAM 31

void aa () {
    
}

void inicializarTabela(KeywordEntry t[]) {
    int i;
    for(i = 0; i < TAM; i++)
        t[i].keyword[0] = '\0'; 
}

int funcaoHash(const char *chave) {
    int hash = 0;
    for(int i = 0; chave[i] != '\0'; i++)
        hash = (hash * 31 + chave[i]) % TAM;

    return hash;
}

void inserir(KeywordEntry t[], char *keyword, void (*fn)()) {
    KeywordEntry entrada;
    strcpy(entrada.keyword, keyword);
    entrada.fn = fn; 
    int id = funcaoHash(keyword);
    while(t[id].keyword[0] != '\0' && strcmp(t[id].keyword, keyword) != 0){
        id = (id + 1) % TAM;
    }
    strcpy(t[id].keyword, keyword);
    t[id].fn = entrada.fn; 
}

void inserir_sem_funcao(KeywordEntry t[], char *keyword) {
    KeywordEntry entrada;
    strcpy(entrada.keyword, keyword);
    entrada.fn = NULL; 
    int id = funcaoHash(keyword);
    while(t[id].keyword[0] != '\0' && strcmp(t[id].keyword, keyword) != 0){
        id = (id + 1) % TAM;
    }
    strcpy(t[id].keyword, keyword);
    t[id].fn = entrada.fn; 
}

void imprimir(KeywordEntry t[]) {
    int i;
    for(i = 0; i < TAM; i++){
        if(t[i].keyword[0] != '\0'){
            printf("%d = %s\n", i, t[i].keyword);
        }
    }
}

void *busca(KeywordEntry t[], const char *chave) {
    int id = funcaoHash(chave);
    while (t[id].keyword[0] != '\0') {
        if (strcmp(t[id].keyword, chave) == 0) {
            return t[id].fn;
        } else {
            id = (id + 1) % TAM;
        }
    }
    return NULL;
}

int createToken(KeywordEntry tabela[TAM]) {
    inicializarTabela(tabela);
    char *InitialTokens[3] = {"let", "const", "import"};
    for(int i = 0; i < 3; i++) {
        inserir(tabela, InitialTokens[i], aa);
    }
    return 0;
}