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
public class OperandBuilder extends AbstractBuilder {
    private String architectureId;
    private Map<String, Operand> operands;

//-------------------------------------------------------------------------------------
    public OperandBuilder(Context _context, String _architectureId) {
        super(_context);
        architectureId = _architectureId;
        operands = new HashMap<String, Operand>();
        init();        
    }

//-------------------------------------------------------------------------------------
    public Operand get(String _operand) {
        Operand _operandObj = operands.get(_operand);
/*        Opcode _opcodeField = (Opcode)_fields[0];
        Operand _operandField = (Operand)_fields[1];
        Value _valueField;
        if(_fields.length > 2){
            _valueField = (Value)_fields[2];
        } else {
            _valueField = new Value(_valueString, _valueNumber);
        }*/
        if(_operandObj == null){
            log.error("Error: cannot find operand: " + _operand);
            System.exit(1);
        }
        return _operandObj;
    }

//-------------------------------------------------------------------------------------
    private void init() {
        addOperand("val");
        addOperand("mab");
        addOperand("mrl");
        addOperand("mri");
        addOperand("cop");
        addOperand("stk");
        addOperand("mrc");
        addOperand("ctl");
    }

//-------------------------------------------------------------------------------------
    private void addOperand(String _operandName) {
        int _operandData = context.getArchitectureImplementations().getArchitecture(architectureId).get("ISA_" + _operandName);
        Operand _operandObj = new Operand(_operandName, _operandData);
        operands.put(_operandName, _operandObj);
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
