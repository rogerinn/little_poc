#ifndef KEYWORD_OPERATIONS_H
#define KEYWORD_OPERATIONS_H
#include "struct_map.h" 

void inicializarTabela(KeywordEntry t[]);
int funcaoHash(const char *chave);
void inserir(KeywordEntry t[], char *keyword, void (*fn)(), int fnId);
void inserir_sem_funcao(KeywordEntry t[], char *keyword);
void imprimir(KeywordEntry t[]);
void *busca(KeywordEntry t[], const char *chave);
int createToken(KeywordEntry t[]);

#endif 