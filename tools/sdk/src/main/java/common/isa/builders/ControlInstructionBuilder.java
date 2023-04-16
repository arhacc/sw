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
public class ControlInstructionBuilder extends InstructionBuilder {

//-------------------------------------------------------------------------------------
    public ControlInstructionBuilder(Context _context, String _arhCode) {
        super(_context, _arhCode);
    }

//-------------------------------------------------------------------------------------
    public Instruction build(AsmParser.ControlInstructionContext _ctx, Primitive _primitive) {
//        log.debug("InstructionBuilder: " + _opcode + ", " + _valueString + ", " + _valueNumber);
        Pair<String, String[]> _opcodeAndArgs = extractOpcodeAndArgs(_ctx);
        return build(_opcodeAndArgs.getLeft(), _opcodeAndArgs.getRight(), _primitive);
    }   

//-------------------------------------------------------------------------------------
    public Pair<String, String[]> extractOpcodeAndArgs(AsmParser.ControlInstructionContext _ctx) {
        String _opcode = null;
        String[] _args = null;
        if(_ctx.controlOpcode0() != null){
            _opcode = _ctx.controlOpcode0().getText();
            _args = new String[]{"ZERO"};
        } else if(_ctx.controlOpcode1() != null){
            _opcode = _ctx.controlOpcode1().getText();
            _args = new String[]{extractValue(_ctx.value(0))};
        } else if(_ctx.controlOpcode2() != null){
            _opcode = _ctx.controlOpcode2().getText();
            _args = new String[]{extractValue(_ctx.value(0)), extractValue(_ctx.value(1))};
        } else if(_ctx.controlOpcode3() != null){
            _opcode = _ctx.controlOpcode3().getText();
            _args = new String[]{extractLabel(_ctx.lb())};
        } else if(_ctx.controlOpcode4() != null){
            _opcode = _ctx.controlOpcode4().getText();
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



        addInstruction("grshift_wob"            , "global_SR" , "ctl" , grshiftWobValueFormat, "INSTR_GLOBAL_SHIFT_right_WOB");
        addInstruction("grshift_wbz"            , "global_SR" , "ctl" , grshiftWobValueFormat, "INSTR_GLOBAL_SHIFT_right_WBZ");
        addInstruction("grshift_wbh"            , "global_SR" , "ctl" , grshiftWobValueFormat, "INSTR_GLOBAL_SHIFT_right_WBH");

        addInstruction("vleftins"               , "global_SR" , "ctl" , standardOneArgValueFormat);

        addInstruction("crightins"              , "global_SR" , "ctl" , grshiftWobValueFormat, "INSTR_GLOBAL_RIGHT_INS");
        addInstruction("cleftins"               , "global_SR" , "ctl" , grshiftWobValueFormat, "INSTR_GLOBAL_LEFT_INS");

        addInstruction("right_redins"           , "global_SR" , "ctl" , grshiftWobValueFormat, "INSTR_GLOBAL_RIGHT_RED_INS");
        addInstruction("left_redins"            , "global_SR" , "ctl" , grshiftWobValueFormat, "INSTR_GLOBAL_LEFT_RED_INS");
        addInstruction("glshift_wob"            , "global_SR" , "ctl" , grshiftWobValueFormat, "INSTR_GLOBAL_SHIFT_left_WOB");
        addInstruction("glshift_wbz"            , "global_SR" , "ctl" , grshiftWobValueFormat, "INSTR_GLOBAL_SHIFT_left_WBZ");
        addInstruction("glshift_wbh"            , "global_SR" , "ctl" , grshiftWobValueFormat, "INSTR_GLOBAL_SHIFT_left_WBH");

        addInstruction("rotateright"            , "global_SR" , "ctl" , grshiftWobValueFormat, "INSTR_GLOBAL_ROTATE_right");
        addInstruction("rotateleft"             , "global_SR" , "ctl" , grshiftWobValueFormat, "INSTR_GLOBAL_ROTATE_left");


        addInstruction("jmp"            , "jmp"                , "ctl"       , jmpValueFormat, new String[]{ "INSTR_JMP_FUNCTION_JMP", "ZERO", "ARG0"});
        addInstruction("brz"            , "jmp"                , "ctl"       , jmpValueFormat, new String[]{ "INSTR_JMP_FUNCTION_BRZ", "ZERO", "ARG0"});
        addInstruction("brnz"           , "jmp"                , "ctl"       , jmpValueFormat, new String[]{ "INSTR_JMP_FUNCTION_BRNZ", "ZERO", "ARG0"});
        addInstruction("brsgn"          , "jmp"                , "ctl"       , jmpValueFormat, new String[]{ "INSTR_JMP_FUNCTION_BRSGN", "ZERO", "ARG0"});
        addInstruction("brnsgn"         , "jmp"                , "ctl"       , jmpValueFormat, new String[]{ "INSTR_JMP_FUNCTION_BRNSGN", "ZERO", "ARG0"});
        addInstruction("brzdec"         , "jmp"                , "ctl"       , jmpValueFormat, new String[]{ "INSTR_JMP_FUNCTION_BRZDEC", "ZERO", "ARG0"});
        addInstruction("brnzdec"        , "jmp"                , "ctl"       , jmpValueFormat, new String[]{ "INSTR_JMP_FUNCTION_BRNZDEC", "ZERO", "ARG0"});
        addInstruction("brbool"         , "jmp"                , "ctl"       , jmpValueFormat, new String[]{ "INSTR_JMP_FUNCTION_BRBOOL", "ZERO", "ARG0"});
        addInstruction("brnbool"        , "jmp"                , "ctl"       , jmpValueFormat, new String[]{ "INSTR_JMP_FUNCTION_BRNBOOL", "ZERO", "ARG0"});
        addInstruction("brcr"           , "jmp"                , "ctl"       , jmpValueFormat, new String[]{ "INSTR_JMP_FUNCTION_BRCR", "ZERO", "ARG0"});
        addInstruction("brncr"          , "jmp"                , "ctl"       , jmpValueFormat, new String[]{ "INSTR_JMP_FUNCTION_BRNCR", "ZERO", "ARG0"});
        addInstruction("brcr_delayed"   , "jmp"                , "ctl"       , jmpValueFormat, new String[]{ "INSTR_JMP_FUNCTION_BRCR_delayed", "ZERO", "ARG0"});
        addInstruction("brncr_delayed"  , "jmp"                , "ctl"       , jmpValueFormat, new String[]{ "INSTR_JMP_FUNCTION_BRNCR_delayed", "ZERO", "ARG0"});
        addInstruction("brvalz"         , "jmp"                , "ctl"       , jmpValueFormat, new String[]{ "INSTR_JMP_FUNCTION_BRCMPVal", "ZERO", "ARG0"});
        addInstruction("brvalnz"        , "jmp"                , "ctl"       , jmpValueFormat, new String[]{ "INSTR_JMP_FUNCTION_BRCMPNVal", "ZERO", "ARG0"});
        addInstruction("brvalsgn"       , "jmp"                , "ctl"       , jmpValueFormat, new String[]{ "INSTR_JMP_FUNCTION_BRCMPValDEC", "ZERO", "ARG0"});
        addInstruction("brvalnsgn"      , "jmp"                , "ctl"       , jmpValueFormat, new String[]{ "INSTR_JMP_FUNCTION_BRCMPNValDEC", "ZERO", "ARG0"});
        addInstruction("brvalzdec"      , "jmp"                , "ctl"       , jmpValueFormat, new String[]{ "INSTR_JMP_FUNCTION_BRValZ", "ARG1", "ARG0"});
        addInstruction("brvalnzdec"     , "jmp"                , "ctl"       , jmpValueFormat, new String[]{ "INSTR_JMP_FUNCTION_BRValNZ", "ARG1", "ARG0"});
        addInstruction("brcmpval"       , "jmp"                , "ctl"       , jmpValueFormat, new String[]{ "INSTR_JMP_FUNCTION_BRValSGN", "ARG1", "ARG0"});
        addInstruction("brcmpnval"      , "jmp"                , "ctl"       , jmpValueFormat, new String[]{ "INSTR_JMP_FUNCTION_BRValNSGN", "ARG1", "ARG0"});
        addInstruction("brcmpvaldec"    , "jmp"                , "ctl"       , jmpValueFormat, new String[]{ "INSTR_JMP_FUNCTION_BRValZDEC", "ARG1", "ARG0"});
        addInstruction("brcmpnvaldec"   , "jmp"                , "ctl"       , jmpValueFormat, new String[]{ "INSTR_JMP_FUNCTION_BRValNZDEC", "ARG1", "ARG0"});
        addInstruction("halt"           , "jmp"                , "ctl"       , jmpValueFormat, new String[]{ "INSTR_JMP_FUNCTION_JMP", "ZERO", "ZERO"});

        addInstruction("float"          , "float"              , "ctl"       , standardOneArgValueFormat);

        addInstruction("sel_addrreg"    , "misc_store_load"    , "ctl"       , selAddrregValueFormat        , new String[]{"INSTR_MISC_STORE_LOAD_set_addrreg_selector", "ARG0"});

        addInstruction("cc_start_w_halt"   , "misc_testing"       , "ctl"    , ccStartWHaltValueFormat      , new String[]{"INSTR_MISC_TESTING_SEL_CC_START_W_HALT"});
        addInstruction("cc_start_wo_halt"  , "misc_testing"       , "ctl"    , ccStartWHaltValueFormat      , new String[]{"INSTR_MISC_TESTING_SEL_CC_START_WO_HALT"});
        addInstruction("cc_stop"           , "misc_testing"       , "ctl"    , ccStartWHaltValueFormat      , new String[]{"INSTR_MISC_TESTING_SEL_CC_STOP"});
        addInstruction("cc_reset"          , "misc_testing"       , "ctl"    , ccStartWHaltValueFormat      , new String[]{"INSTR_MISC_TESTING_SEL_CC_RESET"});

        addInstruction("srstore"           , "misc_store_load"    , "ctl"    , addrStoreValueFormat         , new String[]{"INSTR_MISC_STORE_LOAD_srstore"});
        addInstruction("addrstore"         , "misc_store_load"    , "ctl"    , addrStoreValueFormat         , new String[]{"INSTR_MISC_STORE_LOAD_addrstore"});
        addInstruction("setdec"            , "misc_store_load"    , "ctl"    , addrStoreValueFormat         , new String[]{"INSTR_MISC_STORE_LOAD_setdec"});
        addInstruction("setval"            , "misc_store_load"    , "ctl"    , setvalValueFormat            , new String[]{"INSTR_MISC_STORE_LOAD_set_loop_counter_value", "ARG0"});

        addInstruction("addrload"          , "misc_store_load"    , "ctl"    , ixLoadValueFormat            , new String[]{"INSTR_MISC_STORE_LOAD_addrload"});

        addInstruction("stack_pop"         , "stack_operations"   , "ctl"    , stackPopValueFormat          , new String[]{"ISA_stack_operations_CTL_val_POP"});
        addInstruction("stack_duplicate"   , "stack_operations"   , "ctl"    , stackPopValueFormat          , new String[]{"ISA_stack_operations_CTL_val_DUPLICATE"});
        addInstruction("stack_over"        , "stack_operations"   , "ctl"    , stackPopValueFormat          , new String[]{"ISA_stack_operations_CTL_val_OVER"});
        addInstruction("stack_swap"        , "stack_operations"   , "ctl"    , stackPopValueFormat          , new String[]{"ISA_stack_operations_CTL_val_SWAP"});
        addInstruction("stack_load_layer1" , "stack_operations"   , "ctl"    , stackPopValueFormat          , new String[]{"ISA_stack_operations_CTL_val_LOAD_LAYRER1"});

//##################################################################################################
//        addInstruction("prun"         , "prun"         , "ctl");
//        addInstruction("pload"        , "pload       , "ctl);
        addInstruction("param"        , "param"        , "ctl");
        addInstruction("setint"       , "sendint"      , "ctl");
        addInstruction("waitmatw"     , "waitmatw"     , "ctl"    , standardOneArgValueFormat);
        addInstruction("resready"     , "resready"     , "ctl");

    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
