#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <stdlib.h>

typedef struct {
    char *name;
    int value;
} Symbol;

#define SYMBOL_TABLE_SIZE 100

extern Symbol symtab[SYMBOL_TABLE_SIZE];
extern int symtab_index;


int lookup_symbol(const char *name);
void insert_symbol(const char *name, int value);
int get_symbol_value(const char *name);
void set_symbol_value(const char *name, int value);

#endif 
