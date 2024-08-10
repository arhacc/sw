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
    private boolean isDefault;
    private HashMap<String, Long> architectureImplementation;

    //precalculated ISA attributes:
    private int opcodeWidth;
    private int operandWidth;
    private int valueWidth;

    public static final String XPU_ANY      = "xpu_ANY";
    
//-------------------------------------------------------------------------------------
    public ArchitectureImplementation(Context _context, File _directoryFile, boolean _isDefault) {
        super(_context);
        name = FilenameUtils.getBaseName(_directoryFile.getName());
        isDefault = _isDefault;
        architectureImplementation = new HashMap<String, Long>();

        File _file = new File(_directoryFile, name + ".def");
        
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
        opcodeWidth = get("INSTR_OPCODE_NR_BITS");
        operandWidth = get("INSTR_OPERAND_NR_BITS");
        valueWidth = get("INSTR_VALUE_NR_BITS");
//        String _text = "Found valid architectureImplementation: [" + name + "]";
        if(isDefault){
//            _text += "<-----[DEFAULT]";
            log.debug("Default architectureImplementation: [" + name + "]");
        }
//        log.debug(_text);
    }

//-------------------------------------------------------------------------------------
    public int get(String _attribute) {
        return (int)getLong(_attribute);
    }

//-------------------------------------------------------------------------------------
    public long getLong(String _attribute) {
        Long _value = architectureImplementation.get(_attribute);
        if(_value == null){
            log.error("Error: [" + _attribute + "] is not defined either as define or in architecure " + name);
//            System.exit(1);
            return -1;
        }
//        log.warn("attribute [" + _attribute + "] = " + _value);
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
        return get("ARRAY_NR_CELLS");
    }

//-------------------------------------------------------------------------------------
    public int getMemDataArraySize() {
        return get("ARRAY_CELL_MEM_SIZE");
    }

//-------------------------------------------------------------------------------------
    public int getMemCodeControllerSize() {
        return get("memCodeControllerSizeLog");
    }

//-------------------------------------------------------------------------------------
    public int getMemCodeArraySize() {
        return get("memDataArraySizeLog");
    }

//-------------------------------------------------------------------------------------
    public int getMemFeatureSize() {
        return get("memFeatureSizeLog");
    }

//-------------------------------------------------------------------------------------
    public String toString() {
        return name + " : opcodeWidth=" + opcodeWidth + ", operandWidth=" + operandWidth + ", valueWidth=" + valueWidth;
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
