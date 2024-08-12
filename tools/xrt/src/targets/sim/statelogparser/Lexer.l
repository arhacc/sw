%{
#include <targets/sim/statelogparser/ParsedContext.hpp>
#include <targets/sim/statelogparser/Parser.gen.hpp>
#define YYSTYPE         XPU_STATE_LOG_STYPE
#include <stdio.h>
#include <stdlib.h>
#include <vector>

%}

%option noyywrap
%option reentrant
%option bison-bridge
%option extra-type="ParsedContext *"

%option prefix="xpu_state_log_"

%%
[ \t\n]+                  { /* Ignore whitespace */ }
[xX0-9]+                  { yylval->str = strdup(yytext); /* printf("Lexed NUMBER: %s\n", yytext); */ return NUMBER; }
[a-zA-Z_][a-zA-Z_0-9]*    { yylval->str = strdup(yytext); /* printf("Lexed IDENTIFIER: %s\n", yytext); */ return IDENTIFIER; }
\=                        { return EQUALS; }
\[                        { return LBRACKET; }
\]                        { return RBRACKET; }
\{                        { return LBRACE; }
\}                        { return RBRACE; }
\$\^                      { return START; }
\$\$                      { return END; }
\,                        { return COMMA; }
\:                        { return COLON; }


.             { /* Ignore unknown characters */ }

%%
