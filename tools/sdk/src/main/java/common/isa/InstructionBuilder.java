//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.isa;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import com.opencsv.*;
import com.opencsv.exceptions.*;

import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.context.*;

//-------------------------------------------------------------------------------------
public class InstructionBuilder {
    private Context context;
    private Logger log;

//-------------------------------------------------------------------------------------
    public InstructionBuilder(Context _context) {
        context = _context;
        log = _context.getLog();
    }

//-------------------------------------------------------------------------------------
    public Instruction build(String _opcode, String _value, Primitive _primitive) {
        switch(_opcode.toUpperCase()){
            case "VADD"         : return new Instruction(_opcode, Operation.ADD,         Operand.VAL, _value, _primitive);
            case "ADD"          : return new Instruction(_opcode, Operation.ADD,         Operand.MAB, _value, _primitive);
            case "RADD"         : return new Instruction(_opcode, Operation.ADD,         Operand.MRL, _value, _primitive);
            case "RIADD"        : return new Instruction(_opcode, Operation.ADD,         Operand.MRI, _value, _primitive);
            case "CADD"         : return new Instruction(_opcode, Operation.ADD,         Operand.COP, "0", _primitive);
            case "CAADD"        : return new Instruction(_opcode, Operation.ADD,         Operand.MAC, "0", _primitive);
            case "CRADD"        : return new Instruction(_opcode, Operation.ADD,         Operand.MRC, "0", _primitive);

            case "VADDC"        : return new Instruction(_opcode, Operation.ADDC,        Operand.VAL, _value, _primitive);
            case "ADDC"         : return new Instruction(_opcode, Operation.ADDC,        Operand.MAB, _value, _primitive);
            case "RADDC"        : return new Instruction(_opcode, Operation.ADDC,        Operand.MRL, _value, _primitive);
            case "RIADDC"       : return new Instruction(_opcode, Operation.ADDC,        Operand.MRI, _value, _primitive);
            case "CADDC"        : return new Instruction(_opcode, Operation.ADDC,        Operand.COP, "0", _primitive);
            case "CAADDC"       : return new Instruction(_opcode, Operation.ADDC,        Operand.MAC, "0", _primitive);
            case "CRADDC"       : return new Instruction(_opcode, Operation.ADDC,        Operand.MRC, "0", _primitive);
            
            case "VSUB"         : return new Instruction(_opcode, Operation.SUB,         Operand.VAL, _value, _primitive);
            case "SUB"          : return new Instruction(_opcode, Operation.SUB,         Operand.MAB, _value, _primitive);
            case "RSUB"         : return new Instruction(_opcode, Operation.SUB,         Operand.MRL, _value, _primitive);
            case "RISUB"        : return new Instruction(_opcode, Operation.SUB,         Operand.MRI, _value, _primitive);
            case "CSUB"         : return new Instruction(_opcode, Operation.SUB,         Operand.COP, "0", _primitive);
            case "CASUB"        : return new Instruction(_opcode, Operation.SUB,         Operand.MAC, "0", _primitive);
            case "CRSUB"        : return new Instruction(_opcode, Operation.SUB,         Operand.MRC, "0", _primitive);
            
            case "VRVSUB"       : return new Instruction(_opcode, Operation.RSUB,        Operand.VAL, _value, _primitive);
            case "RVSUB"        : return new Instruction(_opcode, Operation.RSUB,        Operand.MAB, _value, _primitive);
            case "RRVSUB"       : return new Instruction(_opcode, Operation.RSUB,        Operand.MRL, _value, _primitive);
            case "RIRVSUB"      : return new Instruction(_opcode, Operation.RSUB,        Operand.MRI, _value, _primitive);
            case "CRVSUB"       : return new Instruction(_opcode, Operation.RSUB,        Operand.COP, "0", _primitive);
            case "CARVSUB"      : return new Instruction(_opcode, Operation.RSUB,        Operand.MAC, "0", _primitive);
            case "CRRVSUB"      : return new Instruction(_opcode, Operation.RSUB,        Operand.MRC, "0", _primitive);
            
            case "VSUBC"        : return new Instruction(_opcode, Operation.SUBC,        Operand.VAL, _value, _primitive);
            case "SUBC"         : return new Instruction(_opcode, Operation.SUBC,        Operand.MAB, _value, _primitive);
            case "RSUBC"        : return new Instruction(_opcode, Operation.SUBC,        Operand.MRL, _value, _primitive);
            case "RISUBC"       : return new Instruction(_opcode, Operation.SUBC,        Operand.MRI, _value, _primitive);
            case "CSUBC"        : return new Instruction(_opcode, Operation.SUBC,        Operand.COP, "0", _primitive);
            case "CASUBC"       : return new Instruction(_opcode, Operation.SUBC,        Operand.MAC, "0", _primitive);
            case "CRSUBC"       : return new Instruction(_opcode, Operation.SUBC,        Operand.MRC, "0", _primitive);
            
            case "VRVSUBC"      : return new Instruction(_opcode, Operation.RSUBC,       Operand.VAL, _value, _primitive);
            case "RVSUBC"       : return new Instruction(_opcode, Operation.RSUBC,       Operand.MAB, _value, _primitive);
            case "RRVSUBC"      : return new Instruction(_opcode, Operation.RSUBC,       Operand.MRL, _value, _primitive);
            case "RIRVSUBC"     : return new Instruction(_opcode, Operation.RSUBC,       Operand.MRI, _value, _primitive);
            case "CRVSUBC"      : return new Instruction(_opcode, Operation.RSUBC,       Operand.COP, "0", _primitive);
            case "CARVSUBC"     : return new Instruction(_opcode, Operation.RSUBC,       Operand.MAC, "0", _primitive);
            case "CRRVSUBC"     : return new Instruction(_opcode, Operation.RSUBC,       Operand.MRC, "0", _primitive);
            
            case "VMULT"        : return new Instruction(_opcode, Operation.MULT,        Operand.VAL, _value, _primitive);
            case "MULT"         : return new Instruction(_opcode, Operation.MULT,        Operand.MAB, _value, _primitive);
            case "RMULT"        : return new Instruction(_opcode, Operation.MULT,        Operand.MRL, _value, _primitive);
            case "RIMULT"       : return new Instruction(_opcode, Operation.MULT,        Operand.MRI, _value, _primitive);
            case "CMULT"        : return new Instruction(_opcode, Operation.MULT,        Operand.COP, "0", _primitive);
            case "CAMULT"       : return new Instruction(_opcode, Operation.MULT,        Operand.MAC, "0", _primitive);
            case "CRMULT"       : return new Instruction(_opcode, Operation.MULT,        Operand.MRC, "0", _primitive);
            
            case "VAND"         : return new Instruction(_opcode, Operation.BWAND,       Operand.VAL, _value, _primitive);
            case "AND"          : return new Instruction(_opcode, Operation.BWAND,       Operand.MAB, _value, _primitive);
            case "RAND"         : return new Instruction(_opcode, Operation.BWAND,       Operand.MRL, _value, _primitive);
            case "RIAND"        : return new Instruction(_opcode, Operation.BWAND,       Operand.MRI, _value, _primitive);
            case "CAND"         : return new Instruction(_opcode, Operation.BWAND,       Operand.COP, "0", _primitive);
            case "CAAND"        : return new Instruction(_opcode, Operation.BWAND,       Operand.MAC, "0", _primitive);
            case "CRAND"        : return new Instruction(_opcode, Operation.BWAND,       Operand.MRC, "0", _primitive);
            
            case "VOR"          : return new Instruction(_opcode, Operation.BWOR,        Operand.VAL, _value, _primitive);
            case "OR"           : return new Instruction(_opcode, Operation.BWOR,        Operand.MAB, _value, _primitive);
            case "ROR"          : return new Instruction(_opcode, Operation.BWOR,        Operand.MRL, _value, _primitive);
            case "RIOR"         : return new Instruction(_opcode, Operation.BWOR,        Operand.MRI, _value, _primitive);
            case "COR"          : return new Instruction(_opcode, Operation.BWOR,        Operand.COP, "0", _primitive);
            case "CAOR"         : return new Instruction(_opcode, Operation.BWOR,        Operand.MAC, "0", _primitive);
            case "CROR"         : return new Instruction(_opcode, Operation.BWOR,        Operand.MRC, "0", _primitive);
            
            case "VXOR"         : return new Instruction(_opcode, Operation.BWXOR,       Operand.VAL, _value, _primitive);
            case "XOR"          : return new Instruction(_opcode, Operation.BWXOR,       Operand.MAB, _value, _primitive);
            case "RXOR"         : return new Instruction(_opcode, Operation.BWXOR,       Operand.MRL, _value, _primitive);
            case "RIXOR"        : return new Instruction(_opcode, Operation.BWXOR,       Operand.MRI, _value, _primitive);
            case "CXOR"         : return new Instruction(_opcode, Operation.BWXOR,       Operand.COP, "0", _primitive);
            case "CAXOR"        : return new Instruction(_opcode, Operation.BWXOR,       Operand.MAC, "0", _primitive);
            case "CRXOR"        : return new Instruction(_opcode, Operation.BWXOR,       Operand.MRC, "0", _primitive);
            
            case "VLOAD"        : return new Instruction(_opcode, Operation.LOAD,        Operand.VAL, _value, _primitive);
            case "LOAD"         : return new Instruction(_opcode, Operation.LOAD,        Operand.MAB, _value, _primitive);
            case "RLOAD"        : return new Instruction(_opcode, Operation.LOAD,        Operand.MRL, _value, _primitive);
            case "RILOAD"       : return new Instruction(_opcode, Operation.LOAD,        Operand.MRI, _value, _primitive);
            case "CLOAD"        : return new Instruction(_opcode, Operation.LOAD,        Operand.COP, "0", _primitive);
            case "CALOAD"       : return new Instruction(_opcode, Operation.LOAD,        Operand.MAC, "0", _primitive);
            case "CRLOAD"       : return new Instruction(_opcode, Operation.LOAD,        Operand.MRC, "0", _primitive);
            case "IXLOAD"       : return new Instruction(_opcode, Operation.IXLOAD,      Operand.CTL, "0", _primitive);
            case "SRLOAD"       : return new Instruction(_opcode, Operation.SRLOAD,      Operand.CTL, "0", _primitive);
            case "IOLOAD"       : return new Instruction(_opcode, Operation.IOLOAD,      Operand.CTL, "0", _primitive);
            case "INSVAL"       : return new Instruction(_opcode, Operation.INSVAL,      Operand.CTL, _value, _primitive);
        
            case "ADDRLD"       : return new Instruction(_opcode, Operation.ADDRLD,      Operand.CTL, "0", _primitive);
            case "STORE"        : return new Instruction(_opcode, Operation.STORE,       Operand.MAB, _value, _primitive);
            case "RSTORE"       : return new Instruction(_opcode, Operation.STORE,       Operand.MRL, _value, _primitive);
            case "RISTORE"      : return new Instruction(_opcode, Operation.STORE,       Operand.MRI, _value, _primitive);
            case "CSTORE"       : return new Instruction(_opcode, Operation.STORE,       Operand.MAC, "0", _primitive);
            case "SRSTORE"      : return new Instruction(_opcode, Operation.SRSTORE,     Operand.CTL, "0", _primitive);
            case "IOSTORE"      : return new Instruction(_opcode, Operation.IOSTORE,     Operand.CTL, "0", _primitive);
            case "INSERTIO"     : return new Instruction(_opcode, Operation.INSERTIO,    Operand.CTL, "0", _primitive);

            case "COMPARE"      : return new Instruction(_opcode, Operation.COMPARE,     Operand.MAB, _value, _primitive);
            case "RCOMPARE"     : return new Instruction(_opcode, Operation.COMPARE,     Operand.MRL, _value, _primitive);
            case "RICOMPARE"    : return new Instruction(_opcode, Operation.COMPARE,     Operand.MRI, _value, _primitive);
            case "VCOMPARE"     : return new Instruction(_opcode, Operation.COMPARE,     Operand.VAL, _value, _primitive);
            case "CCOMPARE"     : return new Instruction(_opcode, Operation.COMPARE,     Operand.COP, "0", _primitive);
            case "CACOMPARE"    : return new Instruction(_opcode, Operation.COMPARE,     Operand.MAC, "0", _primitive);
            case "CRCOMPARE"    : return new Instruction(_opcode, Operation.COMPARE,     Operand.MRC, "0", _primitive);
    
            case "SHRIGHTC"     : return new Instruction(_opcode, Operation.SHRIGHTC,    Operand.CTL, "0", _primitive);
            case "SHRIGHT"      : return new Instruction(_opcode, Operation.SHRIGHT,     Operand.CTL, "0", _primitive);
            case "SHARIGHT"     : return new Instruction(_opcode, Operation.SHARIGHT,    Operand.CTL, "0", _primitive);
            case "ROTRIGHT"     : return new Instruction(_opcode, Operation.ROTATE,      Operand.CTL, "1", _primitive);
            case "SHR"          : return new Instruction(_opcode, Operation.ROTATE,      Operand.CTL, "2", _primitive);
            case "SHL"          : return new Instruction(_opcode, Operation.ROTATE,      Operand.CTL, "3", _primitive);

//            ##### ARRAY CONTROL INSTRUCTIONS #########_opcode, 
            case "WHEREZERO"    : return new Instruction(_opcode, Operation.WHERE    ,   Operand.CTL, "1", _primitive);
            case "WHERECARRY"   : return new Instruction(_opcode, Operation.WHERE    ,   Operand.CTL, "3", _primitive);
            case "WHEREFIRST"   : return new Instruction(_opcode, Operation.WHERE    ,   Operand.CTL, "5", _primitive);
            case "WHERENEXT"    : return new Instruction(_opcode, Operation.WHERE    ,   Operand.CTL, "7", _primitive);
            case "WHEREPREV"    : return new Instruction(_opcode, Operation.WHERE    ,   Operand.CTL, "9", _primitive);
            case "WHERENZERO"   : return new Instruction(_opcode, Operation.WHERE    ,   Operand.CTL, "0", _primitive);
            case "WHERENCARRY"  : return new Instruction(_opcode, Operation.WHERE    ,   Operand.CTL, "2", _primitive);
            case "WHERENFIRST"  : return new Instruction(_opcode, Operation.WHERE    ,   Operand.CTL, "4", _primitive);
            case "WHERENNEXT"   : return new Instruction(_opcode, Operation.WHERE    ,   Operand.CTL, "6", _primitive);
            case "WHERENPREV"   : return new Instruction(_opcode, Operation.WHERE    ,   Operand.CTL, "8", _primitive);
            case "ELSEWHERE"    : return new Instruction(_opcode, Operation.ELSEW    ,   Operand.CTL, "0", _primitive);
            case "ENDWHERE"     : return new Instruction(_opcode, Operation.ENDWHERE ,   Operand.CTL, "0", _primitive);
            case "ACTIVATE"     : return new Instruction(_opcode, Operation.ACTIVATE ,   Operand.CTL, "0", _primitive);
            case "REDNOP"       : return new Instruction(_opcode, Operation.SETRED   ,   Operand.CTL, "0", _primitive);
            case "REDADD"       : return new Instruction(_opcode, Operation.SETRED   ,   Operand.CTL, "1", _primitive);
            case "REDMIN"       : return new Instruction(_opcode, Operation.SETRED   ,   Operand.CTL, "2", _primitive);
            case "REDMAX"       : return new Instruction(_opcode, Operation.SETRED   ,   Operand.CTL, "3", _primitive);

//##########################################_opcode, 
            case "NOP"          : return new Instruction(_opcode, Operation.ADD,         Operand.VAL, "0", _primitive);

//#######################################################_opcode, ###########################################
            case "JMP"          : return new Instruction(_opcode, Operation.JMP,         Operand.CTL, _value, _primitive);
            case "BRZ"          : return new Instruction(_opcode, Operation.BRZ,         Operand.CTL, _value, _primitive);
            case "BRNZ"         : return new Instruction(_opcode, Operation.BRNZ,        Operand.CTL, _value, _primitive);
            case "BRSGN"        : return new Instruction(_opcode, Operation.BRSGN,       Operand.CTL, _value, _primitive);
            case "BRNSGN"       : return new Instruction(_opcode, Operation.BRNSGN,      Operand.CTL, _value, _primitive);
            case "BRZDEC"       : return new Instruction(_opcode, Operation.BRZDEC,      Operand.CTL, _value, _primitive);
            case "BRNZDEC"      : return new Instruction(_opcode, Operation.BRNZDEC,     Operand.CTL, _value, _primitive);
            case "BRBOOL"       : return new Instruction(_opcode, Operation.BRBOOL,      Operand.CTL, _value, _primitive);
            case "BRNBOOL"      : return new Instruction(_opcode, Operation.BRNBOOL,     Operand.CTL, _value, _primitive);
            case "BRCR"         : return new Instruction(_opcode, Operation.BRCR,        Operand.CTL, _value, _primitive);
            case "BRNCR"        : return new Instruction(_opcode, Operation.BRNCR,       Operand.CTL, _value, _primitive);
            case "HALT"         : return new Instruction(_opcode, Operation.JMP,         Operand.CTL, "0", _primitive);
            case "START"        : return new Instruction(_opcode, Operation.START,       Operand.CTL, "0", _primitive);
            case "STOP"         : return new Instruction(_opcode, Operation.STOP,        Operand.CTL, "0", _primitive);

//#######################################################_opcode, ###########################################
            case "PRUN"         : return new Instruction(_opcode, Operation.PRUN,        Operand.CTL, _value, _primitive);
            //case "PLOAD"        : return new Instruction(_opcode, Operation.PLOAD,       Operand.CTL, "0", _primitive);
            case "PCHECK"        : return new Instruction(_opcode, Operation.PCHECK,     Operand.CTL, "0", _primitive);
            case "PARAM"        : return new Instruction(_opcode, Operation.PARAM,       Operand.CTL, "0", _primitive);
            case "GETV"         : return new Instruction(_opcode, Operation.POPLOOP,     Operand.CTL, "0", _primitive);
            case "SENDV"        : return new Instruction(_opcode, Operation.PUSHLOOP,    Operand.CTL, "0", _primitive);
            case "SETINT"       : return new Instruction(_opcode, Operation.SENDINT,     Operand.CTL, "0", _primitive);



// late additions:
//'setval'|'waitmatw'|'resready'|'brcmpnvaldec'|'setdec'                
            case "SETVAL"       : return new Instruction(_opcode, Operation.SENDINT,     Operand.CTL, "0", _primitive);
            case "SETDEC"       : return new Instruction(_opcode, Operation.SENDINT,     Operand.CTL, "0", _primitive);
            case "WAITMATW"     : return new Instruction(_opcode, Operation.SENDINT,     Operand.CTL, "0", _primitive);
            case "RESREADY"     : return new Instruction(_opcode, Operation.SENDINT,     Operand.CTL, "0", _primitive);
            case "BRCMPNVALDEC" : return new Instruction(_opcode, Operation.SENDINT,     Operand.CTL, "0", _primitive);
            default             : return null;
        }
    }   

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
