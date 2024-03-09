#ifndef KEYWORD_ENTRY_H
#define KEYWORD_ENTRY_H

#define MAX_KEYWORD_LEN 50

typedef struct KeywordEntry {
    char keyword[MAX_KEYWORD_LEN];
    void (*fn)();
    int id;
} KeywordEntry;

#endif