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
import xpu.sw.tools.sdk.common.context.arch.*;

//-------------------------------------------------------------------------------------
public class ArchitectureBuilder extends Builder {
    private Map<String, InstructionBuilder> architectures;

//-------------------------------------------------------------------------------------
    public ArchitectureBuilder(Context _context) {
        super(_context);
        architectures = new HashMap<String, InstructionBuilder>();
    }

//-------------------------------------------------------------------------------------
    public Instruction build(String _opcode, String _valueString, int _valueNumber, Primitive _primitive) {
//        log.debug("InstructionBuilder: " + _opcode + ", " + _valueString + ", " + _valueNumber);
        InstructionBuilder _instructionBuilder = getInstructionBuilder(_primitive.getArhCode());
        if(_instructionBuilder == null){
            return null;
        }
        return _instructionBuilder.build(_opcode, _valueString, _valueNumber, _primitive);
    }   

//-------------------------------------------------------------------------------------
    private InstructionBuilder getInstructionBuilder(String _arhCode) {
        InstructionBuilder _instructionBuilder = architectures.get(_arhCode);
        if(_instructionBuilder == null){
            _instructionBuilder = addArchitecture(_arhCode);
        }
        return _instructionBuilder;
    }   
/*
//-------------------------------------------------------------------------------------
    public Opcode getOpcode(String _arhCode, int _opcode) {
        InstructionBuilder _instructionBuilder = getInstructionBuilder(_arhCode);
        if(_instructionBuilder == null){
            return null;
        }
        return _instructionBuilder.getOpcode(_opcode);
    }
*/
//-------------------------------------------------------------------------------------
    private InstructionBuilder addArchitecture(String _arhCode) {
        //check if exists
        ArchitectureImplementation _architectureImplementation = context.getArchitectureImplementations().getArchitecture(_arhCode);
        if(_architectureImplementation == null){
            log.error("Unimplemented architecture: " + _arhCode);
            return null;
        }
        InstructionBuilder _architecture = new InstructionBuilder(context, _arhCode);
        architectures.put(_arhCode, _architecture);
        return _architecture;
    }   

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
