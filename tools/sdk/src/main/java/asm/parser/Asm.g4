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
   : label? controlInstruction arrayInstruction comment?
   ;

controlInstruction
   : controlOpcode0
   | controlOpcode1 value
   | controlOpcode2 value COMMA value 
   | controlOpcode3 lb
   | controlOpcode4 lb COMMA value 
   ;

controlOpcode0
   : 'riadd'|'cadd'|'caadd'|'cradd'|'vaddc'|'addc'|'raddc'|'riaddc'|'caddc'|'caaddc'|'craddc'|'vsub'|'sub'|'rsub'|'risub'|'csub'|'casub'|'crsub'|'vrvsub'|'rvsub'|'rrvsub'|'rirvsub'|'crvsub'|'carvsub'|'crrvsub'|'vsubc'|'subc'|'rsubc'|'risubc'|'csubc'|'casubc'|'crsubc'|'vrvsubc'|'rvsubc'|'rrvsubc'|'rirvsubc'|'crvsubc'|'carvsubc'|'crrvsubc'|'vmult'|'mult'|'rmult'|'rimult'|'cmult'|'camult'|'crmult'|'vand'|'and'|'rand'|'riand'|'cand'|'caand'|'crand'|'vor'|'or'|'ror'|'rior'|'cor'|'caor'|'cror'|'vxor'|'xor'|'rxor'|'rixor'|'cxor'|'caxor'|'crxor'|'vload'|'load'|'rload'|'riload'|'cload'|'caload'|'crload'|'ixload'|'srload'|'ioload'|'insval'|'addrld'|'store'|'rstore'|'ristore'|'cstore'|'srstore'|'iostore'|'insertio'|'compare'|'rcompare'|'ricompare'|'vcompare'|'ccompare'|'cacompare'|'crcompare'|'shrightc'|'shright'|'sharight'|'rotright'|'shr'|'shl'|'prun'|'pload'|'param'|'getv'|'sendv'|'setint'|'nop'|'jmp'|'brz'|'brnz'|'brsgn'|'brnsgn'|'brzdec'|'brnzdec'|'brbool'|'brnbool'|'brcr'|'brncr'|'halt'|'start'|'stop'|'wherezero'|'wherecarry'|'wherefirst'|'wherenext'|'whereprev'|'wherenzero'|'wherencarry'|'wherenfirst'|'wherennext'|'wherenprev'|'elsewhere'|'endwhere'|'activate'|'rednop'|'redadd'|'redmin'|'redmax'|'setval'|'waitmatw'|'resready'|'brcmpnvaldec'|'setdec'|'addrinc'|'caddrinc'|'addrinc_acc'|'addr_reg_stack_duplicate'|'addr_reg_stack_pop'
   ;

controlOpcode1
   : 'vadd'|'add'|'radd'|'riadd'|'cadd'|'caadd'|'cradd'|'vaddc'|'addc'|'raddc'|'riaddc'|'caddc'|'caaddc'|'craddc'|'vsub'|'sub'|'rsub'|'risub'|'csub'|'casub'|'crsub'|'vrvsub'|'rvsub'|'rrvsub'|'rirvsub'|'crvsub'|'carvsub'|'crrvsub'|'vsubc'|'subc'|'rsubc'|'risubc'|'csubc'|'casubc'|'crsubc'|'vrvsubc'|'rvsubc'|'rrvsubc'|'rirvsubc'|'crvsubc'|'carvsubc'|'crrvsubc'|'vmult'|'mult'|'rmult'|'rimult'|'cmult'|'camult'|'crmult'|'vand'|'and'|'rand'|'riand'|'cand'|'caand'|'crand'|'vor'|'or'|'ror'|'rior'|'cor'|'caor'|'cror'|'vxor'|'xor'|'rxor'|'rixor'|'cxor'|'caxor'|'crxor'|'vload'|'load'|'rload'|'riload'|'cload'|'caload'|'crload'|'ixload'|'srload'|'ioload'|'insval'|'addrld'|'store'|'rstore'|'ristore'|'cstore'|'srstore'|'iostore'|'insertio'|'compare'|'rcompare'|'ricompare'|'vcompare'|'ccompare'|'cacompare'|'crcompare'|'shrightc'|'shright'|'sharight'|'rotright'|'shr'|'shl'|'prun'|'pload'|'param'|'getv'|'sendv'|'setint'|'nop'|'jmp'|'brz'|'brnz'|'brsgn'|'brnsgn'|'brzdec'|'brnzdec'|'brbool'|'brnbool'|'brcr'|'brncr'|'halt'|'start'|'stop'|'wherezero'|'wherecarry'|'wherefirst'|'wherenext'|'whereprev'|'wherenzero'|'wherencarry'|'wherenfirst'|'wherennext'|'wherenprev'|'elsewhere'|'endwhere'|'activate'|'rednop'|'redadd'|'redmin'|'redmax'|'setval'|'waitmatw'|'resready'|'brcmpnvaldec'|'setdec'|'addrinc'|'caddrinc'|'addrinc_acc'|'addr_reg_stack_duplicate'|'addr_reg_stack_pop'
   ;

controlOpcode2
   : 'vadd'|'add'|'radd'|'riadd'|'cadd'|'caadd'|'cradd'|'vaddc'|'addc'|'raddc'|'riaddc'|'caddc'|'caaddc'|'craddc'|'vsub'|'sub'|'rsub'|'risub'|'csub'|'casub'|'crsub'|'vrvsub'|'rvsub'|'rrvsub'|'rirvsub'|'crvsub'|'carvsub'|'crrvsub'|'vsubc'|'subc'|'rsubc'|'risubc'|'csubc'|'casubc'|'crsubc'|'vrvsubc'|'rvsubc'|'rrvsubc'|'rirvsubc'|'crvsubc'|'carvsubc'|'crrvsubc'|'vmult'|'mult'|'rmult'|'rimult'|'cmult'|'camult'|'crmult'|'vand'|'and'|'rand'|'riand'|'cand'|'caand'|'crand'|'vor'|'or'|'ror'|'rior'|'cor'|'caor'|'cror'|'vxor'|'xor'|'rxor'|'rixor'|'cxor'|'caxor'|'crxor'|'vload'|'load'|'rload'|'riload'|'cload'|'caload'|'crload'|'ixload'|'srload'|'ioload'|'insval'|'addrld'|'store'|'rstore'|'ristore'|'cstore'|'srstore'|'iostore'|'insertio'|'compare'|'rcompare'|'ricompare'|'vcompare'|'ccompare'|'cacompare'|'crcompare'|'shrightc'|'shright'|'sharight'|'rotright'|'shr'|'shl'|'prun'|'pload'|'param'|'getv'|'sendv'|'setint'|'nop'|'jmp'|'brz'|'brnz'|'brsgn'|'brnsgn'|'brzdec'|'brnzdec'|'brbool'|'brnbool'|'brcr'|'brncr'|'halt'|'start'|'stop'|'wherezero'|'wherecarry'|'wherefirst'|'wherenext'|'whereprev'|'wherenzero'|'wherencarry'|'wherenfirst'|'wherennext'|'wherenprev'|'elsewhere'|'endwhere'|'activate'|'rednop'|'redadd'|'redmin'|'redmax'|'setval'|'waitmatw'|'resready'|'brcmpnvaldec'|'setdec'|'addrinc'|'caddrinc'|'addrinc_acc'|'addr_reg_stack_duplicate'|'addr_reg_stack_pop'
   ;

controlOpcode3
   : 'vadd'|'add'|'radd'|'riadd'|'cadd'|'caadd'|'cradd'|'vaddc'|'addc'|'raddc'|'riaddc'|'caddc'|'caaddc'|'craddc'|'vsub'|'sub'|'rsub'|'risub'|'csub'|'casub'|'crsub'|'vrvsub'|'rvsub'|'rrvsub'|'rirvsub'|'crvsub'|'carvsub'|'crrvsub'|'vsubc'|'subc'|'rsubc'|'risubc'|'csubc'|'casubc'|'crsubc'|'vrvsubc'|'rvsubc'|'rrvsubc'|'rirvsubc'|'crvsubc'|'carvsubc'|'crrvsubc'|'vmult'|'mult'|'rmult'|'rimult'|'cmult'|'camult'|'crmult'|'vand'|'and'|'rand'|'riand'|'cand'|'caand'|'crand'|'vor'|'or'|'ror'|'rior'|'cor'|'caor'|'cror'|'vxor'|'xor'|'rxor'|'rixor'|'cxor'|'caxor'|'crxor'|'vload'|'load'|'rload'|'riload'|'cload'|'caload'|'crload'|'ixload'|'srload'|'ioload'|'insval'|'addrld'|'store'|'rstore'|'ristore'|'cstore'|'srstore'|'iostore'|'insertio'|'compare'|'rcompare'|'ricompare'|'vcompare'|'ccompare'|'cacompare'|'crcompare'|'shrightc'|'shright'|'sharight'|'rotright'|'shr'|'shl'|'prun'|'pload'|'param'|'getv'|'sendv'|'setint'|'nop'|'jmp'|'brz'|'brnz'|'brsgn'|'brnsgn'|'brzdec'|'brnzdec'|'brbool'|'brnbool'|'brcr'|'brncr'|'halt'|'start'|'stop'|'wherezero'|'wherecarry'|'wherefirst'|'wherenext'|'whereprev'|'wherenzero'|'wherencarry'|'wherenfirst'|'wherennext'|'wherenprev'|'elsewhere'|'endwhere'|'activate'|'rednop'|'redadd'|'redmin'|'redmax'|'setval'|'waitmatw'|'resready'|'brcmpnvaldec'|'setdec'|'addrinc'|'caddrinc'|'addrinc_acc'|'addr_reg_stack_duplicate'|'addr_reg_stack_pop'
   ;

controlOpcode4
   : 'vadd'|'add'|'radd'|'riadd'|'cadd'|'caadd'|'cradd'|'vaddc'|'addc'|'raddc'|'riaddc'|'caddc'|'caaddc'|'craddc'|'vsub'|'sub'|'rsub'|'risub'|'csub'|'casub'|'crsub'|'vrvsub'|'rvsub'|'rrvsub'|'rirvsub'|'crvsub'|'carvsub'|'crrvsub'|'vsubc'|'subc'|'rsubc'|'risubc'|'csubc'|'casubc'|'crsubc'|'vrvsubc'|'rvsubc'|'rrvsubc'|'rirvsubc'|'crvsubc'|'carvsubc'|'crrvsubc'|'vmult'|'mult'|'rmult'|'rimult'|'cmult'|'camult'|'crmult'|'vand'|'and'|'rand'|'riand'|'cand'|'caand'|'crand'|'vor'|'or'|'ror'|'rior'|'cor'|'caor'|'cror'|'vxor'|'xor'|'rxor'|'rixor'|'cxor'|'caxor'|'crxor'|'vload'|'load'|'rload'|'riload'|'cload'|'caload'|'crload'|'ixload'|'srload'|'ioload'|'insval'|'addrld'|'store'|'rstore'|'ristore'|'cstore'|'srstore'|'iostore'|'insertio'|'compare'|'rcompare'|'ricompare'|'vcompare'|'ccompare'|'cacompare'|'crcompare'|'shrightc'|'shright'|'sharight'|'rotright'|'shr'|'shl'|'prun'|'pload'|'param'|'getv'|'sendv'|'setint'|'nop'|'jmp'|'brz'|'brnz'|'brsgn'|'brnsgn'|'brzdec'|'brnzdec'|'brbool'|'brnbool'|'brcr'|'brncr'|'halt'|'start'|'stop'|'wherezero'|'wherecarry'|'wherefirst'|'wherenext'|'whereprev'|'wherenzero'|'wherencarry'|'wherenfirst'|'wherennext'|'wherenprev'|'elsewhere'|'endwhere'|'activate'|'rednop'|'redadd'|'redmin'|'redmax'|'setval'|'waitmatw'|'resready'|'brcmpnvaldec'|'setdec'|'addrinc'|'caddrinc'|'addrinc_acc'|'addr_reg_stack_duplicate'|'addr_reg_stack_pop'
   ;


arrayInstruction
   : arrayOpcode0
   | arrayOpcode1 value
   | arrayOpcode2 value COMMA value 
   | arrayOpcode3 lb
   | arrayOpcode4 lb COMMA value 
   ;

arrayOpcode0
   : 'riadd'|'cadd'|'caadd'|'cradd'|'vaddc'|'addc'|'raddc'|'riaddc'|'caddc'|'caaddc'|'craddc'|'vsub'|'sub'|'rsub'|'risub'|'csub'|'casub'|'crsub'|'vrvsub'|'rvsub'|'rrvsub'|'rirvsub'|'crvsub'|'carvsub'|'crrvsub'|'vsubc'|'subc'|'rsubc'|'risubc'|'csubc'|'casubc'|'crsubc'|'vrvsubc'|'rvsubc'|'rrvsubc'|'rirvsubc'|'crvsubc'|'carvsubc'|'crrvsubc'|'vmult'|'mult'|'rmult'|'rimult'|'cmult'|'camult'|'crmult'|'vand'|'and'|'rand'|'riand'|'cand'|'caand'|'crand'|'vor'|'or'|'ror'|'rior'|'cor'|'caor'|'cror'|'vxor'|'xor'|'rxor'|'rixor'|'cxor'|'caxor'|'crxor'|'vload'|'load'|'rload'|'riload'|'cload'|'caload'|'crload'|'ixload'|'srload'|'ioload'|'insval'|'addrld'|'store'|'rstore'|'ristore'|'cstore'|'srstore'|'iostore'|'insertio'|'compare'|'rcompare'|'ricompare'|'vcompare'|'ccompare'|'cacompare'|'crcompare'|'shrightc'|'shright'|'sharight'|'rotright'|'shr'|'shl'|'prun'|'pload'|'param'|'getv'|'sendv'|'setint'|'nop'|'jmp'|'brz'|'brnz'|'brsgn'|'brnsgn'|'brzdec'|'brnzdec'|'brbool'|'brnbool'|'brcr'|'brncr'|'halt'|'start'|'stop'|'wherezero'|'wherecarry'|'wherefirst'|'wherenext'|'whereprev'|'wherenzero'|'wherencarry'|'wherenfirst'|'wherennext'|'wherenprev'|'elsewhere'|'endwhere'|'activate'|'rednop'|'redadd'|'redmin'|'redmax'|'setval'|'waitmatw'|'resready'|'brcmpnvaldec'|'setdec'|'addrinc'|'caddrinc'|'addrinc_acc'|'addr_reg_stack_duplicate'|'addr_reg_stack_pop'
   ;

arrayOpcode1
   : 'vadd'|'add'|'radd'|'riadd'|'cadd'|'caadd'|'cradd'|'vaddc'|'addc'|'raddc'|'riaddc'|'caddc'|'caaddc'|'craddc'|'vsub'|'sub'|'rsub'|'risub'|'csub'|'casub'|'crsub'|'vrvsub'|'rvsub'|'rrvsub'|'rirvsub'|'crvsub'|'carvsub'|'crrvsub'|'vsubc'|'subc'|'rsubc'|'risubc'|'csubc'|'casubc'|'crsubc'|'vrvsubc'|'rvsubc'|'rrvsubc'|'rirvsubc'|'crvsubc'|'carvsubc'|'crrvsubc'|'vmult'|'mult'|'rmult'|'rimult'|'cmult'|'camult'|'crmult'|'vand'|'and'|'rand'|'riand'|'cand'|'caand'|'crand'|'vor'|'or'|'ror'|'rior'|'cor'|'caor'|'cror'|'vxor'|'xor'|'rxor'|'rixor'|'cxor'|'caxor'|'crxor'|'vload'|'load'|'rload'|'riload'|'cload'|'caload'|'crload'|'ixload'|'srload'|'ioload'|'insval'|'addrld'|'store'|'rstore'|'ristore'|'cstore'|'srstore'|'iostore'|'insertio'|'compare'|'rcompare'|'ricompare'|'vcompare'|'ccompare'|'cacompare'|'crcompare'|'shrightc'|'shright'|'sharight'|'rotright'|'shr'|'shl'|'prun'|'pload'|'param'|'getv'|'sendv'|'setint'|'nop'|'jmp'|'brz'|'brnz'|'brsgn'|'brnsgn'|'brzdec'|'brnzdec'|'brbool'|'brnbool'|'brcr'|'brncr'|'halt'|'start'|'stop'|'wherezero'|'wherecarry'|'wherefirst'|'wherenext'|'whereprev'|'wherenzero'|'wherencarry'|'wherenfirst'|'wherennext'|'wherenprev'|'elsewhere'|'endwhere'|'activate'|'rednop'|'redadd'|'redmin'|'redmax'|'setval'|'waitmatw'|'resready'|'brcmpnvaldec'|'setdec'|'addrinc'|'caddrinc'|'addrinc_acc'|'addr_reg_stack_duplicate'|'addr_reg_stack_pop'
   ;

arrayOpcode2
   : 'vadd'|'add'|'radd'|'riadd'|'cadd'|'caadd'|'cradd'|'vaddc'|'addc'|'raddc'|'riaddc'|'caddc'|'caaddc'|'craddc'|'vsub'|'sub'|'rsub'|'risub'|'csub'|'casub'|'crsub'|'vrvsub'|'rvsub'|'rrvsub'|'rirvsub'|'crvsub'|'carvsub'|'crrvsub'|'vsubc'|'subc'|'rsubc'|'risubc'|'csubc'|'casubc'|'crsubc'|'vrvsubc'|'rvsubc'|'rrvsubc'|'rirvsubc'|'crvsubc'|'carvsubc'|'crrvsubc'|'vmult'|'mult'|'rmult'|'rimult'|'cmult'|'camult'|'crmult'|'vand'|'and'|'rand'|'riand'|'cand'|'caand'|'crand'|'vor'|'or'|'ror'|'rior'|'cor'|'caor'|'cror'|'vxor'|'xor'|'rxor'|'rixor'|'cxor'|'caxor'|'crxor'|'vload'|'load'|'rload'|'riload'|'cload'|'caload'|'crload'|'ixload'|'srload'|'ioload'|'insval'|'addrld'|'store'|'rstore'|'ristore'|'cstore'|'srstore'|'iostore'|'insertio'|'compare'|'rcompare'|'ricompare'|'vcompare'|'ccompare'|'cacompare'|'crcompare'|'shrightc'|'shright'|'sharight'|'rotright'|'shr'|'shl'|'prun'|'pload'|'param'|'getv'|'sendv'|'setint'|'nop'|'jmp'|'brz'|'brnz'|'brsgn'|'brnsgn'|'brzdec'|'brnzdec'|'brbool'|'brnbool'|'brcr'|'brncr'|'halt'|'start'|'stop'|'wherezero'|'wherecarry'|'wherefirst'|'wherenext'|'whereprev'|'wherenzero'|'wherencarry'|'wherenfirst'|'wherennext'|'wherenprev'|'elsewhere'|'endwhere'|'activate'|'rednop'|'redadd'|'redmin'|'redmax'|'setval'|'waitmatw'|'resready'|'brcmpnvaldec'|'setdec'|'addrinc'|'caddrinc'|'addrinc_acc'|'addr_reg_stack_duplicate'|'addr_reg_stack_pop'
   ;

arrayOpcode3
   : 'vadd'|'add'|'radd'|'riadd'|'cadd'|'caadd'|'cradd'|'vaddc'|'addc'|'raddc'|'riaddc'|'caddc'|'caaddc'|'craddc'|'vsub'|'sub'|'rsub'|'risub'|'csub'|'casub'|'crsub'|'vrvsub'|'rvsub'|'rrvsub'|'rirvsub'|'crvsub'|'carvsub'|'crrvsub'|'vsubc'|'subc'|'rsubc'|'risubc'|'csubc'|'casubc'|'crsubc'|'vrvsubc'|'rvsubc'|'rrvsubc'|'rirvsubc'|'crvsubc'|'carvsubc'|'crrvsubc'|'vmult'|'mult'|'rmult'|'rimult'|'cmult'|'camult'|'crmult'|'vand'|'and'|'rand'|'riand'|'cand'|'caand'|'crand'|'vor'|'or'|'ror'|'rior'|'cor'|'caor'|'cror'|'vxor'|'xor'|'rxor'|'rixor'|'cxor'|'caxor'|'crxor'|'vload'|'load'|'rload'|'riload'|'cload'|'caload'|'crload'|'ixload'|'srload'|'ioload'|'insval'|'addrld'|'store'|'rstore'|'ristore'|'cstore'|'srstore'|'iostore'|'insertio'|'compare'|'rcompare'|'ricompare'|'vcompare'|'ccompare'|'cacompare'|'crcompare'|'shrightc'|'shright'|'sharight'|'rotright'|'shr'|'shl'|'prun'|'pload'|'param'|'getv'|'sendv'|'setint'|'nop'|'jmp'|'brz'|'brnz'|'brsgn'|'brnsgn'|'brzdec'|'brnzdec'|'brbool'|'brnbool'|'brcr'|'brncr'|'halt'|'start'|'stop'|'wherezero'|'wherecarry'|'wherefirst'|'wherenext'|'whereprev'|'wherenzero'|'wherencarry'|'wherenfirst'|'wherennext'|'wherenprev'|'elsewhere'|'endwhere'|'activate'|'rednop'|'redadd'|'redmin'|'redmax'|'setval'|'waitmatw'|'resready'|'brcmpnvaldec'|'setdec'|'addrinc'|'caddrinc'|'addrinc_acc'|'addr_reg_stack_duplicate'|'addr_reg_stack_pop'
   ;

arrayOpcode4
   : 'vadd'|'add'|'radd'|'riadd'|'cadd'|'caadd'|'cradd'|'vaddc'|'addc'|'raddc'|'riaddc'|'caddc'|'caaddc'|'craddc'|'vsub'|'sub'|'rsub'|'risub'|'csub'|'casub'|'crsub'|'vrvsub'|'rvsub'|'rrvsub'|'rirvsub'|'crvsub'|'carvsub'|'crrvsub'|'vsubc'|'subc'|'rsubc'|'risubc'|'csubc'|'casubc'|'crsubc'|'vrvsubc'|'rvsubc'|'rrvsubc'|'rirvsubc'|'crvsubc'|'carvsubc'|'crrvsubc'|'vmult'|'mult'|'rmult'|'rimult'|'cmult'|'camult'|'crmult'|'vand'|'and'|'rand'|'riand'|'cand'|'caand'|'crand'|'vor'|'or'|'ror'|'rior'|'cor'|'caor'|'cror'|'vxor'|'xor'|'rxor'|'rixor'|'cxor'|'caxor'|'crxor'|'vload'|'load'|'rload'|'riload'|'cload'|'caload'|'crload'|'ixload'|'srload'|'ioload'|'insval'|'addrld'|'store'|'rstore'|'ristore'|'cstore'|'srstore'|'iostore'|'insertio'|'compare'|'rcompare'|'ricompare'|'vcompare'|'ccompare'|'cacompare'|'crcompare'|'shrightc'|'shright'|'sharight'|'rotright'|'shr'|'shl'|'prun'|'pload'|'param'|'getv'|'sendv'|'setint'|'nop'|'jmp'|'brz'|'brnz'|'brsgn'|'brnsgn'|'brzdec'|'brnzdec'|'brbool'|'brnbool'|'brcr'|'brncr'|'halt'|'start'|'stop'|'wherezero'|'wherecarry'|'wherefirst'|'wherenext'|'whereprev'|'wherenzero'|'wherencarry'|'wherenfirst'|'wherennext'|'wherenprev'|'elsewhere'|'endwhere'|'activate'|'rednop'|'redadd'|'redmin'|'redmax'|'setval'|'waitmatw'|'resready'|'brcmpnvaldec'|'setdec'|'addrinc'|'caddrinc'|'addrinc_acc'|'addr_reg_stack_duplicate'|'addr_reg_stack_pop'
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
   : architectureId
   | data
   ;

architectureId
   : ARCHITECTUREID name
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

comment
   : COMMENT
   ;

ARCHITECTUREID
   : '.architectureId'
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