/*

*/

grammar Asm;
@ header{
 	 package xpu.sw.tools.sdk.asm.parser;
}

parse
   : line+ EOF
   ;

line
   : directive
   | label
   | instruction
   | label? macroCall
   ;

directive
   : assemblerdirective
   | define
   | const
   | include
   | func
   | endfunc
   | macro
   | endmacro
   ;

instruction
   : label? controllerInstruction arrayInstruction
   ;

controllerInstruction
   : controllerOpcode0
   | controllerOpcode1 expression
   | controllerOpcode2 lb
   | controllerOpcode3 lb COMMA expression
   ;

controllerOpcode0
   :
   'cadd'|'sadd'|'cradd'|'caddc'|'saddc'|'craddc'|'smult'|'cmult'|'crmult'|'ssub'|'csub'|'crsub'|'ssubc'|'csubc'|'crsubc'|'srvsub'|'crvsub'|'crrvsub'|'srvsubc'|'crvsubc'|'crrvsubc'|'scompare'|'ccompare'|'crcompare'|'sand'|'cand'|'crand'|'sor'|'cor'|'cror'|'sxor'|'cxor'|'crxor'|'nop'|'crstore'|'cstack_store_pop'|'crstack_store_pop'|'crswap_acc_mem'|'sload'|'cload'|'cstack_push_load'|'sstack_push_load'|'crstack_push_load'|'shright'|'shrightc'|'sharight'|'shright_fixed_amount'|'sharight_fixed_amount'|'grshift_wob'|'grshift_wbz'|'grshift_wbh'|'crightins'|'cleftins'|'right_redins'|'left_redins'|'glshift_wob'|'glshift_wbz'|'glshift_wbh'|'rotateright'|'rotateleft'|'halt'|'cc_start_w_halt'|'cc_start_wo_halt'|'cc_stop'|'cc_reset'|'srstore'|'addrstore'|'setdec'|'addrload'|'stack_pop'|'stack_duplicate'|'stack_over'|'stack_swap'|'stack_load_layer1'|'param'|'setint' | 'reset_wait_transfer_wa_counter' | 'reset_wait_transfer_ra_counter' | 'reset_wait_transfer_wc_counter' | 'reset_wait_transfer_rc_counter' | 'reset_wait_transfer_all_counters' | 'allow_transfer_reset_counter_wa' | 'allow_transfer_reset_counter_ra' | 'allow_transfer_reset_counter_wc' | 'allow_transfer_reset_counter_rc' | 'allow_transfer_reset_counter_all'
   ;

controllerOpcode1
   :
   'vadd'|'add'|'radd'|'riadd'|'vaddc'|'addc'|'raddc'|'riaddc'|'vmult'|'mult'|'rmult'|'rimult'|'vsub'|'sub'|'rsub'|'risub'|'vsubc'|'subc'|'rsubc'|'risubc'|'vrvsub'|'rvsub'|'rrvsub'|'rirvsub'|'vrvsubc'|'rvsubc'|'rrvsubc'|'rirvsubc'|'vcompare'|'compare'|'rcompare'|'ricompare'|'vand'|'and'|'rand'|'riand'|'vor'|'or'|'ror'|'rior'|'vxor'|'xor'|'rxor'|'rixor'|'fadd'|'fradd'|'fsub'|'frsub'|'fmult'|'frmult'|'frtmult'|'rfrtmult'|'scdmult'|'rscdmult'|'fdiv'|'frdiv'|'loop1'|'rloop1'|'loop2'|'rloop2'|'addrinc'|'store'|'rstore'|'ristore'|'sstore'|'stack_store_pop'|'rstack_store_pop'|'ristack_store_pop'|'swap_acc_mem'|'rswap_acc_mem'|'riswap_acc_mem'|'vload'|'load'|'rload'|'riload'|'vstack_push_load'|'stack_push_load'|'rstack_push_load'|'ristack_push_load'|'send'|'rsend'|'risend'|'csend'|'ssend'|'crsend'|'insval'|'rotright_local'|'rotleft_local'|'shift_right'|'shift_left'|'vleftins'|'float'|'sel_addrreg'|'setval' | 'allow_transfer_wa' | 'allow_transfer_ra' | 'allow_transfer_wc' | 'allow_transfer_rc' | 'wait_transfer_wa' | 'wait_transfer_ra' | 'wait_transfer_wc' | 'wait_transfer_rc'
   ;

controllerOpcode2
   :
   'jmp'|'brz'|'brnz'|'brsgn'|'brnsgn'|'brzdec'|'brnzdec'|'brbool'|'brnbool'|'brcr'|'brncr'|'brcr_delayed'|'brncr_delayed'
   ;

controllerOpcode3
   :
   'brvalz'|'brvalnz'|'brvalsgn'|'brvalnsgn'|'brvalzdec'|'brvalnzdec'|'brcmpval'|'brcmpnval'|'brcmpvaldec'|'brcmpnvaldec'
   ;

arrayInstruction
   : arrayOpcode0
   | arrayOpcode1 expression
   ;

arrayOpcode0
   :
   'cadd'|'sadd'|'cradd'|'caddc'|'saddc'|'craddc'|'cmult'|'smult'|'crmult'|'csub'|'ssub'|'crsub'|'csubc'|'ssubc'|'crsubc'|'crvsub'|'srvsub'|'crrvsub'|'crvsubc'|'srvsubc'|'crrvsubc'|'ccompare'|'scompare'|'crcompare'|'cand'|'sand'|'crand'|'cor'|'sor'|'cror'|'cxor'|'sxor'|'crxor'|'caddrinc'|'nop'|'cstore'|'crstore'|'cstack_store_pop'|'crstack_store_pop'|'crswap_acc_mem'|'cload'|'sload'|'crload'|'cstack_push_load'|'sstack_push_load'|'crstack_push_load'|'srcall'|'search'|'csearch'|'selshift'|'delete'|'shright'|'shrightc'|'sharight'|'shright_fixed_amount'|'sharight_fixed_amount'|'wherezero'|'wherepositive'|'wherecarry'|'wherefirst'|'wherenext'|'whereprev'|'wherenzero'|'wherenegative'|'wherencarry'|'wherenfirst'|'wherennext'|'wherenprev'|'wheresgn'|'wherensgn'|'wherelast'|'wherenlast'|'whereafterfirst'|'wherenafterfirst'|'wherebeforelast'|'wherenbeforelast'|'elsewhere'|'endwhere'|'activate'|'scannop'|'scanadd'|'scanmin'|'scanmax'|'scanprefix_add'|'scanprefix_bitwise_xor'|'scansub'|'scan_bitwise_or'|'scan_bitwise_xor'|'scan_bitwise_and'|'addrinc_acc'|'addr_reg_stack_duplicate'|'addr_reg_stack_pop'|'addrstore'|'caddrstore'|'insertio'|'ixload'|'srload'|'scanload'|'stack_pop'|'stack_duplicate'|'stack_over'|'stack_swap'|'stack_load_layer1'
   ;

arrayOpcode1
   :
   'vadd'|'add'|'radd'|'riadd'|'vaddc'|'addc'|'raddc'|'riaddc'|'vmult'|'mult'|'rmult'|'rimult'|'vsub'|'sub'|'rsub'|'risub'|'vsubc'|'subc'|'rsubc'|'risubc'|'vrvsub'|'rvsub'|'rrvsub'|'rirvsub'|'vrvsubc'|'rvsubc'|'rrvsubc'|'rirvsubc'|'vcompare'|'compare'|'rcompare'|'ricompare'|'vand'|'and'|'rand'|'riand'|'vor'|'or'|'ror'|'rior'|'vxor'|'xor'|'rxor'|'rixor'|'fadd'|'fradd'|'fsub'|'frsub'|'fmult'|'frmult'|'frtmult'|'rfrtmult'|'scdmult'|'rscdmult'|'scdmult'|'rscdmult'|'fdiv'|'frdiv'|'loop1'|'rloop1'|'loop2'|'rloop2'|'addrinc'|'store'|'rstore'|'ristore'|'sstore'|'stack_store_pop'|'rstack_store_pop'|'ristack_store_pop'|'swap_acc_mem'|'rswap_acc_mem'|'riswap_acc_mem'|'vload'|'load'|'rload'|'riload'|'vstack_push_load'|'stack_push_load'|'rstack_push_load'|'ristack_push_load'|'vsrcall'|'vsearch'|'vcsearch'|'rotright_local'|'rotleft_local'|'shift_right'|'shift_left'|'scansplit'|'scanpermute'|'float'
   ;

label
   : lb ':'
   ;

assemblerdirective
   : architectureId
   | data
   ;

architectureId
   : ARCHITECTUREID name
   ;

data
   : DATA HEX1 FILEPATH
   ;

define
   : DEFINE name expression
   ;

const
   : CONST name expression
   ;

expression
   : multiplyingExpression ((PLUS | MINUS) multiplyingExpression)*
   ;

multiplyingExpression
   : signedAtom ((TIMES | DIV) signedAtom)*
   ;

signedAtom
   : PLUS signedAtom
   | MINUS signedAtom
   | function
   | atom
   ;

atom
   : number
   | name
   | LPAREN expression RPAREN
   ;

function
   : funcname LPAREN expression RPAREN
   ;

funcname
   : LOG2
   | SQRT
   | CEIL 
   | FLOOR 
   | ROUND 
   ;

LOG2
   : 'log2'
   ;

SQRT
   : 'sqrt'
   ;

CEIL
   : 'ceil'
   ;

FLOOR
   : 'floor'
   ;

ROUND
   : 'round'
   ;


LPAREN
   : '('
   ;


RPAREN
   : ')'
   ;


PLUS
   : '+'
   ;


MINUS
   : '-'
   ;


TIMES
   : '*'
   ;


DIV
   : '/'
   ;


POINT
   : '.'
   ;


POW
   : '^'
   ;

PI
   : 'pi'
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

macro
   : MACRO name '(' parametersNames ')'
   ;

endmacro
   : ENDMACRO
   ;

parametersNames
   : name? (',' name)*
   ;

macroCall
   : name '(' parametersInstantiation ')'
   ;

parametersInstantiation
   : expression? (',' expression)*
   ;

lb
   : name
   ;

name
   : NAME
   ;

number
   : FLOAT
   | DEC
   | HEX1
   | HEX2
   | BIN
   ;

comment
   : COMMENT
   ;

ARCHITECTUREID
   : 'architectureId'
   ;

DATA
   : 'data'
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

MACRO
   : 'macro'
   ;

ENDMACRO
   : 'endmacro'
   ;

DEFINE
   : 'define'
   ;

CONST
   : 'const'
   ;

SIGN
   : '+' | '-'
   ;

NAME
   : [$_a-zA-Z] [a-zA-Z0-9_]*
   ;

FLOAT   
   : DEC '.' DEC+
   ;

DEC
   : [0-9]+
   ;

HEX1
   : '0x' ([a-fA-F0-9_])+
   ;

HEX2
   : ([a-fA-F0-9_])+ [hH]
   ;

BIN
   : '0' [bB] [01]+
   ;

STRING
   : '\'' ~'\''* '\''
   ;

fragment Tab
   : '\t'
   ;

fragment Space
   : ' '
   ;

TS
   : (Tab|Space)+ -> skip
   ;

EOL
   : ('\r'? '\n')+ ->skip
   ;

COMMA
   : ','
   ;

COMMENT
   :';' ~[\n\r]* ('\r'? '\n')+ -> skip
   ;
