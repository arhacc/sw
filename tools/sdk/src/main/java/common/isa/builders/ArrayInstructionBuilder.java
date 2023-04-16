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

import xpu.sw.tools.sdk.common.isa.*;
import xpu.sw.tools.sdk.common.context.*;

import xpu.sw.tools.sdk.asm.parser.*;

//-------------------------------------------------------------------------------------
public class ArrayInstructionBuilder extends InstructionBuilder {

//-------------------------------------------------------------------------------------
    public ArrayInstructionBuilder(Context _context, String _arhCode) {
        super(_context, _arhCode);
    }

//-------------------------------------------------------------------------------------
    public Instruction build(AsmParser.ArrayInstructionContext _ctx, Primitive _primitive) {
        Pair<String, String[]> _opcodeAndArgs = extractOpcodeAndArgs(_ctx);
        return build(_opcodeAndArgs.getLeft(), _opcodeAndArgs.getRight(), _primitive);
    }   

//-------------------------------------------------------------------------------------
    public Pair<String, String[]> extractOpcodeAndArgs(AsmParser.ArrayInstructionContext _ctx) {
        String _opcode = null;
        String[] _args = null;
        if(_ctx.arrayOpcode0() != null){
            _opcode = _ctx.arrayOpcode0().getText();
            _args = new String[]{"ZERO"};
        } else if(_ctx.arrayOpcode1() != null){
            _opcode = _ctx.arrayOpcode1().getText();
            _args = new String[]{extractValue(_ctx.value(0))};
        } else if(_ctx.arrayOpcode2() != null){
            _opcode = _ctx.arrayOpcode2().getText();
            _args = new String[]{extractValue(_ctx.value(0)), extractValue(_ctx.value(1))};
        } else if(_ctx.arrayOpcode3() != null){
            _opcode = _ctx.arrayOpcode3().getText();
            _args = new String[]{extractLabel(_ctx.lb())};
        } else if(_ctx.arrayOpcode4() != null){
            _opcode = _ctx.arrayOpcode4().getText();
            _args = new String[]{extractLabel(_ctx.lb()), extractValue(_ctx.value(0))};
        } else {
            log.error("Error building instruction!");
            _opcode = "";
            _args = new String[]{""};
        }

        return Pair.of(_opcode, _args);
    }   

//-------------------------------------------------------------------------------------
    protected void init() {
        super.init();
        addInstruction("vadd"         , "add",     "val", standardOneArgValueFormat);
        addInstruction("add"          , "add",     "mab", standardOneArgValueFormat);
        addInstruction("radd"         , "add",     "mrl", standardOneArgValueFormat);
        addInstruction("riadd"        , "add",     "mri", standardOneArgValueFormat);
        addInstruction("cadd"         , "add",     "cop");
        addInstruction("sadd"         , "add",     "stk");
        addInstruction("cradd"        , "add",     "mrc");

        addInstruction("vaddc"        , "addc",    "val", standardOneArgValueFormat);
        addInstruction("addc"         , "addc",    "mab", standardOneArgValueFormat);
        addInstruction("raddc"        , "addc",    "mrl", standardOneArgValueFormat);
        addInstruction("riaddc"       , "addc",    "mri", standardOneArgValueFormat);
        addInstruction("caddc"        , "addc",    "cop");
        addInstruction("saddc"        , "addc",    "stk");
        addInstruction("craddc"       , "addc",    "mrc");

        addInstruction("vmult"        , "mult",    "val", standardOneArgValueFormat);
        addInstruction("mult"         , "mult",    "mab", standardOneArgValueFormat);
        addInstruction("rmult"        , "mult",    "mrl", standardOneArgValueFormat);
        addInstruction("rimult"       , "mult",    "mri", standardOneArgValueFormat);
        addInstruction("cmult"        , "mult",    "cop");
        addInstruction("smult"        , "mult",    "stk");
        addInstruction("crmult"       , "mult",    "mrc");
           
        addInstruction("vsub"         , "sub",    "val", standardOneArgValueFormat);
        addInstruction("sub"          , "sub",    "mab", standardOneArgValueFormat);
        addInstruction("rsub"         , "sub",    "mrl", standardOneArgValueFormat);
        addInstruction("risub"        , "sub",    "mri", standardOneArgValueFormat);
        addInstruction("csub"         , "sub",    "cop");
        addInstruction("ssub"         , "sub",    "stk");
        addInstruction("crsub"        , "sub",    "mrc");

        addInstruction("vsubc"        , "subc",    "val", standardOneArgValueFormat);
        addInstruction("subc"         , "subc",    "mab", standardOneArgValueFormat);
        addInstruction("rsubc"        , "subc",    "mrl", standardOneArgValueFormat);
        addInstruction("risubc"       , "subc",    "mri", standardOneArgValueFormat);
        addInstruction("csubc"        , "subc",    "cop");
        addInstruction("ssubc"        , "subc",    "stk");
        addInstruction("crsubc"       , "subc",    "mrc");

        addInstruction("vrvsub"       , "rsub",    "val", standardOneArgValueFormat);
        addInstruction("rvsub"        , "rsub",    "mab", standardOneArgValueFormat);
        addInstruction("rrvsub"       , "rsub",    "mrl", standardOneArgValueFormat);
        addInstruction("rirvsub"      , "rsub",    "mri", standardOneArgValueFormat);
        addInstruction("crvsub"       , "rsub",    "cop");
        addInstruction("srvsub"       , "rsub",    "stk");
        addInstruction("crrvsub"      , "rsub",    "mrc");
           
        addInstruction("vrvsubc"      , "rsubc",    "val", standardOneArgValueFormat);
        addInstruction("rvsubc"       , "rsubc",    "mab", standardOneArgValueFormat);
        addInstruction("rrvsubc"      , "rsubc",    "mrl", standardOneArgValueFormat);
        addInstruction("rirvsubc"     , "rsubc",    "mri", standardOneArgValueFormat);
        addInstruction("crvsubc"      , "rsubc",    "cop");
        addInstruction("srvsubc"      , "rsubc",    "stk");
        addInstruction("crrvsubc"     , "rsubc",    "mrc");
                      
        addInstruction("vcompare"     , "compare" , "val", standardOneArgValueFormat);
        addInstruction("compare"      , "compare" , "mab", standardOneArgValueFormat);
        addInstruction("rcompare"     , "compare" , "mrl", standardOneArgValueFormat);
        addInstruction("ricompare"    , "compare" , "mri", standardOneArgValueFormat);
        addInstruction("ccompare"     , "compare" , "cop");
        addInstruction("scompare"     , "compare" , "stk");
        addInstruction("crcompare"    , "compare" , "mrc");

        addInstruction("vand"         , "bwand",    "val", standardOneArgValueFormat);
        addInstruction("and"          , "bwand",    "mab", standardOneArgValueFormat);
        addInstruction("rand"         , "bwand",    "mrl", standardOneArgValueFormat);
        addInstruction("riand"        , "bwand",    "mri", standardOneArgValueFormat);
        addInstruction("cand"         , "bwand",    "cop");
        addInstruction("sand"         , "bwand",    "stk");
        addInstruction("crand"        , "bwand",    "mrc");
           
        addInstruction("vor"          , "bwor",    "val", standardOneArgValueFormat);
        addInstruction("or"           , "bwor",    "mab", standardOneArgValueFormat);
        addInstruction("ror"          , "bwor",    "mrl", standardOneArgValueFormat);
        addInstruction("rior"         , "bwor",    "mri", standardOneArgValueFormat);
        addInstruction("cor"          , "bwor",    "cop");
        addInstruction("sor"          , "bwor",    "stk");
        addInstruction("cror"         , "bwor",    "mrc");
           
        addInstruction("vxor"         , "bwxor",    "val", standardOneArgValueFormat);
        addInstruction("xor"          , "bwxor",    "mab", standardOneArgValueFormat);
        addInstruction("rxor"         , "bwxor",    "mrl", standardOneArgValueFormat);
        addInstruction("rixor"        , "bwxor",    "mri", standardOneArgValueFormat);
        addInstruction("cxor"         , "bwxor",    "cop");
        addInstruction("sxor"         , "bwxor",    "stk");
        addInstruction("crxor"        , "bwxor",    "mrc");

        addInstruction("fadd"         , "fadd",     "mab", standardOneArgValueFormat);
        addInstruction("fradd"        , "fadd",     "mrl", standardOneArgValueFormat);

        addInstruction("fsub"         , "fsub",     "mab", standardOneArgValueFormat);
        addInstruction("frsub"        , "fsub",     "mrl", standardOneArgValueFormat);

        addInstruction("fmult"        , "fmult",     "mab", standardOneArgValueFormat);
        addInstruction("frmult"       , "fmult",     "mrl", standardOneArgValueFormat);

        addInstruction("frtmult"      , "fmult_first",     "mab", standardOneArgValueFormat);
        addInstruction("rfrtmult"     , "fmult_first",     "mrl", standardOneArgValueFormat);

        addInstruction("scdmult"      , "fmult_second",     "mab", standardOneArgValueFormat);
        addInstruction("rscdmult"     , "fmult_second",     "mrl", standardOneArgValueFormat);

        addInstruction("fdiv"      , "fdiv",     "mab", standardOneArgValueFormat);
        addInstruction("frdiv"     , "fdiv",     "mrl", standardOneArgValueFormat);

        addInstruction("loop1"      , "fdiv_loop1",     "mab", standardOneArgValueFormat);
        addInstruction("rloop1"     , "fdiv_loop2",     "mrl", standardOneArgValueFormat);

        addInstruction("loop2"      , "fdiv_loop2",     "mab", standardOneArgValueFormat);
        addInstruction("rloop2"     , "fdiv_loop2",     "mrl", standardOneArgValueFormat);


        addInstruction("addrinc"      , "addrinc",     "val", standardOneArgValueFormat);
        addInstruction("caddrinc"     , "addrinc",     "cop", standardOneArgValueFormat);

        addInstruction("nop"            , "bwor"                , "val");

        addInstruction("store"        , "store"    ,   "mab", standardOneArgValueFormat);
        addInstruction("rstore"       , "store"    ,   "mrl", standardOneArgValueFormat);
        addInstruction("ristore"      , "store"    ,   "mri", standardOneArgValueFormat);
        addInstruction("cstore"       , "store"    ,   "cop");
        addInstruction("sstore"       , "store"    ,   "stk", standardOneArgValueFormat);
        addInstruction("crstore"      , "store"    ,   "mrc");

        addInstruction("stack_store_pop"      , "stack_store_pop"    ,   "mab", standardOneArgValueFormat);
        addInstruction("rstack_store_pop"     , "stack_store_pop"    ,   "mrl", standardOneArgValueFormat);
        addInstruction("ristack_store_pop"    , "stack_store_pop"    ,   "mri", standardOneArgValueFormat);
        addInstruction("cstack_store_pop"     , "stack_store_pop"    ,   "cop");
        addInstruction("crstack_store_pop"    , "stack_store_pop"    ,   "mrc");

        addInstruction("swap_acc_mem"       , "swap_memacc"    ,   "mab", standardOneArgValueFormat);
        addInstruction("rswap_acc_mem"      , "swap_memacc"    ,   "mrl", standardOneArgValueFormat);
        addInstruction("riswap_acc_mem"     , "swap_memacc"    ,   "mri", standardOneArgValueFormat);
        addInstruction("crswap_acc_mem"     , "swap_memacc"    ,   "mrc");
           
        addInstruction("vload"        , "load",      "val", standardOneArgValueFormat);
        addInstruction("load"         , "load",      "mab", standardOneArgValueFormat);
        addInstruction("rload"        , "load",      "mrl", standardOneArgValueFormat);
        addInstruction("riload"       , "load",      "mri", standardOneArgValueFormat);
        addInstruction("cload"        , "load",      "cop");
        addInstruction("sload"        , "load",      "stk");
        addInstruction("crload"       , "load",      "mrc");
        
        addInstruction("vstack_push_load"        , "stack_push_load",      "val", standardOneArgValueFormat);
        addInstruction("stack_push_load"         , "stack_push_load",      "mab", standardOneArgValueFormat);
        addInstruction("rstack_push_load"        , "stack_push_load",      "mrl", standardOneArgValueFormat);
        addInstruction("ristack_push_load"       , "stack_push_load",      "mri", standardOneArgValueFormat);
        addInstruction("cstack_push_load"        , "stack_push_load",      "cop");
        addInstruction("sstack_push_load"        , "stack_push_load",      "stk");
        addInstruction("crstack_push_load"       , "stack_push_load",      "mrc");
        
        addInstruction("send"         , "send",      "mab", standardOneArgValueFormat);
        addInstruction("rsend"        , "send",      "mrl", standardOneArgValueFormat);
        addInstruction("risend"       , "send",      "mri", standardOneArgValueFormat);
        addInstruction("csend"        , "send",      "cop", standardOneArgValueFormat);
        addInstruction("ssend"        , "send",      "stk", standardOneArgValueFormat);
        addInstruction("crsend"       , "send",      "mrc", standardOneArgValueFormat);
        
        addInstruction("srcall"         , "srcall",      "cop");
        addInstruction("vsrcall"        , "srcall",      "val");
        addInstruction("search"         , "search",      "cop");
        addInstruction("vsearch"        , "search",      "val");
        addInstruction("csearch"        , "csearch",     "cop");
        addInstruction("vcsearch"       , "csearch",     "val");        
        addInstruction("selshift"       , "brshift",     "val");
        addInstruction("delete"         , "delete",      "val");

        addInstruction("shright"                , "right_fixed_shifting"              , "ctl"    , shrightValueFormat, "INSTR_SHIFT1_RIGHT");
        addInstruction("shrightc"               , "right_fixed_shifting"              , "ctl"    , shrightValueFormat, "INSTR_SHIFT1_RIGHT_CARRY");
        addInstruction("sharight"               , "right_fixed_shifting"              , "ctl"    , shrightValueFormat, "INSTR_SHIFT1_RIGHT_ARITHMETIC");
        addInstruction("shright_fixed_amount"   , "right_fixed_shifting"              , "ctl"    , shrightValueFormat, "INSTR_SHIFT_RIGHT_FIXED_AMOUNT");
        addInstruction("sharight_fixed_amount"  , "right_fixed_shifting"              , "ctl"    , shrightValueFormat, "INSTR_SHIFT_RIGHT_FIXED_AMOUNT_ARITHMETIC");

        addInstruction("rotright_local"         , "rotate_local"                      , "ctl"    , rotrightLocalValueFormat, "INSTR_SH_ROT_CTRL_bits_RIGHT_ROTATE");
        addInstruction("shift_right"            , "rotate_local"                      , "ctl"    , rotrightLocalValueFormat, "INSTR_SH_ROT_CTRL_bits_LEFT_ROTATE");
        addInstruction("shift_left"             , "rotate_local"                      , "ctl"    , rotrightLocalValueFormat, "INSTR_SH_ROT_CTRL_bits_RIGHT_SHIFT");

//          ARRAY CONTROL INSTRUCTIONS #########
        addInstruction("wherezero"                  , "spatial_select"      , "ctl"       , whereZeroValueFormat    , new String[]{"INSTR_SPATIAL_SELECT_FUNCTION_WHERE"            , "INSTR_SPATIAL_SELECT_WHERE_COND_WHEREZERO"  });
        addInstruction("wherepositive"              , "spatial_select"      , "ctl"       , whereZeroValueFormat    , new String[]{"INSTR_SPATIAL_SELECT_FUNCTION_WHERE"            , "INSTR_SPATIAL_SELECT_WHERE_COND_WHERENSGN"  });
        addInstruction("wherecarry"                 , "spatial_select"      , "ctl"       , whereZeroValueFormat    , new String[]{"INSTR_SPATIAL_SELECT_FUNCTION_WHERE"            , "INSTR_SPATIAL_SELECT_WHERE_COND_WHERECARRY" });
        addInstruction("wherefirst"                 , "spatial_select"      , "ctl"       , whereZeroValueFormat    , new String[]{"INSTR_SPATIAL_SELECT_FUNCTION_WHERE"            , "INSTR_SPATIAL_SELECT_WHERE_COND_WHEREFIRST" });
        addInstruction("wherenext"                  , "spatial_select"      , "ctl"       , whereZeroValueFormat    , new String[]{"INSTR_SPATIAL_SELECT_FUNCTION_WHERE"            , "INSTR_SPATIAL_SELECT_WHERE_COND_WHERENEXT"  });
        addInstruction("whereprev"                  , "spatial_select"      , "ctl"       , whereZeroValueFormat    , new String[]{"INSTR_SPATIAL_SELECT_FUNCTION_WHERE"            , "INSTR_SPATIAL_SELECT_WHERE_COND_WHEREPREV"  });
        addInstruction("wherenzero"                 , "spatial_select"      , "ctl"       , whereZeroValueFormat    , new String[]{"INSTR_SPATIAL_SELECT_FUNCTION_WHERE"            , "INSTR_SPATIAL_SELECT_WHERE_COND_WHERENZERO" });
        addInstruction("wherenegative"              , "spatial_select"      , "ctl"       , whereZeroValueFormat    , new String[]{"INSTR_SPATIAL_SELECT_FUNCTION_WHERE"            , "INSTR_SPATIAL_SELECT_WHERE_COND_WHERESGN"   });
        addInstruction("wherencarry"                , "spatial_select"      , "ctl"       , whereZeroValueFormat    , new String[]{"INSTR_SPATIAL_SELECT_FUNCTION_WHERE"            , "INSTR_SPATIAL_SELECT_WHERE_COND_WHERENCARRY"});
        addInstruction("wherenfirst"                , "spatial_select"      , "ctl"       , whereZeroValueFormat    , new String[]{"INSTR_SPATIAL_SELECT_FUNCTION_WHERE"            , "INSTR_SPATIAL_SELECT_WHERE_COND_WHERENFIRST"});
        addInstruction("wherennext"                 , "spatial_select"      , "ctl"       , whereZeroValueFormat    , new String[]{"INSTR_SPATIAL_SELECT_FUNCTION_WHERE"            , "INSTR_SPATIAL_SELECT_WHERE_COND_WHERENNEXT" });
        addInstruction("wherenprev"                 , "spatial_select"      , "ctl"       , whereZeroValueFormat    , new String[]{"INSTR_SPATIAL_SELECT_FUNCTION_WHERE"            , "INSTR_SPATIAL_SELECT_WHERE_COND_WHERENPREV" });
        addInstruction("elsewhere"                  , "spatial_select"      , "ctl"       , whereZeroValueFormat    , new String[]{"INSTR_SPATIAL_SELECT_FUNCTION_WHERE"            , "INSTR_SPATIAL_SELECT_WHERE_COND_DONT_CARE"  });
        addInstruction("endwhere"                   , "spatial_select"      , "ctl"       , whereZeroValueFormat    , new String[]{"INSTR_SPATIAL_SELECT_FUNCTION_WHERE"            , "INSTR_SPATIAL_SELECT_WHERE_COND_DONT_CARE"  });
        addInstruction("activate"                   , "spatial_select"      , "ctl"       , whereZeroValueFormat    , new String[]{"INSTR_SPATIAL_SELECT_FUNCTION_WHERE"            , "INSTR_SPATIAL_SELECT_WHERE_COND_DONT_CARE"  });


        addInstruction("scannop"                    , "setscan"             , "ctl"       , scannopValueFormat      , new String[]{"NETWORK_OP_NOP"});
        addInstruction("scansplit"                  , "setscan"             , "ctl"       , scansplitValueFormat    , new String[]{"NETWORK_OP_SPLIT", "ARG0"});
        addInstruction("scanpermute"                , "setscan"             , "ctl"       , scansplitValueFormat    , new String[]{"NETWORK_OP_PERMUTE", "ARG0"});
        addInstruction("scanadd"                    , "setscan"             , "ctl"       , scannopValueFormat      , new String[]{"NETWORK_OP_ADD"});
        addInstruction("scanmin"                    , "setscan"             , "ctl"       , scannopValueFormat      , new String[]{"NETWORK_OP_MIN"});
        addInstruction("scanmax"                    , "setscan"             , "ctl"       , scannopValueFormat      , new String[]{"NETWORK_OP_MAX"});
        addInstruction("scanprefix_add"             , "setscan"             , "ctl"       , scannopValueFormat      , new String[]{"NETWORK_OP_SUM_PREFIX"});
        addInstruction("scanprefix_bitwise_xor"     , "setscan"             , "ctl"       , scannopValueFormat      , new String[]{"NETWORK_OP_bitwise_XOR_PREFIX"});
        addInstruction("scansub"                    , "setscan"             , "ctl"       , scannopValueFormat      , new String[]{"NETWORK_OP_SUB"});
        addInstruction("scan_bitwise_or"            , "setscan"             , "ctl"       , scannopValueFormat      , new String[]{"NETWORK_OP_bitwise_OR"});
        addInstruction("scan_bitwise_xor"           , "setscan"             , "ctl"       , scannopValueFormat      , new String[]{"NETWORK_OP_bitwise_XOR"});
        addInstruction("scan_bitwise_and"           , "setscan"             , "ctl"       , scannopValueFormat      , new String[]{"NETWORK_OP_bitwise_AND"});

        addInstruction("float"                      , "float"               , "ctl"       , standardOneArgValueFormat);

        addInstruction("addrinc_acc"                , "addrinc"             , "ctl"       );
        addInstruction("addr_reg_stack_duplicate"   , "addrinc"             , "ctl"       , addrRegStackDuplicateValueFormat      , new String[]{"INSTR_MISC_STORE_LOAD_array_addr_reg_stack_duplicate"});
        addInstruction("addr_reg_stack_pop"         , "addrinc"             , "ctl"       , addrRegStackDuplicateValueFormat      , new String[]{"INSTR_MISC_STORE_LOAD_array_addr_reg_stack_pop"});

        addInstruction("addrstore"                  , "misc_store_load"     , "ctl"       , addrStoreValueFormat                  , new String[]{"INSTR_MISC_STORE_LOAD_addrstore"});
        addInstruction("caddrstore"                 , "misc_store_load"     , "ctl"       , addrStoreValueFormat                  , new String[]{"INSTR_MISC_STORE_LOAD_addrstore_COP"});
        addInstruction("insertio"                   , "misc_store_load"     , "ctl"       , addrStoreValueFormat                  , new String[]{"INSTR_MISC_STORE_LOAD_insertio"});

        addInstruction("stack_pop"                  , "stack_operations"    , "ctl"       , stackPopValueFormat                   , new String[]{"ISA_stack_operations_CTL_val_POP"});
        addInstruction("stack_duplicate"            , "stack_operations"    , "ctl"       , stackPopValueFormat                   , new String[]{"ISA_stack_operations_CTL_val_DUPLICATE"});
        addInstruction("stack_over"                 , "stack_operations"    , "ctl"       , stackPopValueFormat                   , new String[]{"ISA_stack_operations_CTL_val_OVER"});
        addInstruction("stack_swap"                 , "stack_operations"    , "ctl"       , stackPopValueFormat                   , new String[]{"ISA_stack_operations_CTL_val_SWAP"});
        addInstruction("stack_load_layer1"          , "stack_operations"    , "ctl"       , stackPopValueFormat                   , new String[]{"ISA_stack_operations_CTL_val_LOAD_LAYRER1"});

    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
