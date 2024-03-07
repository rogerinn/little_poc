#include <string.h>
#include <stdio.h>
#include "dot_verify.h"

int dotVerify(const char *arg) {
    const char *dot = strrchr(arg, '.');
    if (dot != NULL && strcmp(dot, ".js") == 0) {
        return 1;
    }   
    printf("Extensão inválida\n");
    return 0;
}