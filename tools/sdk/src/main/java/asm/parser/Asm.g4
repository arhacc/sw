/*

*/

grammar Asm;
@ header{ 
 	 package xpu.sw.tools.sdk.asm.parser;
 	 }

parse
   : (line? EOL) +
   ;

line
   : directive
   | label 
   | instruction
   | comment
   ;

instruction
   : label? instr instr comment?
   ;

instr
   : opcode value? 
   ;

label
   : lb ':'?
   ;

directive
   : assemblerdirective
   | define
   | include 
   | func
   | endfunc
   ;

assemblerdirective
   : arh
   | data
   ;

arh
   : ARH number
   ;

data
   : DATA number FILEPATH
   ;

define
   : DEFINE name expression
   ;

expression
   : multiplyingExpression (SIGN multiplyingExpression)*
   ;

multiplyingExpression
   : value (('*' | '/' | 'mod' | 'and') value)*
   ;

value
   : number
   | name
   | name COMMA number
   ;

include
   : INCLUDE FILEPATH
   ;

func
   : FUNC name
   ;

endfunc
   : ENDFUNC
   ;

lb
   : name
   ;

name
   : NAME
   ;

number
   : SIGN? (NUMBER | HEXADECIMAL)
   ;

opcode
   : OPCODE
   ;

comment
   : COMMENT
   ;

ARH
   : '.arh'
   ;

DATA
   : '.data'
   ;

FILEPATH
   : '"' ~[<"]* '"' | '\'' ~[<']* '\''
   ;

INCLUDE
   : 'include'
   ;

FUNC
   : 'func'
   ;

ENDFUNC
   : 'endfunc'
   ;

DEFINE
   : 'define'
   ;

COMMENT
   : ';' ~ ('\n' | '\r')* '\r'? '\n' -> skip
   ;

OPCODE
   : 'vadd'|'add'|'radd'|'riadd'|'cadd'|'caadd'|'cradd'|'vaddc'|'addc'|'raddc'|'riaddc'|'caddc'|'caaddc'|'craddc'|'vsub'|'sub'|'rsub'|'risub'|'csub'|'casub'|'crsub'|'vrvsub'|'rvsub'|'rrvsub'|'rirvsub'|'crvsub'|'carvsub'|'crrvsub'|'vsubc'|'subc'|'rsubc'|'risubc'|'csubc'|'casubc'|'crsubc'|'vrvsubc'|'rvsubc'|'rrvsubc'|'rirvsubc'|'crvsubc'|'carvsubc'|'crrvsubc'|'vmult'|'mult'|'rmult'|'rimult'|'cmult'|'camult'|'crmult'|'vand'|'and'|'rand'|'riand'|'cand'|'caand'|'crand'|'vor'|'or'|'ror'|'rior'|'cor'|'caor'|'cror'|'vxor'|'xor'|'rxor'|'rixor'|'cxor'|'caxor'|'crxor'|'vload'|'load'|'rload'|'riload'|'cload'|'caload'|'crload'|'ixload'|'srload'|'ioload'|'insval'|'addrld'|'store'|'rstore'|'ristore'|'cstore'|'srstore'|'iostore'|'insertio'|'compare'|'rcompare'|'ricompare'|'vcompare'|'ccompare'|'cacompare'|'crcompare'|'shrightc'|'shright'|'sharight'|'rotright'|'shr'|'shl'|'prun'|'pload'|'param'|'getv'|'sendv'|'setint'|'nop'|'jmp'|'brz'|'brnz'|'brsgn'|'brnsgn'|'brzdec'|'brnzdec'|'brbool'|'brnbool'|'brcr'|'brncr'|'halt'|'start'|'stop'|'wherezero'|'wherecarry'|'wherefirst'|'wherenext'|'whereprev'|'wherenzero'|'wherencarry'|'wherenfirst'|'wherennext'|'wherenprev'|'elsewhere'|'endwhere'|'activate'|'rednop'|'redadd'|'redmin'|'redmax'|'setval'|'waitmatw'|'resready'|'brcmpnvaldec'|'setdec'
   ;

SIGN
   : '+' | '-'
   ;

NAME
   : [.a-zA-Z] [a-zA-Z0-9."_]*
   ;

NUMBER
   : [0-9a-fA-F] + ('H' | 'h')?
   ;

HEXADECIMAL 
   : '0x' ([a-fA-F0-9_])+;

STRING
   : '\u0027' ~'\u0027'* '\u0027'
   ;

EOL
   : ('\r'? '\n' | '\r' | '\n')+
   ;

WS
   : [ \t] -> skip
   ;

COMMA
   : ','
   ;