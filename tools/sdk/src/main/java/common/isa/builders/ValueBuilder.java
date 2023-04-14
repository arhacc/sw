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
public class ValueBuilder extends Builder {
    private String arhCode;
    private Map<String, Value> values;

//-------------------------------------------------------------------------------------
    public ValueBuilder(Context _context, String _arhCode) {
        super(_context);
        arhCode = _arhCode;
        values = new HashMap<String, Value>();
        init();        
    }

//-------------------------------------------------------------------------------------
    public Value get(String _value) {
        Value _valueObj = values.get(_value);
/*        Opcode _opcodeField = (Opcode)_fields[0];
        Operand _operandField = (Operand)_fields[1];
        Value _valueField;
        if(_fields.length > 2){
            _valueField = (Value)_fields[2];
        } else {
            _valueField = new Value(_valueString, _valueNumber);
        }*/
        if(_valueObj == null){
            log.error("Error: cannot find value: " + _value);
            System.exit(1);
        }
        return _valueObj;
    }

//-------------------------------------------------------------------------------------
    private void init() {
        addValue("INSTR_JMP_FUNCTION_JMP          ");
        addValue("INSTR_JMP_FUNCTION_BRZ          ");
        addValue("INSTR_JMP_FUNCTION_BRNZ         ");
        addValue("INSTR_JMP_FUNCTION_BRSGN        ");
        addValue("INSTR_JMP_FUNCTION_BRNSGN       ");
        addValue("INSTR_JMP_FUNCTION_BRZDEC       ");
        addValue("INSTR_JMP_FUNCTION_BRNZDEC      ");
        addValue("INSTR_JMP_FUNCTION_BRBOOL       ");
        addValue("INSTR_JMP_FUNCTION_BRNBOOL      ");
        addValue("INSTR_JMP_FUNCTION_BRCR         ");
        addValue("INSTR_JMP_FUNCTION_BRNCR        ");
        addValue("INSTR_JMP_FUNCTION_BRCR_delayed ");
        addValue("INSTR_JMP_FUNCTION_BRNCR_delayed");
        addValue("INSTR_JMP_FUNCTION_BRCMPVal     ");
        addValue("INSTR_JMP_FUNCTION_BRCMPNVal    ");
        addValue("INSTR_JMP_FUNCTION_BRCMPValDEC  ");
        addValue("INSTR_JMP_FUNCTION_BRCMPNValDEC ");
        addValue("INSTR_JMP_FUNCTION_BRValZ       ");
        addValue("INSTR_JMP_FUNCTION_BRValNZ      ");
        addValue("INSTR_JMP_FUNCTION_BRValSGN     ");
        addValue("INSTR_JMP_FUNCTION_BRValNSGN    ");
        addValue("INSTR_JMP_FUNCTION_BRValZDEC    ");
        addValue("INSTR_JMP_FUNCTION_BRValNZDEC   ");
        addValue("INSTR_MISC_TESTING_SEL_CC_START_W_HALT ");
        addValue("INSTR_MISC_TESTING_SEL_CC_START_WO_HALT");
        addValue("INSTR_MISC_TESTING_SEL_CC_STOP         ");
        addValue("INSTR_MISC_TESTING_SEL_CC_RESET        ");
    }

//-------------------------------------------------------------------------------------
    private void addValue(String _valueName) {
        int _valueData = context.getArchitectureImplementations().getArchitecture(arhCode).get(_valueName);
        Value _valueObj = new Value(_valueName, _valueData);
        values.put(_valueName, _valueObj);
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
