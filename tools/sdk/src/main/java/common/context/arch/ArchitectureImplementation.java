//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.context.arch;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;
import java.time.*;

import org.apache.commons.io.*;
import org.apache.commons.cli.*;
import org.apache.commons.configuration2.*;
import org.apache.logging.log4j.*;

import jakarta.json.*;

import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.xbasics.*;

//-------------------------------------------------------------------------------------
public class ArchitectureImplementation extends XBasic {
    private String name;
    private HashMap<String, Long> architectureImplementation;

    //precalculated ISA attributes:
    private int opcodeWidth;
    private int operandWidth;
    private int valueWidth;

    public static final String DEFAULT_ARCHITECTURE = "noarch";

//-------------------------------------------------------------------------------------
    public ArchitectureImplementation(Context _context, File _file) {
        super(_context);
        name = FilenameUtils.getBaseName(_file.getName());
        architectureImplementation = new HashMap<String, Long>();

        try{
            Scanner _input = new Scanner(_file);
            while (_input.hasNextLine()) {
                String _line = _input.nextLine().trim().replaceAll("\\s+", " ");
                String[] _items = _line.split(" ");
                if(_items.length == 2){
                    String _key = _items[0];
                    String _value = _items[1];
                    try {
                        long _long = Long.parseLong(_value);
                        architectureImplementation.put(_key, _long);                        
                    } catch(NumberFormatException _e1){
                        
                    }
                }
            }
        }catch(IOException _e){
            log.error("Cannot load ArchitectureImplementation: " + _e.getMessage());
            return;
        }
/*
        log.debug("Machine parameters: nCells=" + nCells + ", memCodeControllerSizeLog=" + memCodeControllerSizeLog
                + ", memCodeArraySizeLog=" + memCodeArraySizeLog + ", memDataArraySizeLog=" + memDataArraySizeLog
                + ", memFeatureSizeLog=" + memFeatureSizeLog
        );
*/        
        if(!name.equals(DEFAULT_ARCHITECTURE)){
            opcodeWidth = get("INSTR_OPCODE_NR_BITS_unguarded");
            operandWidth = get("INSTR_OPERAND_NR_BITS_unguarded");
            valueWidth = get("INSTR_VALUE_NR_BITS_unguarded");            
        }
        
    }

//-------------------------------------------------------------------------------------
    public int get(String _attribute) {
        return (int)getLong(_attribute);
    }

//-------------------------------------------------------------------------------------
    public long getLong(String _attribute) {
        Long _value = architectureImplementation.get(_attribute);
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
