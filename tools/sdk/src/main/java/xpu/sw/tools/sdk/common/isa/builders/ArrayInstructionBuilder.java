//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.isa.builders;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import com.opencsv.*;
import com.opencsv.exceptions.*;

import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;
import org.apache.commons.lang3.tuple.*;

import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.isa.flow.*;
import xpu.sw.tools.sdk.common.isa.instruction.*;

import xpu.sw.tools.sdk.asm.parser.*;

//-------------------------------------------------------------------------------------
public class ArrayInstructionBuilder extends InstructionBuilder {

//-------------------------------------------------------------------------------------
    public ArrayInstructionBuilder(Context _context, String _architectureId) {
        super(_context, _architectureId);
    }

//-------------------------------------------------------------------------------------
    public Instruction build(AsmParser.ArrayInstructionContext _ctx, Callable _callable) {
        Triple<String, String, AsmParser.ExpressionContext> _opcodeAndArgs = extractOpcodeAndArgs(_ctx);
        if(_opcodeAndArgs == null){
            return null;
        }
        return build(_opcodeAndArgs.getLeft(), _opcodeAndArgs.getMiddle(), _opcodeAndArgs.getRight());
    }   

//-------------------------------------------------------------------------------------
    public Triple<String, String, AsmParser.ExpressionContext> extractOpcodeAndArgs(AsmParser.ArrayInstructionContext _ctx) {
        String _opcode = null;
        AsmParser.ExpressionContext _expression = null;
        if(_ctx.arrayOpcode0() != null){
            _opcode = _ctx.arrayOpcode0().getText();
            _expression = ZERO;
        } else if(_ctx.arrayOpcode1() != null){
            _opcode = _ctx.arrayOpcode1().getText();
            _expression = _ctx.expression();
        } else {
//            log.error("Error building instruction!");
//            new Throwable().printStackTrace();
//            _opcode = "";
            return null;
        }
        return Triple.of(_opcode, null, _expression);
    }   

//-------------------------------------------------------------------------------------
    protected void init() {
        super.init();
        addInstruction("vadd"         , "add",     "val", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("add"          , "add",     "mab", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("radd"         , "add",     "mrl", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("riadd"        , "add",     "mri", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("cadd"         , "add",     "cop");
        addInstruction("sadd"         , "add",     "stk");
        addInstruction("cradd"        , "add",     "mrc");

        addInstruction("vaddc"        , "addc",    "val", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("addc"         , "addc",    "mab", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("raddc"        , "addc",    "mrl", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("riaddc"       , "addc",    "mri", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("caddc"        , "addc",    "cop");
        addInstruction("saddc"        , "addc",    "stk");
        addInstruction("craddc"       , "addc",    "mrc");

        addInstruction("vmult"        , "mult",    "val", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("mult"         , "mult",    "mab", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("rmult"        , "mult",    "mrl", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("rimult"       , "mult",    "mri", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("cmult"        , "mult",    "cop");
        addInstruction("smult"        , "mult",    "stk");
        addInstruction("crmult"       , "mult",    "mrc");
           
        addInstruction("vsub"         , "sub",    "val", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("sub"          , "sub",    "mab", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("rsub"         , "sub",    "mrl", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("risub"        , "sub",    "mri", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("csub"         , "sub",    "cop");
        addInstruction("ssub"         , "sub",    "stk");
        addInstruction("crsub"        , "sub",    "mrc");

        addInstruction("vsubc"        , "subc",    "val", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("subc"         , "subc",    "mab", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("rsubc"        , "subc",    "mrl", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("risubc"       , "subc",    "mri", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("csubc"        , "subc",    "cop");
        addInstruction("ssubc"        , "subc",    "stk");
        addInstruction("crsubc"       , "subc",    "mrc");

        addInstruction("vrvsub"       , "rsub",    "val", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("rvsub"        , "rsub",    "mab", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("rrvsub"       , "rsub",    "mrl", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("rirvsub"      , "rsub",    "mri", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("crvsub"       , "rsub",    "cop");
        addInstruction("srvsub"       , "rsub",    "stk");
        addInstruction("crrvsub"      , "rsub",    "mrc");
           
        addInstruction("vrvsubc"      , "rsubc",    "val", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("rvsubc"       , "rsubc",    "mab", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("rrvsubc"      , "rsubc",    "mrl", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("rirvsubc"     , "rsubc",    "mri", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("crvsubc"      , "rsubc",    "cop");
        addInstruction("srvsubc"      , "rsubc",    "stk");
        addInstruction("crrvsubc"     , "rsubc",    "mrc");
                      
        addInstruction("vcompare"     , "compare" , "val", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("compare"      , "compare" , "mab", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("rcompare"     , "compare" , "mrl", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("ricompare"    , "compare" , "mri", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("ccompare"     , "compare" , "cop");
        addInstruction("scompare"     , "compare" , "stk");
        addInstruction("crcompare"    , "compare" , "mrc");

        addInstruction("vand"         , "bwand",    "val", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("and"          , "bwand",    "mab", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("rand"         , "bwand",    "mrl", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("riand"        , "bwand",    "mri", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("cand"         , "bwand",    "cop");
        addInstruction("sand"         , "bwand",    "stk");
        addInstruction("crand"        , "bwand",    "mrc");
           
        addInstruction("vor"          , "bwor",    "val", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("or"           , "bwor",    "mab", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("ror"          , "bwor",    "mrl", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("rior"         , "bwor",    "mri", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("cor"          , "bwor",    "cop");
        addInstruction("sor"          , "bwor",    "stk");
        addInstruction("cror"         , "bwor",    "mrc");
           
        addInstruction("vxor"         , "bwxor",    "val", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("xor"          , "bwxor",    "mab", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("rxor"         , "bwxor",    "mrl", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("rixor"        , "bwxor",    "mri", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("cxor"         , "bwxor",    "cop");
        addInstruction("sxor"         , "bwxor",    "stk");
        addInstruction("crxor"        , "bwxor",    "mrc");

        addInstruction("fadd"         , "fadd",     "mab", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("fradd"        , "fadd",     "mrl", "standardValueFormat", new String[]{"ARG0:NUMBER"});

        addInstruction("fsub"         , "fsub",     "mab", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("frsub"        , "fsub",     "mrl", "standardValueFormat", new String[]{"ARG0:NUMBER"});

        addInstruction("fmult"        , "fmult",     "mab", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("frmult"       , "fmult",     "mrl", "standardValueFormat", new String[]{"ARG0:NUMBER"});

        addInstruction("frtmult"      , "fmult_first",     "mab", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("rfrtmult"     , "fmult_first",     "mrl", "standardValueFormat", new String[]{"ARG0:NUMBER"});

        addInstruction("scdmult"      , "fmult_second",     "mab", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("rscdmult"     , "fmult_second",     "mrl", "standardValueFormat", new String[]{"ARG0:NUMBER"});

        addInstruction("fdiv"      , "fdiv",     "mab", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("frdiv"     , "fdiv",     "mrl", "standardValueFormat", new String[]{"ARG0:NUMBER"});

        addInstruction("loop1"      , "fdiv_loop1",     "mab", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("rloop1"     , "fdiv_loop1",     "mrl", "standardValueFormat", new String[]{"ARG0:NUMBER"});

        addInstruction("loop2"      , "fdiv_loop2",     "mab", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("rloop2"     , "fdiv_loop2",     "mrl", "standardValueFormat", new String[]{"ARG0:NUMBER"});


        addInstruction("addrinc"      , "addrinc",     "val", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("caddrinc"     , "addrinc",     "cop");

        addInstruction("nop"            , "bwor"                , "val");

        addInstruction("store"        , "store"    ,   "mab", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("rstore"       , "store"    ,   "mrl", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("ristore"      , "store"    ,   "mri", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("cstore"       , "store"    ,   "cop");
        addInstruction("sstore"       , "store"    ,   "stk", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("crstore"      , "store"    ,   "mrc");

        addInstruction("stack_store_pop"      , "stack_store_pop"    ,   "mab", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("rstack_store_pop"     , "stack_store_pop"    ,   "mrl", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("ristack_store_pop"    , "stack_store_pop"    ,   "mri", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("cstack_store_pop"     , "stack_store_pop"    ,   "cop");
        addInstruction("crstack_store_pop"    , "stack_store_pop"    ,   "mrc");

        addInstruction("swap_acc_mem"       , "swap_memacc"    ,   "mab", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("rswap_acc_mem"      , "swap_memacc"    ,   "mrl", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("riswap_acc_mem"     , "swap_memacc"    ,   "mri", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("crswap_acc_mem"     , "swap_memacc"    ,   "mrc");
           
        addInstruction("vload"        , "load",      "val", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("load"         , "load",      "mab", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("rload"        , "load",      "mrl", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("riload"       , "load",      "mri", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("cload"        , "load",      "cop");
        addInstruction("sload"        , "load",      "stk");
        addInstruction("crload"       , "load",      "mrc");
        
        addInstruction("vstack_push_load"        , "stack_push_load",      "val", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("stack_push_load"         , "stack_push_load",      "mab", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("rstack_push_load"        , "stack_push_load",      "mrl", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("ristack_push_load"       , "stack_push_load",      "mri", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("cstack_push_load"        , "stack_push_load",      "cop");
        addInstruction("sstack_push_load"        , "stack_push_load",      "stk");
        addInstruction("crstack_push_load"       , "stack_push_load",      "mrc");
        
        addInstruction("send"         , "send",      "mab", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("rsend"        , "send",      "mrl", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("risend"       , "send",      "mri", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("csend"        , "send",      "cop", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("ssend"        , "send",      "stk", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("crsend"       , "send",      "mrc", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        
        addInstruction("srcall"         , "srcall",      "cop");
        addInstruction("vsrcall"        , "srcall",      "val", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("search"         , "search",      "cop");
        addInstruction("vsearch"        , "search",      "val", "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("csearch"        , "csearch",     "cop");
        addInstruction("vcsearch"       , "csearch",     "val", "standardValueFormat", new String[]{"ARG0:NUMBER"});        
        addInstruction("selshift"       , "brshift",     "val");
        addInstruction("delete"         , "delete",      "val");

        addInstruction("shright"                , "right_fixed_shifting"              , "ctl"    , "shrightValueFormat", "INSTR_SHIFT1_RIGHT");
        addInstruction("shrightc"               , "right_fixed_shifting"              , "ctl"    , "shrightValueFormat", "INSTR_SHIFT1_RIGHT_CARRY");
        addInstruction("sharight"               , "right_fixed_shifting"              , "ctl"    , "shrightValueFormat", "INSTR_SHIFT1_RIGHT_ARITHMETIC");
        addInstruction("shright_fixed_amount"   , "right_fixed_shifting"              , "ctl"    , "shrightValueFormat", "INSTR_SHIFT_RIGHT_FIXED_AMOUNT");
        addInstruction("sharight_fixed_amount"  , "right_fixed_shifting"              , "ctl"    , "shrightValueFormat", "INSTR_SHIFT_RIGHT_FIXED_AMOUNT_ARITHMETIC");

        addInstruction("rotright_local"         , "rotate_local"                      , "ctl"    , "rotrightLocalValueFormat", new String[]{"INSTR_SH_ROT_CTRL_bits_RIGHT_ROTATE", "DATA_SIZE – ARG0:NUMBER – 1" , "ARG0:NUMBER"});
        addInstruction("rotleft_local"          , "rotate_local"                      , "ctl"    , "rotrightLocalValueFormat", new String[]{"INSTR_SH_ROT_CTRL_bits_LEFT_ROTATE" , "ARG0:NUMBER - 1"             , "ARG0:NUMBER"});
        addInstruction("shift_right"            , "rotate_local"                      , "ctl"    , "rotrightLocalValueFormat", new String[]{"INSTR_SH_ROT_CTRL_bits_RIGHT_SHIFT" , "DATA_SIZE – ARG0:NUMBER – 1" , "ARG0:NUMBER"});
        addInstruction("shift_left"             , "rotate_local"                      , "ctl"    , "rotrightLocalValueFormat", new String[]{"INSTR_SH_ROT_CTRL_bits_LEFT_SHIFT"  , "ARG0:NUMBER - 1"             , "ARG0:NUMBER"});

//          ARRAY CONTROL INSTRUCTIONS #########
        addInstruction("wherezero"                  , "spatial_select"      , "ctl"       , "whereZeroValueFormat"    , new String[]{"INSTR_SPATIAL_SELECT_FUNCTION_WHERE"            , "INSTR_SPATIAL_SELECT_WHERE_COND_WHEREZERO"  });
        addInstruction("wherepositive"              , "spatial_select"      , "ctl"       , "whereZeroValueFormat"    , new String[]{"INSTR_SPATIAL_SELECT_FUNCTION_WHERE"            , "INSTR_SPATIAL_SELECT_WHERE_COND_WHERENSGN"  });
        addInstruction("wherecarry"                 , "spatial_select"      , "ctl"       , "whereZeroValueFormat"    , new String[]{"INSTR_SPATIAL_SELECT_FUNCTION_WHERE"            , "INSTR_SPATIAL_SELECT_WHERE_COND_WHERECARRY" });
        addInstruction("wherefirst"                 , "spatial_select"      , "ctl"       , "whereZeroValueFormat"    , new String[]{"INSTR_SPATIAL_SELECT_FUNCTION_WHERE"            , "INSTR_SPATIAL_SELECT_WHERE_COND_WHEREFIRST" });
        addInstruction("wherenext"                  , "spatial_select"      , "ctl"       , "whereZeroValueFormat"    , new String[]{"INSTR_SPATIAL_SELECT_FUNCTION_WHERE"            , "INSTR_SPATIAL_SELECT_WHERE_COND_WHERENEXT"  });
        addInstruction("whereprev"                  , "spatial_select"      , "ctl"       , "whereZeroValueFormat"    , new String[]{"INSTR_SPATIAL_SELECT_FUNCTION_WHERE"            , "INSTR_SPATIAL_SELECT_WHERE_COND_WHEREPREV"  });
        addInstruction("wherenzero"                 , "spatial_select"      , "ctl"       , "whereZeroValueFormat"    , new String[]{"INSTR_SPATIAL_SELECT_FUNCTION_WHERE"            , "INSTR_SPATIAL_SELECT_WHERE_COND_WHERENZERO" });
        addInstruction("wherenegative"              , "spatial_select"      , "ctl"       , "whereZeroValueFormat"    , new String[]{"INSTR_SPATIAL_SELECT_FUNCTION_WHERE"            , "INSTR_SPATIAL_SELECT_WHERE_COND_WHERESGN"   });
        addInstruction("wherencarry"                , "spatial_select"      , "ctl"       , "whereZeroValueFormat"    , new String[]{"INSTR_SPATIAL_SELECT_FUNCTION_WHERE"            , "INSTR_SPATIAL_SELECT_WHERE_COND_WHERENCARRY"});
        addInstruction("wherenfirst"                , "spatial_select"      , "ctl"       , "whereZeroValueFormat"    , new String[]{"INSTR_SPATIAL_SELECT_FUNCTION_WHERE"            , "INSTR_SPATIAL_SELECT_WHERE_COND_WHERENFIRST"});
        addInstruction("wherennext"                 , "spatial_select"      , "ctl"       , "whereZeroValueFormat"    , new String[]{"INSTR_SPATIAL_SELECT_FUNCTION_WHERE"            , "INSTR_SPATIAL_SELECT_WHERE_COND_WHERENNEXT" });
        addInstruction("wherenprev"                 , "spatial_select"      , "ctl"       , "whereZeroValueFormat"    , new String[]{"INSTR_SPATIAL_SELECT_FUNCTION_WHERE"            , "INSTR_SPATIAL_SELECT_WHERE_COND_WHERENPREV" });

        addInstruction("wheresgn"                   , "spatial_select"      , "ctl"       , "whereZeroValueFormat"    , new String[]{"INSTR_SPATIAL_SELECT_FUNCTION_WHERE"            , "INSTR_SPATIAL_SELECT_WHERE_COND_WHERESGN"           });
        addInstruction("wherensgn"                  , "spatial_select"      , "ctl"       , "whereZeroValueFormat"    , new String[]{"INSTR_SPATIAL_SELECT_FUNCTION_WHERE"            , "INSTR_SPATIAL_SELECT_WHERE_COND_WHERENSGN"          });
        addInstruction("wherelast"                  , "spatial_select"      , "ctl"       , "whereZeroValueFormat"    , new String[]{"INSTR_SPATIAL_SELECT_FUNCTION_WHERE"            , "INSTR_SPATIAL_SELECT_WHERE_COND_WHERELAST"          });
        addInstruction("wherenlast"                 , "spatial_select"      , "ctl"       , "whereZeroValueFormat"    , new String[]{"INSTR_SPATIAL_SELECT_FUNCTION_WHERE"            , "INSTR_SPATIAL_SELECT_WHERE_COND_WHERENLAST"         });
        addInstruction("whereafterfirst"            , "spatial_select"      , "ctl"       , "whereZeroValueFormat"    , new String[]{"INSTR_SPATIAL_SELECT_FUNCTION_WHERE"            , "INSTR_SPATIAL_SELECT_WHERE_COND_WHERE_AFTER_FIRST"  });
        addInstruction("wherenafterfirst"           , "spatial_select"      , "ctl"       , "whereZeroValueFormat"    , new String[]{"INSTR_SPATIAL_SELECT_FUNCTION_WHERE"            , "INSTR_SPATIAL_SELECT_WHERE_COND_WHEREN_AFTER_FIRST" });
        addInstruction("wherebeforelast"            , "spatial_select"      , "ctl"       , "whereZeroValueFormat"    , new String[]{"INSTR_SPATIAL_SELECT_FUNCTION_WHERE"            , "INSTR_SPATIAL_SELECT_WHERE_COND_WHERE_BEFORE_LAST"  });
        addInstruction("wherenbeforelast"           , "spatial_select"      , "ctl"       , "whereZeroValueFormat"    , new String[]{"INSTR_SPATIAL_SELECT_FUNCTION_WHERE"            , "INSTR_SPATIAL_SELECT_WHERE_COND_WHEREN_BEFORE_LAST" });

        addInstruction("elsewhere"                  , "spatial_select"      , "ctl"       , "whereZeroValueFormat"    , new String[]{"INSTR_SPATIAL_SELECT_FUNCTION_ELSEWHERE"            , "INSTR_SPATIAL_SELECT_WHERE_COND_DONT_CARE"  });
        addInstruction("endwhere"                   , "spatial_select"      , "ctl"       , "whereZeroValueFormat"    , new String[]{"INSTR_SPATIAL_SELECT_FUNCTION_ENDWHERE"            , "INSTR_SPATIAL_SELECT_WHERE_COND_DONT_CARE"  });
        addInstruction("activate"                   , "spatial_select"      , "ctl"       , "whereZeroValueFormat"    , new String[]{"INSTR_SPATIAL_SELECT_FUNCTION_ACTIVATE"            , "INSTR_SPATIAL_SELECT_WHERE_COND_DONT_CARE"  });


        addInstruction("scannop"                    , "setscan"             , "ctl"       , "scannopValueFormat"      , new String[]{"NETWORK_OP_NOP"});
        addInstruction("scansplit"                  , "setscan"             , "ctl"       , "scansplitValueFormat"    , new String[]{"NETWORK_OP_SPLIT", "ARG0:NUMBER"});
        addInstruction("scanpermute"                , "setscan"             , "ctl"       , "scansplitValueFormat"    , new String[]{"NETWORK_OP_PERMUTE", "ARG0:NUMBER"});
        addInstruction("scanadd"                    , "setscan"             , "ctl"       , "scannopValueFormat"      , new String[]{"NETWORK_OP_ADD"});
        addInstruction("scanmin"                    , "setscan"             , "ctl"       , "scannopValueFormat"      , new String[]{"NETWORK_OP_MIN"});
        addInstruction("scanmax"                    , "setscan"             , "ctl"       , "scannopValueFormat"      , new String[]{"NETWORK_OP_MAX"});
        addInstruction("scanprefix_add"             , "setscan"             , "ctl"       , "scannopValueFormat"      , new String[]{"NETWORK_OP_SUM_PREFIX"});
        addInstruction("scanprefix_bitwise_xor"     , "setscan"             , "ctl"       , "scannopValueFormat"      , new String[]{"NETWORK_OP_bitwise_XOR_PREFIX"});
        addInstruction("scansub"                    , "setscan"             , "ctl"       , "scannopValueFormat"      , new String[]{"NETWORK_OP_SUB"});
        addInstruction("scan_bitwise_or"            , "setscan"             , "ctl"       , "scannopValueFormat"      , new String[]{"NETWORK_OP_bitwise_OR"});
        addInstruction("scan_bitwise_xor"           , "setscan"             , "ctl"       , "scannopValueFormat"      , new String[]{"NETWORK_OP_bitwise_XOR"});
        addInstruction("scan_bitwise_and"           , "setscan"             , "ctl"       , "scannopValueFormat"      , new String[]{"NETWORK_OP_bitwise_AND"});

        addInstruction("float"                      , "float"               , "ctl"       , "standardValueFormat", new String[]{"ARG0:NUMBER"});

        addInstruction("addrinc_acc"                , "addrinc"             , "ctl"       );
        addInstruction("addr_reg_stack_duplicate"   , "misc_store_load"             , "ctl"       , "addrRegStackDuplicateValueFormat"      , new String[]{"INSTR_MISC_STORE_LOAD_array_addr_reg_stack_duplicate"});
        addInstruction("addr_reg_stack_pop"         , "misc_store_load"             , "ctl"       , "addrRegStackDuplicateValueFormat"      , new String[]{"INSTR_MISC_STORE_LOAD_array_addr_reg_stack_pop"});

        addInstruction("addrstore"                  , "misc_store_load"     , "ctl"       , "addrStoreValueFormat"                  , new String[]{"INSTR_MISC_STORE_LOAD_addrstore"});
        addInstruction("caddrstore"                 , "misc_store_load"     , "ctl"       , "addrStoreValueFormat"                  , new String[]{"INSTR_MISC_STORE_LOAD_addrstore_COP"});
        addInstruction("insertio"                   , "misc_store_load"     , "ctl"       , "addrStoreValueFormat"                  , new String[]{"INSTR_MISC_STORE_LOAD_insertio"});

        addInstruction("ixload"                     , "misc_store_load"     , "ctl"       , "ixLoadValueFormat"                     , new String[]{"INSTR_MISC_STORE_LOAD_ixload"});
        addInstruction("srload"                     , "misc_store_load"     , "ctl"       , "ixLoadValueFormat"                     , new String[]{"INSTR_MISC_STORE_LOAD_srload"});
        addInstruction("scanload"                   , "misc_store_load"     , "ctl"       , "ixLoadValueFormat"                     , new String[]{"INSTR_MISC_STORE_LOAD_scanload"});

        addInstruction("stack_pop"                  , "stack_operations"    , "ctl"       , "stackPopValueFormat"                   , new String[]{"ISA_stack_operations_CTL_val_POP"});
        addInstruction("stack_duplicate"            , "stack_operations"    , "ctl"       , "stackPopValueFormat"                   , new String[]{"ISA_stack_operations_CTL_val_DUPLICATE"});
        addInstruction("stack_over"                 , "stack_operations"    , "ctl"       , "stackPopValueFormat"                   , new String[]{"ISA_stack_operations_CTL_val_OVER"});
        addInstruction("stack_swap"                 , "stack_operations"    , "ctl"       , "stackPopValueFormat"                   , new String[]{"ISA_stack_operations_CTL_val_SWAP"});
        addInstruction("stack_load_layer1"          , "stack_operations"    , "ctl"       , "stackPopValueFormat"                   , new String[]{"ISA_stack_operations_CTL_val_LOAD_LAYRER1"});


    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
