//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.isa;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.context.arch.*;

//-------------------------------------------------------------------------------------
public class Value extends Field {
    public static final Value INSTR_JMP_FUNCTION_JMP                    = new Value("INSTR_JMP_FUNCTION_JMP          ");
    public static final Value INSTR_JMP_FUNCTION_BRZ                    = new Value("INSTR_JMP_FUNCTION_BRZ          ");
    public static final Value INSTR_JMP_FUNCTION_BRNZ                   = new Value("INSTR_JMP_FUNCTION_BRNZ         ");
    public static final Value INSTR_JMP_FUNCTION_BRSGN                  = new Value("INSTR_JMP_FUNCTION_BRSGN        ");
    public static final Value INSTR_JMP_FUNCTION_BRNSGN                 = new Value("INSTR_JMP_FUNCTION_BRNSGN       ");
    public static final Value INSTR_JMP_FUNCTION_BRZDEC                 = new Value("INSTR_JMP_FUNCTION_BRZDEC       ");
    public static final Value INSTR_JMP_FUNCTION_BRNZDEC                = new Value("INSTR_JMP_FUNCTION_BRNZDEC      ");
    public static final Value INSTR_JMP_FUNCTION_BRBOOL                 = new Value("INSTR_JMP_FUNCTION_BRBOOL       ");
    public static final Value INSTR_JMP_FUNCTION_BRNBOOL                = new Value("INSTR_JMP_FUNCTION_BRNBOOL      ");
    public static final Value INSTR_JMP_FUNCTION_BRCR                   = new Value("INSTR_JMP_FUNCTION_BRCR         ");
    public static final Value INSTR_JMP_FUNCTION_BRNCR                  = new Value("INSTR_JMP_FUNCTION_BRNCR        ");
    public static final Value INSTR_JMP_FUNCTION_BRCR_delayed           = new Value("INSTR_JMP_FUNCTION_BRCR_delayed ");
    public static final Value INSTR_JMP_FUNCTION_BRNCR_delayed          = new Value("INSTR_JMP_FUNCTION_BRNCR_delayed");
    public static final Value INSTR_JMP_FUNCTION_BRCMPVal               = new Value("INSTR_JMP_FUNCTION_BRCMPVal     ");
    public static final Value INSTR_JMP_FUNCTION_BRCMPNVal              = new Value("INSTR_JMP_FUNCTION_BRCMPNVal    ");
    public static final Value INSTR_JMP_FUNCTION_BRCMPValDEC            = new Value("INSTR_JMP_FUNCTION_BRCMPValDEC  ");
    public static final Value INSTR_JMP_FUNCTION_BRCMPNValDEC           = new Value("INSTR_JMP_FUNCTION_BRCMPNValDEC ");
    public static final Value INSTR_JMP_FUNCTION_BRValZ                 = new Value("INSTR_JMP_FUNCTION_BRValZ       ");
    public static final Value INSTR_JMP_FUNCTION_BRValNZ                = new Value("INSTR_JMP_FUNCTION_BRValNZ      ");
    public static final Value INSTR_JMP_FUNCTION_BRValSGN               = new Value("INSTR_JMP_FUNCTION_BRValSGN     ");
    public static final Value INSTR_JMP_FUNCTION_BRValNSGN              = new Value("INSTR_JMP_FUNCTION_BRValNSGN    ");
    public static final Value INSTR_JMP_FUNCTION_BRValZDEC              = new Value("INSTR_JMP_FUNCTION_BRValZDEC    ");
    public static final Value INSTR_JMP_FUNCTION_BRValNZDEC             = new Value("INSTR_JMP_FUNCTION_BRValNZDEC   ");


    public static final Value INSTR_MISC_TESTING_SEL_CC_START_W_HALT    = new Value("INSTR_MISC_TESTING_SEL_CC_START_W_HALT ");
    public static final Value INSTR_MISC_TESTING_SEL_CC_START_WO_HALT   = new Value("INSTR_MISC_TESTING_SEL_CC_START_WO_HALT");
    public static final Value INSTR_MISC_TESTING_SEL_CC_STOP            = new Value("INSTR_MISC_TESTING_SEL_CC_STOP         ");
    public static final Value INSTR_MISC_TESTING_SEL_CC_RESET           = new Value("INSTR_MISC_TESTING_SEL_CC_RESET        ");

//-------------------------------------------------------------------------------------
    public Value() {
        super("", 0);
    }

//-------------------------------------------------------------------------------------
    public Value(String _valueString, int _valueNumber) {
        super(_valueString, _valueNumber);
    }

//-------------------------------------------------------------------------------------
    public Value(int _value) {
        super("const", _value);
    }

//-------------------------------------------------------------------------------------
    public Value(String _name) {
        super(_name.trim(), ArchitectureDefinitions.get(_name.trim()));
    }

//-------------------------------------------------------------------------------------
    public Value clone(){
        return new Value(name, intdata);
    }

//-------------------------------------------------------------------------------------
    public void set(String _valueString, int _valueNumber){
        setName(_valueString);
        setData(_valueNumber, length);
    }

//-------------------------------------------------------------------------------------
    public String getName() {
        return name;
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
