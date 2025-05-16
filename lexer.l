%{
  #include "parser.tab.h"
  #include <stdlib.h>
  #include <string.h>


%}

%option noyywrap
%option yylineno



%%

\[[ \t]*\"([^\\\"]|\\.)*\"[ \t]*\] { 
                    yylval.sval = strdup(yytext);  
                    return STRING; 
                  }
"int"       { return INT; }           
"print"     { return PRINT; }          
"take"      { return TAKE; }            
[a-zA-Z_][a-zA-Z0-9_]* { 
                    yylval.sval = strdup(yytext);  
                    return IDENTIFIER;
                 }
[0-9]+      { 
                    yylval.ival = atoi(yytext);
                    return NUMBER;
             }
"="         { return '='; }             
"+"         { return '+'; }
"-"         { return '-'; }
"*"         { return '*'; }
"/"         { return '/'; }
"%"         { return '%'; }
";"         { return ';'; }
[ \t\n]+    { /* Ignore whitespace */ }


"//".*                      { /* Ignore single-line comment */ }
"/*"(.|\n)*?"*/"            { /* Ignore multi-line comment */ }


.           { return yytext[0]; }         
%%
