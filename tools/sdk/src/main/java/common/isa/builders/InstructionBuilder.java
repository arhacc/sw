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
    private HashMap<String, Instruction> instructions;
    private HashMap<Integer, Opcode> opcodes;
    private HashMap<Integer, Operand> operands;

//-------------------------------------------------------------------------------------
    public InstructionBuilder(Context _context) {
        super(_context);
        instructions = new HashMap<String, Instruction>();
        opcodes = new HashMap<Integer, Opcode>();
        operands = new HashMap<Integer, Operand>();
        init();        
    }

//-------------------------------------------------------------------------------------
    public Instruction build(String _opcode, String _valueString, int _valueNumber, Primitive _primitive) {
        log.debug("InstructionBuilder: " + _opcode + ", " + _valueString + ", " + _valueNumber);
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

//-------------------------------------------------------------------------------------
    public Opcode getOpcode(int _opcode) {
        return opcodes.get(_opcode);
    }

//-------------------------------------------------------------------------------------
    private void init() {
        addOperand(Operand.VAL);
        addOperand(Operand.MAB);
        addOperand(Operand.MRL);
        addOperand(Operand.MRI);
        addOperand(Operand.COP);
        addOperand(Operand.MAC);
        addOperand(Operand.MRC);
        addOperand(Operand.CTL);

        addInstruction("vadd"         , Opcode.ADD,     Operand.VAL);
        addInstruction("add"          , Opcode.ADD,     Operand.MAB);
        addInstruction("radd"         , Opcode.ADD,     Operand.MRL);
        addInstruction("riadd"        , Opcode.ADD,     Operand.MRI);
        addInstruction("cadd"         , Opcode.ADD,     Operand.COP);
        addInstruction("caadd"        , Opcode.ADD,     Operand.MAC);
        addInstruction("cradd"        , Opcode.ADD,     Operand.MRC);
 
        addInstruction("vaddc"        , Opcode.ADDC,    Operand.VAL);
        addInstruction("addc"         , Opcode.ADDC,    Operand.MAB);
        addInstruction("raddc"        , Opcode.ADDC,    Operand.MRL);
        addInstruction("riaddc"       , Opcode.ADDC,    Operand.MRI);
        addInstruction("caddc"        , Opcode.ADDC,    Operand.COP);
        addInstruction("caaddc"       , Opcode.ADDC,    Operand.MAC);
        addInstruction("craddc"       , Opcode.ADDC,    Operand.MRC);
           
        addInstruction("vsub"         , Opcode.SUB,    Operand.VAL);
        addInstruction("sub"          , Opcode.SUB,    Operand.MAB);
        addInstruction("rsub"         , Opcode.SUB,    Operand.MRL);
        addInstruction("risub"        , Opcode.SUB,    Operand.MRI);
        addInstruction("csub"         , Opcode.SUB,    Operand.COP);
        addInstruction("casub"        , Opcode.SUB,    Operand.MAC);
        addInstruction("crsub"        , Opcode.SUB,    Operand.MRC);

        addInstruction("vrvsub"       , Opcode.RSUB,    Operand.VAL);
        addInstruction("rvsub"        , Opcode.RSUB,    Operand.MAB);
        addInstruction("rrvsub"       , Opcode.RSUB,    Operand.MRL);
        addInstruction("rirvsub"      , Opcode.RSUB,    Operand.MRI);
        addInstruction("crvsub"       , Opcode.RSUB,    Operand.COP);
        addInstruction("carvsub"      , Opcode.RSUB,    Operand.MAC);
        addInstruction("crrvsub"      , Opcode.RSUB,    Operand.MRC);

        addInstruction("vsubc"        , Opcode.SUBC,    Operand.VAL);
        addInstruction("subc"         , Opcode.SUBC,    Operand.MAB);
        addInstruction("rsubc"        , Opcode.SUBC,    Operand.MRL);
        addInstruction("risubc"       , Opcode.SUBC,    Operand.MRI);
        addInstruction("csubc"        , Opcode.SUBC,    Operand.COP);
        addInstruction("casubc"       , Opcode.SUBC,    Operand.MAC);
        addInstruction("crsubc"       , Opcode.SUBC,    Operand.MRC);
           
        addInstruction("vrvsubc"      , Opcode.RSUBC,    Operand.VAL);
        addInstruction("rvsubc"       , Opcode.RSUBC,    Operand.MAB);
        addInstruction("rrvsubc"      , Opcode.RSUBC,    Operand.MRL);
        addInstruction("rirvsubc"     , Opcode.RSUBC,    Operand.MRI);
        addInstruction("crvsubc"      , Opcode.RSUBC,    Operand.COP);
        addInstruction("carvsubc"     , Opcode.RSUBC,    Operand.MAC);
        addInstruction("crrvsubc"     , Opcode.RSUBC,    Operand.MRC);
           
        addInstruction("vmult"        , Opcode.MULT,    Operand.VAL);
        addInstruction("mult"         , Opcode.MULT,    Operand.MAB);
        addInstruction("rmult"        , Opcode.MULT,    Operand.MRL);
        addInstruction("rimult"       , Opcode.MULT,    Operand.MRI);
        addInstruction("cmult"        , Opcode.MULT,    Operand.COP);
        addInstruction("camult"       , Opcode.MULT,    Operand.MAC);
        addInstruction("crmult"       , Opcode.MULT,    Operand.MRC);
           
        addInstruction("vand"         , Opcode.BWAND,    Operand.VAL);
        addInstruction("and"          , Opcode.BWAND,    Operand.MAB);
        addInstruction("rand"         , Opcode.BWAND,    Operand.MRL);
        addInstruction("riand"        , Opcode.BWAND,    Operand.MRI);
        addInstruction("cand"         , Opcode.BWAND,    Operand.COP);
        addInstruction("caand"        , Opcode.BWAND,    Operand.MAC);
        addInstruction("crand"        , Opcode.BWAND,    Operand.MRC);
           
        addInstruction("vor"          , Opcode.BWOR,    Operand.VAL);
        addInstruction("or"           , Opcode.BWOR,    Operand.MAB);
        addInstruction("ror"          , Opcode.BWOR,    Operand.MRL);
        addInstruction("rior"         , Opcode.BWOR,    Operand.MRI);
        addInstruction("cor"          , Opcode.BWOR,    Operand.COP);
        addInstruction("caor"         , Opcode.BWOR,    Operand.MAC);
        addInstruction("cror"         , Opcode.BWOR,    Operand.MRC);
           
        addInstruction("vxor"         , Opcode.BWXOR,    Operand.VAL);
        addInstruction("xor"          , Opcode.BWXOR,    Operand.MAB);
        addInstruction("rxor"         , Opcode.BWXOR,    Operand.MRL);
        addInstruction("rixor"        , Opcode.BWXOR,    Operand.MRI);
        addInstruction("cxor"         , Opcode.BWXOR,    Operand.COP);
        addInstruction("caxor"        , Opcode.BWXOR,    Operand.MAC);
        addInstruction("crxor"        , Opcode.BWXOR,    Operand.MRC);
           
        addInstruction("vload"        , Opcode.LOAD  ,      Operand.VAL);
        addInstruction("load"         , Opcode.LOAD  ,      Operand.MAB);
        addInstruction("rload"        , Opcode.LOAD  ,      Operand.MRL);
        addInstruction("riload"       , Opcode.LOAD  ,      Operand.MRI);
        addInstruction("cload"        , Opcode.LOAD  ,      Operand.COP);
        addInstruction("caload"       , Opcode.LOAD  ,      Operand.MAC);
        addInstruction("crload"       , Opcode.LOAD  ,      Operand.MRC);
        addInstruction("ixload"       , Opcode.MISC_STORE_LOAD,      Operand.CTL);
        addInstruction("srload"       , Opcode.MISC_STORE_LOAD,      Operand.CTL);
        addInstruction("scanload"     , Opcode.MISC_STORE_LOAD,      Operand.CTL);
        addInstruction("addrload"     , Opcode.MISC_STORE_LOAD,      Operand.CTL);
//        addInstruction("ioload"       , Opcode.IOLOAD,      Operand.CTL);
        addInstruction("insval"       , Opcode.INSVAL,      Operand.CTL);
        
//        addInstruction("addrld"       , Opcode.ADDRLD   ,   Operand.CTL);
        addInstruction("store"        , Opcode.STORE    ,   Operand.MAB);
        addInstruction("rstore"       , Opcode.STORE    ,   Operand.MRL);
        addInstruction("ristore"      , Opcode.STORE    ,   Operand.MRI);
        addInstruction("cstore"       , Opcode.STORE    ,   Operand.MAC);
        addInstruction("srstore"      , Opcode.MISC_STORE_LOAD  ,   Operand.CTL);
//        addInstruction("iostore"      , Opcode.IOSTORE  ,   Operand.CTL);
        addInstruction("insertio"     , Opcode.MISC_STORE_LOAD ,   Operand.CTL);

        addInstruction("compare"      , Opcode.COMPARE , Operand.MAB);
        addInstruction("rcompare"     , Opcode.COMPARE , Operand.MRL);
        addInstruction("ricompare"    , Opcode.COMPARE , Operand.MRI);
        addInstruction("vcompare"     , Opcode.COMPARE , Operand.VAL);
        addInstruction("ccompare"     , Opcode.COMPARE , Operand.COP);
        addInstruction("cacompare"    , Opcode.COMPARE , Operand.MAC);
        addInstruction("crcompare"    , Opcode.COMPARE , Operand.MRC);
    
        addInstruction("shright"      , Opcode.RIGHT_FIXED_SHIFTING    , Operand.CTL);
        addInstruction("shrightc"     , Opcode.RIGHT_FIXED_SHIFTING   , Operand.CTL);
        addInstruction("sharight"     , Opcode.RIGHT_FIXED_SHIFTING   , Operand.CTL);
        addInstruction("shright_fixed_amount"     , Opcode.RIGHT_FIXED_SHIFTING   , Operand.CTL);
        addInstruction("sharight_fixed_amount"     , Opcode.RIGHT_FIXED_SHIFTING   , Operand.CTL);

        addInstruction("rotright"     , Opcode.ROTATE_LOCAL     , Operand.CTL);
        addInstruction("shr"          , Opcode.ROTATE_LOCAL     , Operand.CTL);
        addInstruction("shl"          , Opcode.ROTATE_LOCAL     , Operand.CTL);

//          ARRAY CONTROL INSTRUCTIONS #########
        addInstruction("wherezero"    , Opcode.SPATIAL_SELECT      , Operand.CTL);
        addInstruction("wherecarry"   , Opcode.SPATIAL_SELECT      , Operand.CTL);
        addInstruction("wherefirst"   , Opcode.SPATIAL_SELECT      , Operand.CTL);
        addInstruction("wherenext"    , Opcode.SPATIAL_SELECT      , Operand.CTL);
        addInstruction("whereprev"    , Opcode.SPATIAL_SELECT      , Operand.CTL);
        addInstruction("wherenzero"   , Opcode.SPATIAL_SELECT      , Operand.CTL);
        addInstruction("wherencarry"  , Opcode.SPATIAL_SELECT      , Operand.CTL);
        addInstruction("wherenfirst"  , Opcode.SPATIAL_SELECT      , Operand.CTL);
        addInstruction("wherennext"   , Opcode.SPATIAL_SELECT      , Operand.CTL);
        addInstruction("wherenprev"   , Opcode.SPATIAL_SELECT      , Operand.CTL);
        addInstruction("elsewhere"    , Opcode.SPATIAL_SELECT      , Operand.CTL);
        addInstruction("endwhere"     , Opcode.SPATIAL_SELECT      , Operand.CTL);
        addInstruction("activate"     , Opcode.SPATIAL_SELECT      , Operand.CTL);
/*        addInstruction("rednop"       , Opcode.SETRED              , Operand.CTL);
        addInstruction("redadd"       , Opcode.SETRED              , Operand.CTL);
        addInstruction("redmin"       , Opcode.SETRED              , Operand.CTL);
        addInstruction("redmax"       , Opcode.SETRED              , Operand.CTL);
*/

//##########################################
        addInstruction("nop"          , Opcode.ADD        , Operand.VAL);

//##################################################################################################
        addInstruction("jmp"            , Opcode.JMP                , Operand.CTL       , Value.INSTR_JMP_FUNCTION_JMP          );
        addInstruction("brz"            , Opcode.JMP                , Operand.CTL       , Value.INSTR_JMP_FUNCTION_BRZ          );
        addInstruction("brnz"           , Opcode.JMP                , Operand.CTL       , Value.INSTR_JMP_FUNCTION_BRNZ         );
        addInstruction("brsgn"          , Opcode.JMP                , Operand.CTL       , Value.INSTR_JMP_FUNCTION_BRSGN        );
        addInstruction("brnsgn"         , Opcode.JMP                , Operand.CTL       , Value.INSTR_JMP_FUNCTION_BRNSGN       );
        addInstruction("brzdec"         , Opcode.JMP                , Operand.CTL       , Value.INSTR_JMP_FUNCTION_BRZDEC       );
        addInstruction("brnzdec"        , Opcode.JMP                , Operand.CTL       , Value.INSTR_JMP_FUNCTION_BRNZDEC      );
        addInstruction("brbool"         , Opcode.JMP                , Operand.CTL       , Value.INSTR_JMP_FUNCTION_BRBOOL       );
        addInstruction("brnbool"        , Opcode.JMP                , Operand.CTL       , Value.INSTR_JMP_FUNCTION_BRNBOOL      );
        addInstruction("brcr"           , Opcode.JMP                , Operand.CTL       , Value.INSTR_JMP_FUNCTION_BRCR         );
        addInstruction("brncr"          , Opcode.JMP                , Operand.CTL       , Value.INSTR_JMP_FUNCTION_BRNCR        );
        addInstruction("brcr_delayed"   , Opcode.JMP                , Operand.CTL       , Value.INSTR_JMP_FUNCTION_BRCR_delayed );
        addInstruction("brncr_delayed"  , Opcode.JMP                , Operand.CTL       , Value.INSTR_JMP_FUNCTION_BRNCR_delayed);
        addInstruction("brvalz"         , Opcode.JMP                , Operand.CTL       , Value.INSTR_JMP_FUNCTION_BRCMPVal     );
        addInstruction("brvalnz"        , Opcode.JMP                , Operand.CTL       , Value.INSTR_JMP_FUNCTION_BRCMPNVal    );
        addInstruction("brvalsgn"       , Opcode.JMP                , Operand.CTL       , Value.INSTR_JMP_FUNCTION_BRCMPValDEC  );
        addInstruction("brvalnsgn"      , Opcode.JMP                , Operand.CTL       , Value.INSTR_JMP_FUNCTION_BRCMPNValDEC );
        addInstruction("brvalzdec"      , Opcode.JMP                , Operand.CTL       , Value.INSTR_JMP_FUNCTION_BRValZ       );
        addInstruction("brvalnzdec"     , Opcode.JMP                , Operand.CTL       , Value.INSTR_JMP_FUNCTION_BRValNZ      );
        addInstruction("brcmpval"       , Opcode.JMP                , Operand.CTL       , Value.INSTR_JMP_FUNCTION_BRValSGN     );
        addInstruction("brcmpnval"      , Opcode.JMP                , Operand.CTL       , Value.INSTR_JMP_FUNCTION_BRValNSGN    );
        addInstruction("brcmpvaldec"    , Opcode.JMP                , Operand.CTL       , Value.INSTR_JMP_FUNCTION_BRValZDEC    );
        addInstruction("brcmpnvaldec"   , Opcode.JMP                , Operand.CTL       , Value.INSTR_JMP_FUNCTION_BRValNZDEC   );
        addInstruction("halt"           , Opcode.JMP                , Operand.CTL       , Value.INSTR_JMP_FUNCTION_JMP);



        addInstruction("start_w_halt"   , Opcode.MISC_TESTING       , Operand.CTL       , Value.INSTR_MISC_TESTING_SEL_CC_START_W_HALT);
        addInstruction("start_wo_halt"  , Opcode.MISC_TESTING       , Operand.CTL       , Value.INSTR_MISC_TESTING_SEL_CC_START_WO_HALT);
        addInstruction("stop"           , Opcode.MISC_TESTING       , Operand.CTL       , Value.INSTR_MISC_TESTING_SEL_CC_STOP);
        addInstruction("reset"          , Opcode.MISC_TESTING       , Operand.CTL       , Value.INSTR_MISC_TESTING_SEL_CC_RESET);

//##################################################################################################
        addInstruction("prun"         , Opcode.PRUN         , Operand.CTL);
//        addInstruction("pcheck"       , Opcode.PCHECK       , Operand.CTL);
        addInstruction("param"        , Opcode.PARAM        , Operand.CTL);
//        addInstruction("getv"         , Opcode.POPLOOP      , Operand.CTL);
//        addInstruction("sendv"        , Opcode.PUSHLOOP     , Operand.CTL);
        addInstruction("setint"       , Opcode.SENDINT      , Operand.CTL);

// late additions:
//'setval'|'waitmatw'|'resready'|'brcmpnvaldec'|'setdec'                
        addInstruction("setval"       , Opcode.MISC_STORE_LOAD      , Operand.CTL);
        addInstruction("setdec"       , Opcode.MISC_STORE_LOAD      , Operand.CTL);
        addInstruction("waitmatw"     , Opcode.WAITMATW             , Operand.CTL);
        addInstruction("resready"     , Opcode.RESREADY             , Operand.CTL);
        addInstruction("brcmpnvaldec" , Opcode.JMP                  , Operand.CTL);
    }

//-------------------------------------------------------------------------------------
    private void addOperand(Operand _operand) {
        operands.put(_operand.getData(), _operand);
    }

//-------------------------------------------------------------------------------------
    private void addInstruction(String _instruction, Opcode _opcode, Operand _operand) {
        addInstruction(_instruction, _opcode, _operand, new Value());
    }

//-------------------------------------------------------------------------------------
    private void addInstruction(String _instructionName, Opcode _opcode, Operand _operand, Value _value) {
/*        Field[] _fields;
        if(_value == null){
            _fields = new Field[]{_opcode, _operand};
        } else {
            _fields = new Field[]{_opcode, _operand, _value};            
        }*/
        Instruction _instruction = new Instruction(_instructionName, _opcode, _operand, _value);
        instructions.put(_instructionName, _instruction);
        int _code = _opcode.getData();
        opcodes.putIfAbsent(_code, _opcode);
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
