/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_XPU_STATE_LOG_HOME_VSERBU_WORK_XPU_SW_TOOLS_XRT_INCLUDE_TARGETS_SIM_STATELOGPARSER_PARSER_GEN_HPP_INCLUDED
# define YY_XPU_STATE_LOG_HOME_VSERBU_WORK_XPU_SW_TOOLS_XRT_INCLUDE_TARGETS_SIM_STATELOGPARSER_PARSER_GEN_HPP_INCLUDED
/* Debug traces.  */
#ifndef XPU_STATE_LOG_DEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define XPU_STATE_LOG_DEBUG 1
#  else
#   define XPU_STATE_LOG_DEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define XPU_STATE_LOG_DEBUG 1
# endif /* ! defined YYDEBUG */
#endif  /* ! defined XPU_STATE_LOG_DEBUG */
#if XPU_STATE_LOG_DEBUG
extern int xpu_state_log_debug;
#endif

/* Token kinds.  */
#ifndef XPU_STATE_LOG_TOKENTYPE
# define XPU_STATE_LOG_TOKENTYPE
  enum xpu_state_log_tokentype
  {
    XPU_STATE_LOG_EMPTY = -2,
    XPU_STATE_LOG_EOF = 0,         /* "end of file"  */
    XPU_STATE_LOG_error = 256,     /* error  */
    XPU_STATE_LOG_UNDEF = 257,     /* "invalid token"  */
    IDENTIFIER = 258,              /* IDENTIFIER  */
    NUMBER = 259,                  /* NUMBER  */
    START = 260,                   /* START  */
    END = 261,                     /* END  */
    COMMA = 262,                   /* COMMA  */
    EQUALS = 263,                  /* EQUALS  */
    COLON = 264,                   /* COLON  */
    LBRACE = 265,                  /* LBRACE  */
    RBRACE = 266,                  /* RBRACE  */
    LBRACKET = 267,                /* LBRACKET  */
    RBRACKET = 268                 /* RBRACKET  */
  };
  typedef enum xpu_state_log_tokentype xpu_state_log_token_kind_t;
#endif

/* Value type.  */
#if ! defined XPU_STATE_LOG_STYPE && ! defined XPU_STATE_LOG_STYPE_IS_DECLARED
union XPU_STATE_LOG_STYPE
{
#line 26 "/home/vserbu/work/xpu/sw/tools/xrt/src/targets/sim/statelogparser/Parser.y"

    char *str;
    uint32_t num;

#line 90 "/home/vserbu/work/xpu/sw/tools/xrt/include/targets/sim/statelogparser/Parser.gen.hpp"

};
typedef union XPU_STATE_LOG_STYPE XPU_STATE_LOG_STYPE;
# define XPU_STATE_LOG_STYPE_IS_TRIVIAL 1
# define XPU_STATE_LOG_STYPE_IS_DECLARED 1
#endif




#ifndef YYPUSH_MORE_DEFINED
# define YYPUSH_MORE_DEFINED
enum { YYPUSH_MORE = 4 };
#endif

typedef struct xpu_state_log_pstate xpu_state_log_pstate;


int xpu_state_log_parse (void *scanner);
int xpu_state_log_push_parse (xpu_state_log_pstate *ps,
                  int pushed_char, XPU_STATE_LOG_STYPE const *pushed_val, void *scanner);
int xpu_state_log_pull_parse (xpu_state_log_pstate *ps, void *scanner);
xpu_state_log_pstate *xpu_state_log_pstate_new (void);
void xpu_state_log_pstate_delete (xpu_state_log_pstate *ps);


#endif /* !YY_XPU_STATE_LOG_HOME_VSERBU_WORK_XPU_SW_TOOLS_XRT_INCLUDE_TARGETS_SIM_STATELOGPARSER_PARSER_GEN_HPP_INCLUDED  */
