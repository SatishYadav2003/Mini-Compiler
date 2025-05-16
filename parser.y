%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include "semantic.h"   // Semantic analysis functions (in C)
  #include "tac.h"        // TAC generation functions

  extern int yylineno;
  void yyerror(const char *s);
  int yylex(void);

  char* strip_brackets_quotes(const char* s);
%}

/* Declare types for nonterminals */
%union {
  int ival;         /* For numeric values (used for NUMBER tokens) */
  char *sval;       /* For identifiers, temporary variable names, and strings */
}

%token <ival> NUMBER
%token <sval> IDENTIFIER
%token <sval> STRING
%token INT PRINT TAKE

%left '+' '-'
%left '*' '/' '%'

/* For TAC generation, our expressions return a temporary name (char *) */
%type <sval> expr
%type <sval> statement

%%

program:
    statements { 
        /* At the end of parsing, output all generated TAC instructions to TAC.txt */
        print_tac(); 
    }
    ;

statements:
      /* empty */
    | statements statement { /* We don't use the synthesized value */ }
    ;

statement:
      /* Declaration with assignment: int a = expr; */
      INT IDENTIFIER '=' expr ';' {
          insert_symbol($2, 0);  /* Insert variable in symbol table with initial value */
          emit(TAC_ASSIGN, $4, NULL, $2);  /* Generate TAC: a = expr */
          free($2);
          free($4);
          $$ = NULL;
      }
    | /* Declaration without assignment: int a; */
      INT IDENTIFIER ';' {
        insert_symbol($2, 0);
        emit(TAC_DECLARE, $2, NULL, NULL);  /* Generate TAC for variable declaration */
        free($2);
        $$ = NULL;
      }
    | /* Assignment: a = expr; */
      IDENTIFIER '=' expr ';' {
          if (lookup_symbol($1) == -1) {
              yyerror("Variable not declared");
          }
          emit(TAC_ASSIGN, $3, NULL, $1);
          set_symbol_value($1, 0); /* (Not used in TAC generation) */
          free($1);
          free($3);
          $$ = NULL;
      }
    | /* Print statement: print identifier; */
      PRINT IDENTIFIER ';' {
          if (lookup_symbol($2) == -1) {
              yyerror("Variable not declared");
          }
          emit(TAC_PRINT, $2, NULL, NULL);
          free($2);
          $$ = NULL;
      }
    | /* Print statement: print string; */
      PRINT STRING ';' {
          char *stripped = strip_brackets_quotes($2);
          emit(TAC_PRINT, stripped, NULL, NULL);
          free(stripped);
          free($2);
          $$ = NULL;
      }
    | /* Print statement with string literal and identifier: print STRING IDENTIFIER; */
      PRINT STRING IDENTIFIER ';' {
          char *stripped = strip_brackets_quotes($2);
          /* First, print the string, then print the variable's value */
          emit(TAC_PRINT, stripped, NULL, NULL);
          emit(TAC_PRINT, $3, NULL, NULL);
          free(stripped);
          free($2);
          free($3);
          $$ = NULL;
      }
    | /* Take statement: take identifier; */
      TAKE IDENTIFIER ';' {
          if (lookup_symbol($2) == -1) {
              char error_msg[100];
              sprintf(error_msg, "Variable %s not declared for take", $2);
              yyerror(error_msg);
              exit(1);
          }
          emit(TAC_TAKE, $2, NULL, NULL);

          free($2);
          $$ = NULL;
      };

expr:
      expr '+' expr {

          char *temp = new_temp();
          emit(TAC_ADD, $1, $3, temp);
          free($1); free($3);
          $$ = temp;
      }
    | expr '-' expr {
          char *temp = new_temp();
          emit(TAC_SUB, $1, $3, temp);
          free($1); free($3);
          $$ = temp;
      }
    | expr '*' expr {
          char *temp = new_temp();
          emit(TAC_MUL, $1, $3, temp);
          free($1); free($3);
          $$ = temp;
      }
    | expr '/' expr { 
          if (atoi($3) == 0) { 
              yyerror("Division by zero"); 
              exit(1);
          } else { 
              char *temp = new_temp();
              emit(TAC_DIV, $1, $3, temp);
              free($1); free($3);
              $$ = temp;
          }
      }
    | expr '%' expr { 
          if (atoi($3) == 0) { 
              yyerror("Modulo by zero"); 
              exit(1);
          } else { 
              char *temp = new_temp();
              emit(TAC_MOD, $1, $3, temp);
              free($1); free($3);
              $$ = temp;
          }
      }
    | '(' expr ')'  { $$ = $2; }
    | IDENTIFIER    { $$ = strdup($1); free($1); } /* Return variable name as temporary reference */ 
    | NUMBER        {
          char buffer[20];
          sprintf(buffer, "%d", $1);
          char *temp = new_temp();
          emit(TAC_ASSIGN, buffer, NULL, temp);
          $$ = temp;
      }
    ;

%%

char* strip_brackets_quotes(const char* s) {
    size_t len = strlen(s);
    if (len < 4) return strdup("");
    int start = 1;
    while (start < len && (s[start] == ' ' || s[start] == '\t')) start++;
    if (s[start] != '\"') return strdup(s);
    int end = len - 2;
    while (end > 0 && (s[end] == ' ' || s[end] == '\t')) end--;
    if (s[end] != '\"') return strdup(s);
    int content_len = end - start - 1;
    char *result = (char*) malloc(content_len + 1);
    strncpy(result, s + start + 1, content_len);
    result[content_len] = '\0';
    return result;
}

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s at line %d\n", s, yylineno);
}

int main(void) {
    return yyparse();
}
