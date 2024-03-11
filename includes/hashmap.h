#ifndef KEYWORD_OPERATIONS_H
#define KEYWORD_OPERATIONS_H
#define MAX_KEYWORD_LEN 50

typedef struct KeywordEntry {
    char keyword[MAX_KEYWORD_LEN];
    void (*fn)();
    int id;
} KeywordEntry;


void inicializarTabela(KeywordEntry t[]);
int funcaoHash(const char *chave);
void inserir(KeywordEntry t[], char *keyword, void (*fn)(), int fnId);
void inserir_sem_funcao(KeywordEntry t[], char *keyword);
void imprimir(KeywordEntry t[]);
void *busca(KeywordEntry t[], const char *chave);
int createToken(KeywordEntry t[]);

#endif 