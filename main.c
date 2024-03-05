#include <stdio.h>
#include <string.h>
#include "dot_verify.c"

#define TAM 31
#define MAX_KEYWORD_LEN 10
int validationState = 1;

typedef struct {
    char keyword[MAX_KEYWORD_LEN];
} KeywordEntry;

typedef int (*PrintCallback)(const char *, KeywordEntry tabela[]);

void inicializarTabela(KeywordEntry t[]) {
    int i;
    for(i = 0; i < TAM; i++)
        t[i].keyword[0] = '\0'; 
}

int funcaoHash(const char *chave) {
    int hash = 0;
    int i;
    for(i = 0; chave[i] != '\0'; i++)
        hash = (hash * 31 + chave[i]) % TAM;

    return hash;
}

void inserir(KeywordEntry t[], char *keyword) {
    KeywordEntry entrada;
    strcpy(entrada.keyword, keyword);
    int id = funcaoHash(keyword);
    while(t[id].keyword[0] != '\0' && strcmp(t[id].keyword, keyword) != 0){
        id = (id + 1) % TAM;
    }
    strcpy(t[id].keyword, keyword);
}

void imprimir(KeywordEntry t[]) {
    int i;
    for(i = 0; i < TAM; i++){
        if(t[i].keyword[0] != '\0'){
            printf("%d = %s\n", i, t[i].keyword);
        }
    }
}

int busca(KeywordEntry t[],const char *chave) {
    int id = funcaoHash(chave);
    printf("\nIndice gerado: %d\n", id);
    while (t[id].keyword[0] != '\0') {
        if (strcmp(t[id].keyword, chave) == 0) {
            printf("Palavra chave encontrada: %s\n", chave);
            return 1;
        } else {
            id = (id + 1) % TAM;
        }
    }
    printf("Palavra chave nao encontrada: %s\n", chave);
    return 0; 
}


int printCallback(const char *str, KeywordEntry tabela[]) {
    if(validationState) {
        int search = busca(tabela, str);
        if(!search) {
            validationState = 0;
            return 0;
        }
        validationState = 0;
    }
    return 1;
}

void loadFile(const char *filename, const char *delimiters, const char *keep_delimiters, PrintCallback callback, KeywordEntry tabela[]) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    char buffer[1024];
    char *token;
    while (fgets(buffer, sizeof(buffer), file) != NULL && validationState) {
        token = strtok(buffer, delimiters);
        while (token != NULL) {
            if (strlen(token) == 1 && strchr(keep_delimiters, token[0]) != NULL) {
                callback(token, tabela); 
            } else {
                char buffer_token[strlen(token)];
                int buffer_index = 0;
                for (int i = 0; i < strlen(token); i++) {
                    if (strchr(keep_delimiters, token[i]) == NULL) {
                        buffer_token[buffer_index++] = token[i];
                    } else {
                        if (buffer_index > 0) {
                            buffer_token[buffer_index] = '\0'; 
                            callback(buffer_token, tabela); 
                            buffer_index = 0; 
                        }
                        char temp[2] = {token[i], '\0'}; 
                        callback(temp, tabela); 
                    }
                }
                if (buffer_index > 0) {
                    buffer_token[buffer_index] = '\0'; 
                    callback(buffer_token, tabela); 
                }
            }
            token = strtok(NULL, delimiters); 
        }
    }
    fclose(file);
}

int createToken(KeywordEntry tabela[TAM]) {
    inicializarTabela(tabela);
    char *InitialTokens[3] = {"let", "const", "import"};
    for(int i = 0; i < 3; i++) {
        inserir(tabela, InitialTokens[i]);
    }
    return 0;
}

int main(int argc, char *argv[]) {
    KeywordEntry tabela[TAM], entrada;
    if (argc < 2) {
        printf("Uso: %s <arquivo>\n", argv[0]);
        return 1;
    }

    int result = dotVerify(argv[1]);
    if(result) { 
        createToken(tabela);
        loadFile(argv[1], " \t\n", "\\\"\'`()<>|{}[];.=+-/*?%$&#@!_-:,~", printCallback, tabela);
    }
    return result;
}