#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_VARS 256
#define MAX_VAR_LENGTH 64
#define MAX_LINES 1024
#define MAX_LINE_LENGTH 256

// Global array to keep track of declared non-temporary variables.
char declared_vars[MAX_VARS][MAX_VAR_LENGTH];
int declared_var_count = 0;

// Structure for mapping a temporary variable to its arithmetic expression.
typedef struct {
    char temp[MAX_VAR_LENGTH];
    char expression[512];
} TempMapping;

TempMapping tempMappings[MAX_LINES];
int tempMappingCount = 0;

// Check if a variable has already been declared.
int is_declared(const char *var) {
    for (int i = 0; i < declared_var_count; i++) {
        if (strcmp(declared_vars[i], var) == 0) {
            return 1;
        }
    }
    return 0;
}

// Add a variable to the declaration list if not already present.
void add_declaration(const char *var) {
    if (!is_declared(var)) {
        strncpy(declared_vars[declared_var_count], var, MAX_VAR_LENGTH - 1);
        declared_vars[declared_var_count][MAX_VAR_LENGTH - 1] = '\0';
        declared_var_count++;
    }
}

// Check if a token is a valid C identifier.
int is_valid_identifier(const char *token) {
    if (!token || !token[0])
        return 0;
    if (!(isalpha(token[0]) || token[0]=='_')) return 0;
    for (int i = 1; token[i] != '\0'; i++) {
        if (!(isalnum(token[i]) || token[i]=='_')) return 0;
    }
    return 1;
}

// Check if an identifier is a temporary variable (e.g. t0, t1, etc.).
int is_temp(const char *var) {
    if(var[0]=='t') {
        for (int i = 1; var[i] != '\0'; i++) {
            if (!isdigit(var[i]))
                return 0;
        }
        return 1;
    }
    return 0;
}

// Simple trim function (trims leading and trailing whitespace).
void trim(char *str) {
    while(isspace((unsigned char)*str)) str++;
    char *end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) {
        *end = '\0';
        end--;
    }
}

int main() {
    FILE *fin = fopen("TAC.txt", "r");
    if (!fin) {
        perror("Error opening TAC.txt file");
        return 1;
    }
    
    FILE *fout = fopen("output.c", "w");
    if (!fout) {
        perror("Error opening output.c file");
        fclose(fin);
        return 1;
    }
    
    // Buffer to hold the generated C code lines (excluding declarations).
    char *code_lines[MAX_LINES];
    int code_line_count = 0;
    
    char line[MAX_LINE_LENGTH];
    while(fgets(line, sizeof(line), fin)) {
        // Remove newline and skip empty lines.
        line[strcspn(line, "\n")] = '\0';
        if(strlen(line) == 0)
            continue;
            
        // Make a copy for tokenization.
        char line_copy[MAX_LINE_LENGTH];
        strncpy(line_copy, line, MAX_LINE_LENGTH);
        
        // Tokenize the TAC line. Expected format: OPERATION, arg1, arg2, result
        char *token;
        char *tokens[4] = {NULL, NULL, NULL, NULL};
        int token_index = 0;
        token = strtok(line_copy, ",");
        while(token != NULL && token_index < 4) {
            // Trim leading/trailing spaces.
            while(*token == ' ') token++;
            char *end = token + strlen(token) - 1;
            while(end > token && (isspace((unsigned char)*end))) {
                *end = '\0';
                end--;
            }
            tokens[token_index++] = token;
            token = strtok(NULL, ",");
        }
        
        if(tokens[0] == NULL)
            continue;
        
        // --- DECLARE: Record non-temporary variable.
        if(strcmp(tokens[0], "DECLARE") == 0) {
            if(tokens[1] && strlen(tokens[1]) > 0) {
                // Do not add temporary variables to declarations.
                if(!is_temp(tokens[1]))
                    add_declaration(tokens[1]);
            }
        }
        // --- PRINT: Generate printf statement.
        else if(strcmp(tokens[0], "PRINT") == 0) {
            if(tokens[1] && strlen(tokens[1]) > 0) {
                char buffer[512];
                // If the token is a declared variable, print its value as integer.
                if(is_valid_identifier(tokens[1]) && is_declared(tokens[1])) {
                    snprintf(buffer, sizeof(buffer), "printf(\"%%d\", %s);", tokens[1]);
                } else {
                    // Otherwise, treat it as a literal string.
                    snprintf(buffer, sizeof(buffer), "printf(\"%%s\", \"%s\");", tokens[1]);
                }
                code_lines[code_line_count] = strdup(buffer);
                code_line_count++;
            }
        }
        // --- TAKE: Generate scanf statement.
        else if(strcmp(tokens[0], "TAKE") == 0) {
            if(tokens[1] && strlen(tokens[1]) > 0) {
                // Ensure the variable is declared if not temporary.
                if(is_valid_identifier(tokens[1]) && !is_temp(tokens[1]) && !is_declared(tokens[1]))
                    add_declaration(tokens[1]);
                char buffer[512];
                snprintf(buffer, sizeof(buffer), "scanf(\"%%d\", &%s);", tokens[1]);
                code_lines[code_line_count] = strdup(buffer);
                code_line_count++;
            }
        }
        // --- Arithmetic operations: ADD, SUB, MUL, DIV ---
        else if(strcmp(tokens[0], "ADD") == 0 || strcmp(tokens[0], "SUB") == 0 ||
                strcmp(tokens[0], "MUL") == 0 || strcmp(tokens[0], "DIV") == 0) {
            char op;
            if(strcmp(tokens[0], "ADD") == 0)
                op = '+';
            else if(strcmp(tokens[0], "SUB") == 0)
                op = '-';
            else if(strcmp(tokens[0], "MUL") == 0)
                op = '*';
            else if(strcmp(tokens[0], "DIV") == 0)
                op = '/';
            
            // Ensure operands are declared if they are non-temporary.
            if(tokens[1] && is_valid_identifier(tokens[1]) && !is_temp(tokens[1]) && !is_declared(tokens[1]))
                add_declaration(tokens[1]);
            if(tokens[2] && is_valid_identifier(tokens[2]) && !is_temp(tokens[2]) && !is_declared(tokens[2]))
                add_declaration(tokens[2]);
            
            // Check if the target is temporary.
            if(tokens[3] && is_valid_identifier(tokens[3]) && is_temp(tokens[3])) {
                // Instead of outputting a separate line, store the arithmetic expression.
                char expr[512];
                snprintf(expr, sizeof(expr), "%s %c %s", tokens[1], op, tokens[2]);
                strncpy(tempMappings[tempMappingCount].temp, tokens[3], MAX_VAR_LENGTH - 1);
                tempMappings[tempMappingCount].temp[MAX_VAR_LENGTH - 1] = '\0';
                strncpy(tempMappings[tempMappingCount].expression, expr, sizeof(tempMappings[tempMappingCount].expression) - 1);
                tempMappings[tempMappingCount].expression[sizeof(tempMappings[tempMappingCount].expression) - 1] = '\0';
                tempMappingCount++;
            } else if(tokens[3] && is_valid_identifier(tokens[3])) {
                // For non-temporary target, output the arithmetic operation normally.
                // Also ensure the target variable is declared.
                if(!is_temp(tokens[3]) && !is_declared(tokens[3]))
                    add_declaration(tokens[3]);
                char buffer[512];
                snprintf(buffer, sizeof(buffer), "%s = %s %c %s;", tokens[3], tokens[1], op, tokens[2]);
                code_lines[code_line_count] = strdup(buffer);
                code_line_count++;
            }
        }
        // --- ASSIGN: Simple assignment from source to target.
        else if(strcmp(tokens[0], "ASSIGN") == 0) {
            if(tokens[1] && tokens[3] && strlen(tokens[1]) > 0 && strlen(tokens[3]) > 0) {
                // Ensure the target variable is declared if not temporary.
                if(is_valid_identifier(tokens[3]) && !is_temp(tokens[3]) && !is_declared(tokens[3]))
                    add_declaration(tokens[3]);
                
                // If the source is a temporary, check if we have an expression for it.
                if(is_valid_identifier(tokens[1]) && is_temp(tokens[1])) {
                    int found = 0;
                    for (int i = 0; i < tempMappingCount; i++) {
                        if(strcmp(tempMappings[i].temp, tokens[1]) == 0) {
                            char buffer[512];
                            snprintf(buffer, sizeof(buffer), "%s = %s;", tokens[3], tempMappings[i].expression);
                            code_lines[code_line_count] = strdup(buffer);
                            code_line_count++;
                            found = 1;
                            break;
                        }
                    }
                    if(!found) { // Fallback if mapping not found.
                        char buffer[512];
                        snprintf(buffer, sizeof(buffer), "%s = %s;", tokens[3], tokens[1]);
                        code_lines[code_line_count] = strdup(buffer);
                        code_line_count++;
                    }
                } else {
                    // Otherwise, output a normal assignment.
                    char buffer[512];
                    snprintf(buffer, sizeof(buffer), "%s = %s;", tokens[3], tokens[1]);
                    code_lines[code_line_count] = strdup(buffer);
                    code_line_count++;
                }
            }
        }
        // --- (Additional TAC operations can be added here if needed) ---
    }
    
    // Write the final C code to the output file.
    fprintf(fout, "#include <stdio.h>\n#include <stdlib.h>\n\n");
    fprintf(fout, "int main() {\n");
    
    // Output declarations for non-temporary variables.
    for (int i = 0; i < declared_var_count; i++) {
        fprintf(fout, "    int %s;\n", declared_vars[i]);
    }
    fprintf(fout, "\n");
    
    // Output the generated code lines.
    for (int i = 0; i < code_line_count; i++) {
        fprintf(fout, "    %s\n", code_lines[i]);
        free(code_lines[i]);
    }
    
    fprintf(fout, "    return 0;\n");
    fprintf(fout, "}\n");
    
    fclose(fin);
    fclose(fout);
    
    return 0;
}


