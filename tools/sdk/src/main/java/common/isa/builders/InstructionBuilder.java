//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.isa.builders;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import com.opencsv.*;
import com.opencsv.exceptions.*;

import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.isa.*;
import xpu.sw.tools.sdk.common.context.*;

//-------------------------------------------------------------------------------------
public class InstructionBuilder extends Builder {
    private String arhCode;
    private Map<String, Instruction> instructions;
    private OpcodeBuilder opcodeBuilder;
    private OperandBuilder operandBuilder;
    private ValueBuilder valueBuilder;

//-------------------------------------------------------------------------------------
    public InstructionBuilder(Context _context, String _arhCode) {
        super(_context);
        arhCode = _arhCode;
        instructions = new HashMap<String, Instruction>();
        opcodeBuilder = new OpcodeBuilder(_context, _arhCode);
        operandBuilder = new OperandBuilder(_context, _arhCode);
        valueBuilder = new ValueBuilder(_context, _arhCode);
        init();        
    }

//-------------------------------------------------------------------------------------
    public Instruction build(String _opcode, String _valueString, int _valueNumber, Primitive _primitive) {
//        log.debug("InstructionBuilder: " + _opcode + ", " + _valueString + ", " + _valueNumber);
        Instruction _instruction = instructions.get(_opcode);
/*        Opcode _opcodeField = (Opcode)_fields[0];
        Operand _operandField = (Operand)_fields[1];
        Value _valueField;
        if(_fields.length > 2){
            _valueField = (Value)_fields[2];
        } else {
            _valueField = new Value(_valueString, _valueNumber);
        }*/
        if(_instruction == null){
            log.error("Error: cannot find instruction: " + _opcode);
            System.exit(1);
        }
        _instruction = _instruction.clone();
        _instruction.setPrimitive(_primitive);
        _instruction.setValues(_valueString, _valueNumber);
        return _instruction;
    }   
/*
//-------------------------------------------------------------------------------------
    public Opcode getOpcode(int _opcode) {
        return opcodeBuilder.get(_opcode);
    }
*/
//-------------------------------------------------------------------------------------
    private void init() {
        addInstruction("vadd"         , "add",     "val");
        addInstruction("add"          , "add",     "mab");
        addInstruction("radd"         , "add",     "mrl");
        addInstruction("riadd"        , "add",     "mri");
        addInstruction("cadd"         , "add",     "cop");
        addInstruction("caadd"        , "add",     "mac");
        addInstruction("cradd"        , "add",     "mrc");
 
        addInstruction("vaddc"        , "addc",    "val");
        addInstruction("addc"         , "addc",    "mab");
        addInstruction("raddc"        , "addc",    "mrl");
        addInstruction("riaddc"       , "addc",    "mri");
        addInstruction("caddc"        , "addc",    "cop");
        addInstruction("caaddc"       , "addc",    "mac");
        addInstruction("craddc"       , "addc",    "mrc");
           
        addInstruction("vsub"         , "sub",    "val");
        addInstruction("sub"          , "sub",    "mab");
        addInstruction("rsub"         , "sub",    "mrl");
        addInstruction("risub"        , "sub",    "mri");
        addInstruction("csub"         , "sub",    "cop");
        addInstruction("casub"        , "sub",    "mac");
        addInstruction("crsub"        , "sub",    "mrc");

        addInstruction("vrvsub"       , "rsub",    "val");
        addInstruction("rvsub"        , "rsub",    "mab");
        addInstruction("rrvsub"       , "rsub",    "mrl");
        addInstruction("rirvsub"      , "rsub",    "mri");
        addInstruction("crvsub"       , "rsub",    "cop");
        addInstruction("carvsub"      , "rsub",    "mac");
        addInstruction("crrvsub"      , "rsub",    "mrc");

        addInstruction("vsubc"        , "subc",    "val");
        addInstruction("subc"         , "subc",    "mab");
        addInstruction("rsubc"        , "subc",    "mrl");
        addInstruction("risubc"       , "subc",    "mri");
        addInstruction("csubc"        , "subc",    "cop");
        addInstruction("casubc"       , "subc",    "mac");
        addInstruction("crsubc"       , "subc",    "mrc");
           
        addInstruction("vrvsubc"      , "rsubc",    "val");
        addInstruction("rvsubc"       , "rsubc",    "mab");
        addInstruction("rrvsubc"      , "rsubc",    "mrl");
        addInstruction("rirvsubc"     , "rsubc",    "mri");
        addInstruction("crvsubc"      , "rsubc",    "cop");
        addInstruction("carvsubc"     , "rsubc",    "mac");
        addInstruction("crrvsubc"     , "rsubc",    "mrc");
           
        addInstruction("vmult"        , "mult",    "val");
        addInstruction("mult"         , "mult",    "mab");
        addInstruction("rmult"        , "mult",    "mrl");
        addInstruction("rimult"       , "mult",    "mri");
        addInstruction("cmult"        , "mult",    "cop");
        addInstruction("camult"       , "mult",    "mac");
        addInstruction("crmult"       , "mult",    "mrc");
           
        addInstruction("vand"         , "bwand",    "val");
        addInstruction("and"          , "bwand",    "mab");
        addInstruction("rand"         , "bwand",    "mrl");
        addInstruction("riand"        , "bwand",    "mri");
        addInstruction("cand"         , "bwand",    "cop");
        addInstruction("caand"        , "bwand",    "mac");
        addInstruction("crand"        , "bwand",    "mrc");
           
        addInstruction("vor"          , "bwor",    "val");
        addInstruction("or"           , "bwor",    "mab");
        addInstruction("ror"          , "bwor",    "mrl");
        addInstruction("rior"         , "bwor",    "mri");
        addInstruction("cor"          , "bwor",    "cop");
        addInstruction("caor"         , "bwor",    "mac");
        addInstruction("cror"         , "bwor",    "mrc");
           
        addInstruction("vxor"         , "bwxor",    "val");
        addInstruction("xor"          , "bwxor",    "mab");
        addInstruction("rxor"         , "bwxor",    "mrl");
        addInstruction("rixor"        , "bwxor",    "mri");
        addInstruction("cxor"         , "bwxor",    "cop");
        addInstruction("caxor"        , "bwxor",    "mac");
        addInstruction("crxor"        , "bwxor",    "mrc");
           
        addInstruction("vload"        , "load",      "val");
        addInstruction("load"         , "load",      "mab");
        addInstruction("rload"        , "load",      "mrl");
        addInstruction("riload"       , "load",      "mri");
        addInstruction("cload"        , "load",      "cop");
        addInstruction("caload"       , "load",      "mac");
        addInstruction("crload"       , "load",      "mrc");
        addInstruction("ixload"       , "misc_store_load",      "ctl");
        addInstruction("srload"       , "misc_store_load",      "ctl");
        addInstruction("scanload"     , "misc_store_load",      "ctl");
        addInstruction("addrload"     , "misc_store_load",      "ctl");
//        addInstruction("ioload"       , "ioload,      "ctl);
        addInstruction("insval"       , "insval",      "ctl");
        
//        addInstruction("addrld"       , "addrld   ,   "ctl);
        addInstruction("store"        , "store"    ,   "mab");
        addInstruction("rstore"       , "store"    ,   "mrl");
        addInstruction("ristore"      , "store"    ,   "mri");
        addInstruction("cstore"       , "store"    ,   "mac");
        addInstruction("srstore"      , "misc_store_load"  ,   "ctl");
//        addInstruction("iostore"      , "iostore  ,   "ctl);
        addInstruction("insertio"     , "misc_store_load" ,   "ctl");

        addInstruction("compare"      , "compare" , "mab");
        addInstruction("rcompare"     , "compare" , "mrl");
        addInstruction("ricompare"    , "compare" , "mri");
        addInstruction("vcompare"     , "compare" , "val");
        addInstruction("ccompare"     , "compare" , "cop");
        addInstruction("cacompare"    , "compare" , "mac");
        addInstruction("crcompare"    , "compare" , "mrc");
    
        addInstruction("shright"      , "right_fixed_shifting"   , "ctl");
        addInstruction("shrightc"     , "right_fixed_shifting"   , "ctl");
        addInstruction("sharight"     , "right_fixed_shifting"   , "ctl");
        addInstruction("shright_fixed_amount"     , "right_fixed_shifting"   , "ctl");
        addInstruction("sharight_fixed_amount"     , "right_fixed_shifting"   , "ctl");

        addInstruction("rotright"     , "rotate_local"     , "ctl");
        addInstruction("shr"          , "rotate_local"     , "ctl");
        addInstruction("shl"          , "rotate_local"     , "ctl");

//          ARRAY CONTROL INSTRUCTIONS #########
        addInstruction("wherezero"    , "spatial_select"      , "ctl");
        addInstruction("wherecarry"   , "spatial_select"      , "ctl");
        addInstruction("wherefirst"   , "spatial_select"      , "ctl");
        addInstruction("wherenext"    , "spatial_select"      , "ctl");
        addInstruction("whereprev"    , "spatial_select"      , "ctl");
        addInstruction("wherenzero"   , "spatial_select"      , "ctl");
        addInstruction("wherencarry"  , "spatial_select"      , "ctl");
        addInstruction("wherenfirst"  , "spatial_select"      , "ctl");
        addInstruction("wherennext"   , "spatial_select"      , "ctl");
        addInstruction("wherenprev"   , "spatial_select"      , "ctl");
        addInstruction("elsewhere"    , "spatial_select"      , "ctl");
        addInstruction("endwhere"     , "spatial_select"      , "ctl");
        addInstruction("activate"     , "spatial_select"      , "ctl");
/*        addInstruction("rednop"       , "SETRED              , "CTL);
        addInstruction("redadd"       , "SETRED              , "CTL);
        addInstruction("redmin"       , "SETRED              , "CTL);
        addInstruction("redmax"       , "SETRED              , "CTL);
*/

//##########################################
        addInstruction("nop"            , "add"                , "val");

//##################################################################################################
        addInstruction("jmp"            , "jmp"                , "ctl"       , "INSTR_JMP_FUNCTION_JMP          ");
        addInstruction("brz"            , "jmp"                , "ctl"       , "INSTR_JMP_FUNCTION_BRZ          ");
        addInstruction("brnz"           , "jmp"                , "ctl"       , "INSTR_JMP_FUNCTION_BRNZ         ");
        addInstruction("brsgn"          , "jmp"                , "ctl"       , "INSTR_JMP_FUNCTION_BRSGN        ");
        addInstruction("brnsgn"         , "jmp"                , "ctl"       , "INSTR_JMP_FUNCTION_BRNSGN       ");
        addInstruction("brzdec"         , "jmp"                , "ctl"       , "INSTR_JMP_FUNCTION_BRZDEC       ");
        addInstruction("brnzdec"        , "jmp"                , "ctl"       , "INSTR_JMP_FUNCTION_BRNZDEC      ");
        addInstruction("brbool"         , "jmp"                , "ctl"       , "INSTR_JMP_FUNCTION_BRBOOL       ");
        addInstruction("brnbool"        , "jmp"                , "ctl"       , "INSTR_JMP_FUNCTION_BRNBOOL      ");
        addInstruction("brcr"           , "jmp"                , "ctl"       , "INSTR_JMP_FUNCTION_BRCR         ");
        addInstruction("brncr"          , "jmp"                , "ctl"       , "INSTR_JMP_FUNCTION_BRNCR        ");
        addInstruction("brcr_delayed"   , "jmp"                , "ctl"       , "INSTR_JMP_FUNCTION_BRCR_delayed ");
        addInstruction("brncr_delayed"  , "jmp"                , "ctl"       , "INSTR_JMP_FUNCTION_BRNCR_delayed");
        addInstruction("brvalz"         , "jmp"                , "ctl"       , "INSTR_JMP_FUNCTION_BRCMPVal     ");
        addInstruction("brvalnz"        , "jmp"                , "ctl"       , "INSTR_JMP_FUNCTION_BRCMPNVal    ");
        addInstruction("brvalsgn"       , "jmp"                , "ctl"       , "INSTR_JMP_FUNCTION_BRCMPValDEC  ");
        addInstruction("brvalnsgn"      , "jmp"                , "ctl"       , "INSTR_JMP_FUNCTION_BRCMPNValDEC ");
        addInstruction("brvalzdec"      , "jmp"                , "ctl"       , "INSTR_JMP_FUNCTION_BRValZ       ");
        addInstruction("brvalnzdec"     , "jmp"                , "ctl"       , "INSTR_JMP_FUNCTION_BRValNZ      ");
        addInstruction("brcmpval"       , "jmp"                , "ctl"       , "INSTR_JMP_FUNCTION_BRValSGN     ");
        addInstruction("brcmpnval"      , "jmp"                , "ctl"       , "INSTR_JMP_FUNCTION_BRValNSGN    ");
        addInstruction("brcmpvaldec"    , "jmp"                , "ctl"       , "INSTR_JMP_FUNCTION_BRValZDEC    ");
        addInstruction("brcmpnvaldec"   , "jmp"                , "ctl"       , "INSTR_JMP_FUNCTION_BRValNZDEC   ");
        addInstruction("halt"           , "jmp"                , "ctl"       , "INSTR_JMP_FUNCTION_JMP");



        addInstruction("start_w_halt"   , "misc_testing"       , "ctl"       , "Value.INSTR_MISC_TESTING_SEL_CC_START_W_HALT");
        addInstruction("start_wo_halt"  , "misc_testing"       , "ctl"       , "Value.INSTR_MISC_TESTING_SEL_CC_START_WO_HALT");
        addInstruction("stop"           , "misc_testing"       , "ctl"       , "Value.INSTR_MISC_TESTING_SEL_CC_STOP");
        addInstruction("reset"          , "misc_testing"       , "ctl"       , "Value.INSTR_MISC_TESTING_SEL_CC_RESET");

//##################################################################################################
        addInstruction("prun"         , "prun"         , "ctl");
//        addInstruction("pcheck"       , "pcheck       , "ctl);
        addInstruction("param"        , "param"        , "ctl");
//        addInstruction("getv"         , "poploop      , "ctl);
//        addInstruction("sendv"        , "pushloop     , "ctl);
        addInstruction("setint"       , "sendint"      , "ctl");

// late additions:
//'setval'|'waitmatw'|'resready'|'brcmpnvaldec'|'setdec'                
        addInstruction("setval"       , "misc_store_load"      , "ctl");
        addInstruction("setdec"       , "misc_store_load"      , "ctl");
        addInstruction("waitmatw"     , "waitmatw"             , "ctl");
        addInstruction("resready"     , "resready"             , "ctl");
        addInstruction("brcmpnvaldec" , "jmp"                  , "ctl");
    }

//-------------------------------------------------------------------------------------
    private void addInstruction(String _instructionName, String _opcode, String _operand) {
        addInstruction(_instructionName, _opcode, _operand, "");
    }

//-------------------------------------------------------------------------------------
    private void addInstruction(String _instructionName, String _opcode, String _operand, String _value) {
        Opcode _opcodeObj = opcodeBuilder.get(_opcode);
        Operand _operandObj = operandBuilder.get(_operand);
        Value _valueObj = valueBuilder.get(_value);

        Instruction _instruction = new Instruction(_instructionName, _opcodeObj, _operandObj, _valueObj);
        instructions.put(_instructionName, _instruction);
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
