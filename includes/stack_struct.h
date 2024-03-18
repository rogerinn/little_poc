#ifndef STACK_STRUCT_H
#define STACK_STRUCT_H
#define MAX_STRUCT 100

typedef struct StackItem {
    char *token;
} StackItem;

typedef struct Stack {
    StackItem subStack[MAX_STRUCT];
    char *block;
    int idBlock;
    struct Stack *subBlock[MAX_STRUCT];
    int count;
} Stack;

struct Stack stack;

#endif