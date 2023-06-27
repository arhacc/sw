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
public class ControllerInstructionBuilder extends InstructionBuilder {

//-------------------------------------------------------------------------------------
    public ControllerInstructionBuilder(Context _context, String _architectureId) {
        super(_context, _architectureId);
    }

//-------------------------------------------------------------------------------------
    public Instruction build(AsmParser.ControllerInstructionContext _ctx, Callable _callable) {
//        log.debug("InstructionBuilder: " + _opcode + ", " + _valueString + ", " + _valueNumber);
        Triple<String, String, AsmParser.ExpressionContext> _opcodeAndArgs = extractOpcodeAndArgs(_ctx);
        return build(_opcodeAndArgs.getLeft(), _opcodeAndArgs.getMiddle(), _opcodeAndArgs.getRight());
    }   

//-------------------------------------------------------------------------------------
    public Triple<String, String, AsmParser.ExpressionContext> extractOpcodeAndArgs(AsmParser.ControllerInstructionContext _ctx) {
        String _opcode = null;
        String _label = null;
        AsmParser.ExpressionContext _expression = null;
        if(_ctx.controllerOpcode0() != null){
            _opcode = _ctx.controllerOpcode0().getText();
            _expression = ZERO;
        } else if(_ctx.controllerOpcode1() != null){
            _opcode = _ctx.controllerOpcode1().getText();
            _expression = _ctx.expression();
        } else if(_ctx.controllerOpcode2() != null){
            _opcode = _ctx.controllerOpcode2().getText();
            _label = extractLabel(_ctx.lb());
        } else if(_ctx.controllerOpcode3() != null){
            _opcode = _ctx.controllerOpcode3().getText();
            _label = extractLabel(_ctx.lb());
            _expression = _ctx.expression();
        } else {
            log.error("Error building instruction!");
            _opcode = "";
        }

        return Triple.of(_opcode, _label, _expression);
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
        addInstruction("caddrinc"     , "addrinc",     "cop", "standardValueFormat", new String[]{"ARG0:NUMBER"});

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
        addInstruction("insval"                 , "insval"                            , "ctl"    , "standardValueFormat", new String[]{"ARG0:NUMBER"});

        addInstruction("rotright_local"         , "rotate_local"                      , "ctl"    , "rotrightLocalValueFormat", new String[]{"INSTR_SH_ROT_CTRL_bits_RIGHT_ROTATE", "DATA_SIZE – ARG0:NUMBER – 1" , "ARG0:NUMBER"});
        addInstruction("rotleft_local"          , "rotate_local"                      , "ctl"    , "rotrightLocalValueFormat", new String[]{"INSTR_SH_ROT_CTRL_bits_LEFT_ROTATE" , "ARG0:NUMBER - 1"             , "ARG0:NUMBER"});
        addInstruction("shift_right"            , "rotate_local"                      , "ctl"    , "rotrightLocalValueFormat", new String[]{"INSTR_SH_ROT_CTRL_bits_RIGHT_SHIFT" , "DATA_SIZE – ARG0:NUMBER – 1" , "ARG0:NUMBER"});
        addInstruction("shift_left"             , "rotate_local"                      , "ctl"    , "rotrightLocalValueFormat", new String[]{"INSTR_SH_ROT_CTRL_bits_LEFT_SHIFT"  , "ARG0:NUMBER - 1"             , "ARG0:NUMBER"});



        addInstruction("grshift_wob"            , "global_SR" , "ctl" , "grshiftWobValueFormat", "INSTR_GLOBAL_SHIFT_right_WOB");
        addInstruction("grshift_wbz"            , "global_SR" , "ctl" , "grshiftWobValueFormat", "INSTR_GLOBAL_SHIFT_right_WBZ");
        addInstruction("grshift_wbh"            , "global_SR" , "ctl" , "grshiftWobValueFormat", "INSTR_GLOBAL_SHIFT_right_WBH");

        addInstruction("vleftins"               , "global_SR" , "val" , "standardValueFormat", new String[]{"ARG0:NUMBER"});

        addInstruction("crightins"              , "global_SR" , "ctl" , "grshiftWobValueFormat", "INSTR_GLOBAL_RIGHT_INS");
        addInstruction("cleftins"               , "global_SR" , "ctl" , "grshiftWobValueFormat", "INSTR_GLOBAL_LEFT_INS");

        addInstruction("right_redins"           , "global_SR" , "ctl" , "grshiftWobValueFormat", "INSTR_GLOBAL_RIGHT_RED_INS");
        addInstruction("left_redins"            , "global_SR" , "ctl" , "grshiftWobValueFormat", "INSTR_GLOBAL_LEFT_RED_INS");
        addInstruction("glshift_wob"            , "global_SR" , "ctl" , "grshiftWobValueFormat", "INSTR_GLOBAL_SHIFT_left_WOB");
        addInstruction("glshift_wbz"            , "global_SR" , "ctl" , "grshiftWobValueFormat", "INSTR_GLOBAL_SHIFT_left_WBZ");
        addInstruction("glshift_wbh"            , "global_SR" , "ctl" , "grshiftWobValueFormat", "INSTR_GLOBAL_SHIFT_left_WBH");

        addInstruction("rotateright"            , "global_SR" , "ctl" , "grshiftWobValueFormat", "INSTR_GLOBAL_ROTATE_right");
        addInstruction("rotateleft"             , "global_SR" , "ctl" , "grshiftWobValueFormat", "INSTR_GLOBAL_ROTATE_left");


        addInstruction("jmp"            , "jmp"                , "ctl"       , "jmpValueFormat", new String[]{ "INSTR_JMP_FUNCTION_JMP", "ZERO", "ARG0:LABEL"});
        addInstruction("brz"            , "jmp"                , "ctl"       , "jmpValueFormat", new String[]{ "INSTR_JMP_FUNCTION_BRZ", "ZERO", "ARG0:LABEL"});
        addInstruction("brnz"           , "jmp"                , "ctl"       , "jmpValueFormat", new String[]{ "INSTR_JMP_FUNCTION_BRNZ", "ZERO", "ARG0:LABEL"});
        addInstruction("brsgn"          , "jmp"                , "ctl"       , "jmpValueFormat", new String[]{ "INSTR_JMP_FUNCTION_BRSGN", "ZERO", "ARG0:LABEL"});
        addInstruction("brnsgn"         , "jmp"                , "ctl"       , "jmpValueFormat", new String[]{ "INSTR_JMP_FUNCTION_BRNSGN", "ZERO", "ARG0:LABEL"});
        addInstruction("brzdec"         , "jmp"                , "ctl"       , "jmpValueFormat", new String[]{ "INSTR_JMP_FUNCTION_BRZDEC", "ZERO", "ARG0:LABEL"});
        addInstruction("brnzdec"        , "jmp"                , "ctl"       , "jmpValueFormat", new String[]{ "INSTR_JMP_FUNCTION_BRNZDEC", "ZERO", "ARG0:LABEL"});
        addInstruction("brbool"         , "jmp"                , "ctl"       , "jmpValueFormat", new String[]{ "INSTR_JMP_FUNCTION_BRBOOL", "ZERO", "ARG0:LABEL"});
        addInstruction("brnbool"        , "jmp"                , "ctl"       , "jmpValueFormat", new String[]{ "INSTR_JMP_FUNCTION_BRNBOOL", "ZERO", "ARG0:LABEL"});
        addInstruction("brcr"           , "jmp"                , "ctl"       , "jmpValueFormat", new String[]{ "INSTR_JMP_FUNCTION_BRCR", "ZERO", "ARG0:LABEL"});
        addInstruction("brncr"          , "jmp"                , "ctl"       , "jmpValueFormat", new String[]{ "INSTR_JMP_FUNCTION_BRNCR", "ZERO", "ARG0:LABEL"});
        addInstruction("brcr_delayed"   , "jmp"                , "ctl"       , "jmpValueFormat", new String[]{ "INSTR_JMP_FUNCTION_BRCR_delayed", "ZERO", "ARG0:LABEL"});
        addInstruction("brncr_delayed"  , "jmp"                , "ctl"       , "jmpValueFormat", new String[]{ "INSTR_JMP_FUNCTION_BRNCR_delayed", "ZERO", "ARG0:LABEL"});
        addInstruction("brvalz"         , "jmp"                , "ctl"       , "jmpValueFormat", new String[]{ "INSTR_JMP_FUNCTION_BRValZ", "ARG1:NUMBER", "ARG0:LABEL"});
        addInstruction("brvalnz"        , "jmp"                , "ctl"       , "jmpValueFormat", new String[]{ "INSTR_JMP_FUNCTION_BRValNZ", "ARG1:NUMBER", "ARG0:LABEL"});
        addInstruction("brvalsgn"       , "jmp"                , "ctl"       , "jmpValueFormat", new String[]{ "INSTR_JMP_FUNCTION_BRValSGN", "ARG1:NUMBER", "ARG0:LABEL"});
        addInstruction("brvalnsgn"      , "jmp"                , "ctl"       , "jmpValueFormat", new String[]{ "INSTR_JMP_FUNCTION_BRValNSGN", "ARG1:NUMBER", "ARG0:LABEL"});
        addInstruction("brvalzdec"      , "jmp"                , "ctl"       , "jmpValueFormat", new String[]{ "INSTR_JMP_FUNCTION_BRValZDEC", "ARG1:NUMBER", "ARG0:LABEL"});
        addInstruction("brvalnzdec"     , "jmp"                , "ctl"       , "jmpValueFormat", new String[]{ "INSTR_JMP_FUNCTION_BRValNZDEC", "ARG1:NUMBER", "ARG0:LABEL"});
        addInstruction("brcmpval"       , "jmp"                , "ctl"       , "jmpValueFormat", new String[]{ "INSTR_JMP_FUNCTION_BRCMPVal", "ARG1:NUMBER", "ARG0:LABEL"});
        addInstruction("brcmpnval"      , "jmp"                , "ctl"       , "jmpValueFormat", new String[]{ "INSTR_JMP_FUNCTION_BRCMPNVal", "ARG1:NUMBER", "ARG0:LABEL"});
        addInstruction("brcmpvaldec"    , "jmp"                , "ctl"       , "jmpValueFormat", new String[]{ "INSTR_JMP_FUNCTION_BRCMPValDEC", "ARG1:NUMBER", "ARG0:LABEL"});
        addInstruction("brcmpnvaldec"   , "jmp"                , "ctl"       , "jmpValueFormat", new String[]{ "INSTR_JMP_FUNCTION_BRCMPNValDEC", "ARG1:NUMBER", "ARG0:LABEL"});
        addInstruction("halt"           , "jmp"                , "ctl"       , "jmpValueFormat", new String[]{ "INSTR_JMP_FUNCTION_JMP", "ZERO", "ZERO"});

        addInstruction("float"          , "float"              , "ctl"       , "standardValueFormat", new String[]{"ARG0:NUMBER"});

        addInstruction("sel_addrreg"    , "misc_store_load"    , "ctl"       , "selAddrregValueFormat"        , new String[]{"INSTR_MISC_STORE_LOAD_set_addrreg_selector", "ARG0:NUMBER"});

        addInstruction("cc_start_w_halt"   , "misc_testing"       , "ctl"    , "ccStartWHaltValueFormat"      , new String[]{"INSTR_MISC_TESTING_SEL_CC_START_W_HALT"});
        addInstruction("cc_start_wo_halt"  , "misc_testing"       , "ctl"    , "ccStartWHaltValueFormat"      , new String[]{"INSTR_MISC_TESTING_SEL_CC_START_WO_HALT"});
        addInstruction("cc_stop"           , "misc_testing"       , "ctl"    , "ccStartWHaltValueFormat"      , new String[]{"INSTR_MISC_TESTING_SEL_CC_STOP"});
        addInstruction("cc_reset"          , "misc_testing"       , "ctl"    , "ccStartWHaltValueFormat"      , new String[]{"INSTR_MISC_TESTING_SEL_CC_RESET"});

        addInstruction("srstore"           , "misc_store_load"    , "ctl"    , "addrStoreValueFormat"         , new String[]{"INSTR_MISC_STORE_LOAD_srstore"});
        addInstruction("addrstore"         , "misc_store_load"    , "ctl"    , "addrStoreValueFormat"         , new String[]{"INSTR_MISC_STORE_LOAD_addrstore"});
        addInstruction("setdec"            , "misc_store_load"    , "ctl"    , "addrStoreValueFormat"         , new String[]{"INSTR_MISC_STORE_LOAD_setdec"});
        addInstruction("setval"            , "misc_store_load"    , "ctl"    , "setvalValueFormat"            , new String[]{"INSTR_MISC_STORE_LOAD_set_loop_counter_value", "ARG0:NUMBER"});

        addInstruction("addrload"          , "misc_store_load"    , "ctl"    , "ixLoadValueFormat"            , new String[]{"INSTR_MISC_STORE_LOAD_addrload"});

        addInstruction("stack_pop"         , "stack_operations"   , "ctl"    , "stackPopValueFormat"          , new String[]{"ISA_stack_operations_CTL_val_POP"});
        addInstruction("stack_duplicate"   , "stack_operations"   , "ctl"    , "stackPopValueFormat"          , new String[]{"ISA_stack_operations_CTL_val_DUPLICATE"});
        addInstruction("stack_over"        , "stack_operations"   , "ctl"    , "stackPopValueFormat"          , new String[]{"ISA_stack_operations_CTL_val_OVER"});
        addInstruction("stack_swap"        , "stack_operations"   , "ctl"    , "stackPopValueFormat"          , new String[]{"ISA_stack_operations_CTL_val_SWAP"});
        addInstruction("stack_load_layer1" , "stack_operations"   , "ctl"    , "stackPopValueFormat"          , new String[]{"ISA_stack_operations_CTL_val_LOAD_LAYRER1"});

//##################################################################################################
//        addInstruction("prun"         , "prun"         , "ctl");
//        addInstruction("pload"        , "pload       , "ctl);
        addInstruction("param"        , "param"        , "ctl");
        addInstruction("setint"       , "sendint"      , "ctl");
        addInstruction("waitmatw"     , "waitmatw"     , "ctl"    , "standardValueFormat", new String[]{"ARG0:NUMBER"});
        addInstruction("resready"     , "resready"     , "ctl");

    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
