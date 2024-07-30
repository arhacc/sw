%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <targets/sim/statelogparser/ParsedContext.hpp>
#include <targets/sim/statelogparser/Parser.gen.hpp>
#include <targets/sim/statelogparser/Lexer.gen.hpp>


void xpu_state_log_error(void * scanner, const char *s);
uint32_t xpu_state_log_parse_int(char *x);
uint32_t xpu_state_log_parse_int_bin(char *x);
uint32_t xpu_state_log_parse_int_internal(char *x, int base);

%}

%define api.prefix {xpu_state_log_}
%define api.pure full
%define api.push-pull both
%define parse.error detailed
%lex-param { void *scanner }
%parse-param { void *scanner }
%define parse.trace

%union {
    char *str;
    uint32_t num;
}

%token <str> IDENTIFIER
%token <str> NUMBER
%token START END COMMA EQUALS COLON
%token LBRACE RBRACE LBRACKET RBRACKET
%nterm <num> number bnumber

%%
input: %empty
     | input map { /* Done */ }
     ;

map: START assignments END { xpu_state_log_get_extra(scanner)->callback(*xpu_state_log_get_extra(scanner)); }
   ;

assignments: %empty
           | assignments assignment
           ;

assignment: IDENTIFIER maybevectordescription EQUALS value { xpu_state_log_get_extra(scanner)->result[$1] = xpu_state_log_get_extra(scanner)->curvalue; free($1); }
          | IDENTIFIER LBRACKET NUMBER RBRACKET EQUALS value {
              std::string name = $1;
              name += "[";
              name += $3;
              name += "]";
              xpu_state_log_get_extra(scanner)->result[name] = xpu_state_log_get_extra(scanner)->curvalue;
              free($1);
            }
          | IDENTIFIER COLON LBRACE flist RBRACE EQUALS bvalue { xpu_state_log_get_extra(scanner)->result[$1] = xpu_state_log_get_extra(scanner)->curvalue; }
          ;

maybevectordescription: %empty
                      | COLON LBRACKET flist RBRACKET

flist: IDENTIFIER
     | flist COMMA IDENTIFIER
     ;

value: number { xpu_state_log_get_extra(scanner)->curvalue = ParsedContext::Value($1); }
     | LBRACKET vector RBRACKET { xpu_state_log_get_extra(scanner)->curvalue = ParsedContext::Value(xpu_state_log_get_extra(scanner)->curvector); }
     ;

bvalue: bnumber { xpu_state_log_get_extra(scanner)->curvalue = ParsedContext::Value($1); }
      | LBRACKET bvector RBRACKET { xpu_state_log_get_extra(scanner)->curvalue = ParsedContext::Value(xpu_state_log_get_extra(scanner)->curvector); }
      ;

vector: %empty { xpu_state_log_get_extra(scanner)->curvector.clear(); }
      | vector number { xpu_state_log_get_extra(scanner)->curvector.push_back($2); }
      ;

bvector: %empty { xpu_state_log_get_extra(scanner)->curvector.clear(); }
      | vector bnumber { xpu_state_log_get_extra(scanner)->curvector.push_back($2); }
      ;

number: NUMBER {$$ = xpu_state_log_parse_int($1); free($1); }

bnumber: NUMBER {$$ = xpu_state_log_parse_int_bin($1); free($1); }
%%

void xpu_state_log_error(void *scanner, const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

uint32_t xpu_state_log_parse_int(char *x) {
  return xpu_state_log_parse_int_internal(x, 10);
}

uint32_t xpu_state_log_parse_int_bin(char *x) {
  return xpu_state_log_parse_int_internal(x, 2);
}

uint32_t xpu_state_log_parse_int_internal(char *x, int base) {
  uint32_t res = 0;
  for (int i = 0; i < strlen(x); ++i) {
    switch (x[i]) {
    case '0': case '1': case '2': case '3':
    case '4': case '5': case '6': case '7':
    case '8': case '9':
      res = res * base + (x[i] - '0');
      break;

    default:
      res = res * base;
      break;
    }
  }

  return res;
}
