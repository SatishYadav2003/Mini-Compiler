#ifndef TAC_H
#define TAC_H

typedef enum {
    TAC_ADD,
    TAC_SUB,
    TAC_MUL,
    TAC_DIV,
    TAC_MOD,
    TAC_ASSIGN,
    TAC_PRINT,
    TAC_TAKE,
    TAC_DECLARE
} TacOp;

typedef struct {
    TacOp op;
    char *arg1;    // First operand
    char *arg2;    // Second operand (if needed)
    char *result;  // Destination or result temporary
} TACInstruction;

#define TAC_MAX 1000

extern TACInstruction tac[TAC_MAX];
extern int tac_count;

/* Emit a TAC instruction */
void emit(TacOp op, const char *arg1, const char *arg2, const char *result);

/* Create a new temporary variable name */
char* new_temp(void);

/* Print all TAC instructions to TAC.txt */
void print_tac(void);

#endif
