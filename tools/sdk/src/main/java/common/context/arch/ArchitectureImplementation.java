//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.context.arch;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;
import java.time.*;

import org.apache.commons.cli.*;
import org.apache.commons.configuration2.*;
import org.apache.logging.log4j.*;

import jakarta.json.*;

import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.xbasics.*;

//-------------------------------------------------------------------------------------
public class ArchitectureImplementation extends XBasic {
    private String name;
    private HashMap<String, Integer> architectureImplementation;

    //precalculated ISA attributes:
    private int opcodeWidth;
    private int operandWidth;
    private int valueWidth;

    public static final String DEFAULT_ARCHITECTURE = "noarch";

//-------------------------------------------------------------------------------------
    public ArchitectureImplementation(Context _context, String _name, JsonObject _archObject) {
        super(_context);
        name = _name;
        architectureImplementation = new HashMap<String, Integer>();
        _archObject.forEach((_key, _value) -> {
            architectureImplementation.put(_key, ((JsonNumber)_value).intValue());
        });
/*
        log.debug("Machine parameters: nCells=" + nCells + ", memCodeControllerSizeLog=" + memCodeControllerSizeLog
                + ", memCodeArraySizeLog=" + memCodeArraySizeLog + ", memDataArraySizeLog=" + memDataArraySizeLog
                + ", memFeatureSizeLog=" + memFeatureSizeLog
        );
*/        
        if(!_name.equals(DEFAULT_ARCHITECTURE)){
            opcodeWidth = get("opcodeWidth");
            operandWidth = get("operandWidth");
            valueWidth = get("valueWidth");            
        }
        
    }

//-------------------------------------------------------------------------------------
    public int get(String _attribute) {
        Integer _value = architectureImplementation.get(_attribute);
        if(_value == null){
            log.warn("Warning: cannot find attribute [" + _attribute + "] in architecure " + name);
            return -1;
        }
        return _value;
    }

//-------------------------------------------------------------------------------------
    public String getName() {
        return name;
    }

//-------------------------------------------------------------------------------------
    public int getOpcodeWidth() {
        return opcodeWidth;
    }

//-------------------------------------------------------------------------------------
    public int getOperandWidth() {
        return operandWidth;
    }

//-------------------------------------------------------------------------------------
    public int getValueWidth() {
        return valueWidth;
    }

//-------------------------------------------------------------------------------------
    public int getNCells() {
        return get("nCells");
    }

//-------------------------------------------------------------------------------------
    public int getMemDataArraySizeLog() {
        return get("memCodeArraySizeLog");
    }

//-------------------------------------------------------------------------------------
    public int getMemCodeControllerSizeLog() {
        return get("memCodeControllerSizeLog");
    }

//-------------------------------------------------------------------------------------
    public int getMemCodeArraySizeLog() {
        return get("memDataArraySizeLog");
    }

//-------------------------------------------------------------------------------------
    public int getMemFeatureSizeLog() {
        return get("memFeatureSizeLog");
    }

//-------------------------------------------------------------------------------------
    public String toString() {
        return name + " : opcodeWidth=" + opcodeWidth + ", operandWidth=" + operandWidth + ", valueWidth=" + valueWidth;
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
