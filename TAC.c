#include "tac.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

TACInstruction tac[TAC_MAX];
int tac_count = 0;
int temp_count = 0;

void emit(TacOp op, const char *arg1, const char *arg2, const char *result) {
    if (tac_count >= TAC_MAX) {
        fprintf(stderr, "TAC limit exceeded\n");
        exit(1);
    }
    tac[tac_count].op = op;
    tac[tac_count].arg1 = arg1 ? strdup(arg1) : NULL;
    tac[tac_count].arg2 = arg2 ? strdup(arg2) : NULL;
    tac[tac_count].result = result ? strdup(result) : NULL;
    tac_count++;
}

char* new_temp(void) {
    char buffer[20];
    sprintf(buffer, "t%d", temp_count++);
    return strdup(buffer);
}

void print_tac(void) {
    FILE *fp = fopen("TAC.txt", "w");
    if (!fp) {
        fprintf(stderr, "Error: Cannot open TAC.txt for writing\n");
        exit(1);
    }
    for (int i = 0; i < tac_count; i++) {
        char *op_str;
        switch(tac[i].op) {
            case TAC_DECLARE: op_str = "DECLARE"; break;  // **Fixed this line**
            case TAC_ADD:    op_str = "ADD";    break;
            case TAC_SUB:    op_str = "SUB";    break;
            case TAC_MUL:    op_str = "MUL";    break;
            case TAC_DIV:    op_str = "DIV";    break;
            case TAC_MOD:    op_str = "MOD";    break;
            case TAC_ASSIGN: op_str = "ASSIGN"; break;
            case TAC_PRINT:  op_str = "PRINT";  break;
            case TAC_TAKE:   op_str = "TAKE";   break;
            default:         op_str = "UNKNOWN";break;
        }
        fprintf(fp, "%s, %s, %s, %s\n", op_str,
                tac[i].arg1 ? tac[i].arg1 : "",
                tac[i].arg2 ? tac[i].arg2 : "",
                tac[i].result ? tac[i].result : "");
    }
    fclose(fp);
}
