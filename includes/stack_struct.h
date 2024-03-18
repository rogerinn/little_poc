#ifndef STACK_STRUCT_H
#define STACK_STRUCT_H

typedef struct SubStack {
    char *token;
} SubStack;

typedef struct Stack {
    SubStack subStack[100];
    char *block;
    int count;
} Stack;

struct Stack stack;

#endif