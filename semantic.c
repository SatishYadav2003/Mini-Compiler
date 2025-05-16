#include "semantic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylineno;  // Use the line number from the lexer

Symbol symtab[SYMBOL_TABLE_SIZE];
int symtab_index = 0;

int lookup_symbol(const char *name) {
    for (int i = 0; i < symtab_index; i++) {
        if (strcmp(symtab[i].name, name) == 0)
            return i;
    }
    return -1;
}

void insert_symbol(const char *name, int value) {
    int index = lookup_symbol(name);
    if (index == -1) {
        symtab[symtab_index].name = strdup(name);
        symtab[symtab_index].value = value;
        symtab_index++;
    } else {
        fprintf(stderr, "Semantic Error: Variable '%s' is already declared at line %d.\n", name, yylineno);
        exit(1);
    }
}

int get_symbol_value(const char *name) {
    int index = lookup_symbol(name);
    if (index != -1)
        return symtab[index].value;
    fprintf(stderr, "Semantic Error: Variable '%s' is not declared at line %d.\n", name, yylineno);
    exit(1);
}

void set_symbol_value(const char *name, int value) {
    int index = lookup_symbol(name);
    if (index != -1) {
        symtab[index].value = value;
    } else {
        fprintf(stderr, "Semantic Error: Variable '%s' is not declared at line %d.\n", name, yylineno);
        exit(1);
    }
}
