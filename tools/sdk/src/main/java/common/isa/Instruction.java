//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.isa;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;

//-------------------------------------------------------------------------------------

public class Instruction {
    private String opcode;
    private Operation operation;
    private Operand operand;
    private Value value;

    private Program program;

//-------------------------------------------------------------------------------------
    public Instruction(String _opcode, Operation _operation, Operand _operand, String _value, Program _program) {
        opcode = _opcode;
        operation = _operation;
        operand = _operand;
        program = _program;
        int _valueLength = _program.getArhCode() - 8;
        value = Value.getValue(_value, _valueLength);
    }   

//-------------------------------------------------------------------------------------
    public static Instruction getInstruction(String _opcode, String _value, Program _program) {
        switch(_opcode.toUpperCase()){
            case "VADD"         : return new Instruction(_opcode, Operation.ADD,         Operand.VAL, _value, _program);
            case "ADD"          : return new Instruction(_opcode, Operation.ADD,         Operand.MAB, _value, _program);
            case "RADD"         : return new Instruction(_opcode, Operation.ADD,         Operand.MRL, _value, _program);
            case "RIADD"        : return new Instruction(_opcode, Operation.ADD,         Operand.MRI, _value, _program);
            case "CADD"         : return new Instruction(_opcode, Operation.ADD,         Operand.COP, "0", _program);
            case "CAADD"        : return new Instruction(_opcode, Operation.ADD,         Operand.MAC, "0", _program);
            case "CRADD"        : return new Instruction(_opcode, Operation.ADD,         Operand.MRC, "0", _program);

            case "VADDC"        : return new Instruction(_opcode, Operation.ADDC,        Operand.VAL, _value, _program);
            case "ADDC"         : return new Instruction(_opcode, Operation.ADDC,        Operand.MAB, _value, _program);
            case "RADDC"        : return new Instruction(_opcode, Operation.ADDC,        Operand.MRL, _value, _program);
            case "RIADDC"       : return new Instruction(_opcode, Operation.ADDC,        Operand.MRI, _value, _program);
            case "CADDC"        : return new Instruction(_opcode, Operation.ADDC,        Operand.COP, "0", _program);
            case "CAADDC"       : return new Instruction(_opcode, Operation.ADDC,        Operand.MAC, "0", _program);
            case "CRADDC"       : return new Instruction(_opcode, Operation.ADDC,        Operand.MRC, "0", _program);
            
            case "VSUB"         : return new Instruction(_opcode, Operation.SUB,         Operand.VAL, _value, _program);
            case "SUB"          : return new Instruction(_opcode, Operation.SUB,         Operand.MAB, _value, _program);
            case "RSUB"         : return new Instruction(_opcode, Operation.SUB,         Operand.MRL, _value, _program);
            case "RISUB"        : return new Instruction(_opcode, Operation.SUB,         Operand.MRI, _value, _program);
            case "CSUB"         : return new Instruction(_opcode, Operation.SUB,         Operand.COP, "0", _program);
            case "CASUB"        : return new Instruction(_opcode, Operation.SUB,         Operand.MAC, "0", _program);
            case "CRSUB"        : return new Instruction(_opcode, Operation.SUB,         Operand.MRC, "0", _program);
            
            case "VRVSUB"       : return new Instruction(_opcode, Operation.RSUB,        Operand.VAL, _value, _program);
            case "RVSUB"        : return new Instruction(_opcode, Operation.RSUB,        Operand.MAB, _value, _program);
            case "RRVSUB"       : return new Instruction(_opcode, Operation.RSUB,        Operand.MRL, _value, _program);
            case "RIRVSUB"      : return new Instruction(_opcode, Operation.RSUB,        Operand.MRI, _value, _program);
            case "CRVSUB"       : return new Instruction(_opcode, Operation.RSUB,        Operand.COP, "0", _program);
            case "CARVSUB"      : return new Instruction(_opcode, Operation.RSUB,        Operand.MAC, "0", _program);
            case "CRRVSUB"      : return new Instruction(_opcode, Operation.RSUB,        Operand.MRC, "0", _program);
            
            case "VSUBC"        : return new Instruction(_opcode, Operation.SUBC,        Operand.VAL, _value, _program);
            case "SUBC"         : return new Instruction(_opcode, Operation.SUBC,        Operand.MAB, _value, _program);
            case "RSUBC"        : return new Instruction(_opcode, Operation.SUBC,        Operand.MRL, _value, _program);
            case "RISUBC"       : return new Instruction(_opcode, Operation.SUBC,        Operand.MRI, _value, _program);
            case "CSUBC"        : return new Instruction(_opcode, Operation.SUBC,        Operand.COP, "0", _program);
            case "CASUBC"       : return new Instruction(_opcode, Operation.SUBC,        Operand.MAC, "0", _program);
            case "CRSUBC"       : return new Instruction(_opcode, Operation.SUBC,        Operand.MRC, "0", _program);
            
            case "VRVSUBC"      : return new Instruction(_opcode, Operation.RSUBC,       Operand.VAL, _value, _program);
            case "RVSUBC"       : return new Instruction(_opcode, Operation.RSUBC,       Operand.MAB, _value, _program);
            case "RRVSUBC"      : return new Instruction(_opcode, Operation.RSUBC,       Operand.MRL, _value, _program);
            case "RIRVSUBC"     : return new Instruction(_opcode, Operation.RSUBC,       Operand.MRI, _value, _program);
            case "CRVSUBC"      : return new Instruction(_opcode, Operation.RSUBC,       Operand.COP, "0", _program);
            case "CARVSUBC"     : return new Instruction(_opcode, Operation.RSUBC,       Operand.MAC, "0", _program);
            case "CRRVSUBC"     : return new Instruction(_opcode, Operation.RSUBC,       Operand.MRC, "0", _program);
            
            case "VMULT"        : return new Instruction(_opcode, Operation.MULT,        Operand.VAL, _value, _program);
            case "MULT"         : return new Instruction(_opcode, Operation.MULT,        Operand.MAB, _value, _program);
            case "RMULT"        : return new Instruction(_opcode, Operation.MULT,        Operand.MRL, _value, _program);
            case "RIMULT"       : return new Instruction(_opcode, Operation.MULT,        Operand.MRI, _value, _program);
            case "CMULT"        : return new Instruction(_opcode, Operation.MULT,        Operand.COP, "0", _program);
            case "CAMULT"       : return new Instruction(_opcode, Operation.MULT,        Operand.MAC, "0", _program);
            case "CRMULT"       : return new Instruction(_opcode, Operation.MULT,        Operand.MRC, "0", _program);
            
            case "VAND"         : return new Instruction(_opcode, Operation.BWAND,       Operand.VAL, _value, _program);
            case "AND"          : return new Instruction(_opcode, Operation.BWAND,       Operand.MAB, _value, _program);
            case "RAND"         : return new Instruction(_opcode, Operation.BWAND,       Operand.MRL, _value, _program);
            case "RIAND"        : return new Instruction(_opcode, Operation.BWAND,       Operand.MRI, _value, _program);
            case "CAND"         : return new Instruction(_opcode, Operation.BWAND,       Operand.COP, "0", _program);
            case "CAAND"        : return new Instruction(_opcode, Operation.BWAND,       Operand.MAC, "0", _program);
            case "CRAND"        : return new Instruction(_opcode, Operation.BWAND,       Operand.MRC, "0", _program);
            
            case "VOR"          : return new Instruction(_opcode, Operation.BWOR,        Operand.VAL, _value, _program);
            case "OR"           : return new Instruction(_opcode, Operation.BWOR,        Operand.MAB, _value, _program);
            case "ROR"          : return new Instruction(_opcode, Operation.BWOR,        Operand.MRL, _value, _program);
            case "RIOR"         : return new Instruction(_opcode, Operation.BWOR,        Operand.MRI, _value, _program);
            case "COR"          : return new Instruction(_opcode, Operation.BWOR,        Operand.COP, "0", _program);
            case "CAOR"         : return new Instruction(_opcode, Operation.BWOR,        Operand.MAC, "0", _program);
            case "CROR"         : return new Instruction(_opcode, Operation.BWOR,        Operand.MRC, "0", _program);
            
            case "VXOR"         : return new Instruction(_opcode, Operation.BWXOR,       Operand.VAL, _value, _program);
            case "XOR"          : return new Instruction(_opcode, Operation.BWXOR,       Operand.MAB, _value, _program);
            case "RXOR"         : return new Instruction(_opcode, Operation.BWXOR,       Operand.MRL, _value, _program);
            case "RIXOR"        : return new Instruction(_opcode, Operation.BWXOR,       Operand.MRI, _value, _program);
            case "CXOR"         : return new Instruction(_opcode, Operation.BWXOR,       Operand.COP, "0", _program);
            case "CAXOR"        : return new Instruction(_opcode, Operation.BWXOR,       Operand.MAC, "0", _program);
            case "CRXOR"        : return new Instruction(_opcode, Operation.BWXOR,       Operand.MRC, "0", _program);
            
            case "VLOAD"        : return new Instruction(_opcode, Operation.LOAD,        Operand.VAL, _value, _program);
            case "LOAD"         : return new Instruction(_opcode, Operation.LOAD,        Operand.MAB, _value, _program);
            case "RLOAD"        : return new Instruction(_opcode, Operation.LOAD,        Operand.MRL, _value, _program);
            case "RILOAD"       : return new Instruction(_opcode, Operation.LOAD,        Operand.MRI, _value, _program);
            case "CLOAD"        : return new Instruction(_opcode, Operation.LOAD,        Operand.COP, "0", _program);
            case "CALOAD"       : return new Instruction(_opcode, Operation.LOAD,        Operand.MAC, "0", _program);
            case "CRLOAD"       : return new Instruction(_opcode, Operation.LOAD,        Operand.MRC, "0", _program);
            case "IXLOAD"       : return new Instruction(_opcode, Operation.IXLOAD,      Operand.CTL, "0", _program);
            case "SRLOAD"       : return new Instruction(_opcode, Operation.SRLOAD,      Operand.CTL, "0", _program);
            case "IOLOAD"       : return new Instruction(_opcode, Operation.IOLOAD,      Operand.CTL, "0", _program);
            case "INSVAL"       : return new Instruction(_opcode, Operation.INSVAL,      Operand.CTL, _value, _program);
        
            case "ADDRLD"       : return new Instruction(_opcode, Operation.ADDRLD,      Operand.CTL, "0", _program);
            case "STORE"        : return new Instruction(_opcode, Operation.STORE,       Operand.MAB, _value, _program);
            case "RSTORE"       : return new Instruction(_opcode, Operation.STORE,       Operand.MRL, _value, _program);
            case "RISTORE"      : return new Instruction(_opcode, Operation.STORE,       Operand.MRI, _value, _program);
            case "CSTORE"       : return new Instruction(_opcode, Operation.STORE,       Operand.MAC, "0", _program);
            case "SRSTORE"      : return new Instruction(_opcode, Operation.SRSTORE,     Operand.CTL, "0", _program);
            case "IOSTORE"      : return new Instruction(_opcode, Operation.IOSTORE,     Operand.CTL, "0", _program);
            case "INSERTIO"     : return new Instruction(_opcode, Operation.INSERTIO,    Operand.CTL, "0", _program);

            case "COMPARE"      : return new Instruction(_opcode, Operation.COMPARE,     Operand.MAB, _value, _program);
            case "RCOMPARE"     : return new Instruction(_opcode, Operation.COMPARE,     Operand.MRL, _value, _program);
            case "RICOMPARE"    : return new Instruction(_opcode, Operation.COMPARE,     Operand.MRI, _value, _program);
            case "VCOMPARE"     : return new Instruction(_opcode, Operation.COMPARE,     Operand.VAL, _value, _program);
            case "CCOMPARE"     : return new Instruction(_opcode, Operation.COMPARE,     Operand.COP, "0", _program);
            case "CACOMPARE"    : return new Instruction(_opcode, Operation.COMPARE,     Operand.MAC, "0", _program);
            case "CRCOMPARE"    : return new Instruction(_opcode, Operation.COMPARE,     Operand.MRC, "0", _program);
    
            case "SHRIGHTC"     : return new Instruction(_opcode, Operation.SHRIGHTC,    Operand.CTL, "0", _program);
            case "SHRIGHT"      : return new Instruction(_opcode, Operation.SHRIGHT,     Operand.CTL, "0", _program);
            case "SHARIGHT"     : return new Instruction(_opcode, Operation.SHARIGHT,    Operand.CTL, "0", _program);
            case "ROTRIGHT"     : return new Instruction(_opcode, Operation.ROTATE,      Operand.CTL, "1", _program);
            case "SHR"          : return new Instruction(_opcode, Operation.ROTATE,      Operand.CTL, "2", _program);
            case "SHL"          : return new Instruction(_opcode, Operation.ROTATE,      Operand.CTL, "3", _program);

//            ##### ARRAY CONTROL INSTRUCTIONS #########_opcode, 
            case "WHEREZERO"    : return new Instruction(_opcode, Operation.WHERE    ,   Operand.CTL, "1", _program);
            case "WHERECARRY"   : return new Instruction(_opcode, Operation.WHERE    ,   Operand.CTL, "3", _program);
            case "WHEREFIRST"   : return new Instruction(_opcode, Operation.WHERE    ,   Operand.CTL, "5", _program);
            case "WHERENEXT"    : return new Instruction(_opcode, Operation.WHERE    ,   Operand.CTL, "7", _program);
            case "WHEREPREV"    : return new Instruction(_opcode, Operation.WHERE    ,   Operand.CTL, "9", _program);
            case "WHERENZERO"   : return new Instruction(_opcode, Operation.WHERE    ,   Operand.CTL, "0", _program);
            case "WHERENCARRY"  : return new Instruction(_opcode, Operation.WHERE    ,   Operand.CTL, "2", _program);
            case "WHERENFIRST"  : return new Instruction(_opcode, Operation.WHERE    ,   Operand.CTL, "4", _program);
            case "WHERENNEXT"   : return new Instruction(_opcode, Operation.WHERE    ,   Operand.CTL, "6", _program);
            case "WHERENPREV"   : return new Instruction(_opcode, Operation.WHERE    ,   Operand.CTL, "8", _program);
            case "ELSEWHERE"    : return new Instruction(_opcode, Operation.ELSEW    ,   Operand.CTL, "0", _program);
            case "ENDWHERE"     : return new Instruction(_opcode, Operation.ENDWHERE ,   Operand.CTL, "0", _program);
            case "ACTIVATE"     : return new Instruction(_opcode, Operation.ACTIVATE ,   Operand.CTL, "0", _program);
            case "REDNOP"       : return new Instruction(_opcode, Operation.SETRED   ,   Operand.CTL, "0", _program);
            case "REDADD"       : return new Instruction(_opcode, Operation.SETRED   ,   Operand.CTL, "1", _program);
            case "REDMIN"       : return new Instruction(_opcode, Operation.SETRED   ,   Operand.CTL, "2", _program);
            case "REDMAX"       : return new Instruction(_opcode, Operation.SETRED   ,   Operand.CTL, "3", _program);

//##########################################_opcode, 
            case "NOP"          : return new Instruction(_opcode, Operation.ADD,         Operand.VAL, "0", _program);

//#######################################################_opcode, ###########################################
            case "JMP"          : return new Instruction(_opcode, Operation.JMP,         Operand.CTL, _value, _program);
            case "BRZ"          : return new Instruction(_opcode, Operation.BRZ,         Operand.CTL, _value, _program);
            case "BRNZ"         : return new Instruction(_opcode, Operation.BRNZ,        Operand.CTL, _value, _program);
            case "BRSGN"        : return new Instruction(_opcode, Operation.BRSGN,       Operand.CTL, _value, _program);
            case "BRNSGN"       : return new Instruction(_opcode, Operation.BRNSGN,      Operand.CTL, _value, _program);
            case "BRZDEC"       : return new Instruction(_opcode, Operation.BRZDEC,      Operand.CTL, _value, _program);
            case "BRNZDEC"      : return new Instruction(_opcode, Operation.BRNZDEC,     Operand.CTL, _value, _program);
            case "BRBOOL"       : return new Instruction(_opcode, Operation.BRBOOL,      Operand.CTL, _value, _program);
            case "BRNBOOL"      : return new Instruction(_opcode, Operation.BRNBOOL,     Operand.CTL, _value, _program);
            case "BRCR"         : return new Instruction(_opcode, Operation.BRCR,        Operand.CTL, _value, _program);
            case "BRNCR"        : return new Instruction(_opcode, Operation.BRNCR,       Operand.CTL, _value, _program);
            case "HALT"         : return new Instruction(_opcode, Operation.JMP,         Operand.CTL, "0", _program);
            case "START"        : return new Instruction(_opcode, Operation.START,       Operand.CTL, "0", _program);
            case "STOP"         : return new Instruction(_opcode, Operation.STOP,        Operand.CTL, "0", _program);

//#######################################################_opcode, ###########################################
            case "PRUN"         : return new Instruction(_opcode, Operation.PRUN,        Operand.CTL, _value, _program);
            //case "PLOAD"        : return new Instruction(_opcode, Operation.PLOAD,       Operand.CTL, "0", _program);
            case "PCHECK"        : return new Instruction(_opcode, Operation.PCHECK,     Operand.CTL, "0", _program);
            case "PARAM"        : return new Instruction(_opcode, Operation.PARAM,       Operand.CTL, "0", _program);
            case "GETV"         : return new Instruction(_opcode, Operation.POPLOOP,     Operand.CTL, "0", _program);
            case "SENDV"        : return new Instruction(_opcode, Operation.PUSHLOOP,    Operand.CTL, "0", _program);
            case "SETINT"       : return new Instruction(_opcode, Operation.SENDINT,     Operand.CTL, "0", _program);



// late additions:
//'setval'|'waitmatw'|'resready'|'brcmpnvaldec'|'setdec'                
            case "SETVAL"       : return new Instruction(_opcode, Operation.SENDINT,     Operand.CTL, "0", _program);
            case "SETDEC"       : return new Instruction(_opcode, Operation.SENDINT,     Operand.CTL, "0", _program);
            case "WAITMATW"     : return new Instruction(_opcode, Operation.SENDINT,     Operand.CTL, "0", _program);
            case "RESREADY"     : return new Instruction(_opcode, Operation.SENDINT,     Operand.CTL, "0", _program);
            case "BRCMPNVALDEC" : return new Instruction(_opcode, Operation.SENDINT,     Operand.CTL, "0", _program);
            default             : return null;
        }
    }   

//-------------------------------------------------------------------------------------
    public boolean resolve() {
        if(operation.isJump()){
            int _valueLength = program.getArhCode() - 8;
            if(opcode.equals("halt")){
//                value = V_0;
                value = Value.getValue("0", _valueLength);
            } else {
                int _address = program.getByLabel(value.getName());
                value = new Value(_address, _valueLength);                
                if(_address == Integer.MIN_VALUE){
                    return false;
                }
            }
        }
        return true;
    }
    
//-------------------------------------------------------------------------------------
    public int toBin() {
        return Cell.toBin(operation, operand, value);
    }

//-------------------------------------------------------------------------------------
    public String toHex() {
        return Cell.toHex(operation, operand, value);
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
