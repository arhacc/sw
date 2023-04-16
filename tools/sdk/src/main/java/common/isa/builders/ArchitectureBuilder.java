//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.isa.builders;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import com.opencsv.*;
import com.opencsv.exceptions.*;

import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;
import org.apache.commons.lang3.tuple.*;

import xpu.sw.tools.sdk.common.isa.*;
import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.context.arch.*;

import xpu.sw.tools.sdk.asm.parser.*;

//-------------------------------------------------------------------------------------
public class ArchitectureBuilder extends AbstractBuilder {
    private Map<String, Pair<ControlInstructionBuilder, ArrayInstructionBuilder>> architectures;

//-------------------------------------------------------------------------------------
    public ArchitectureBuilder(Context _context) {
        super(_context);
        architectures = new HashMap<String, Pair<ControlInstructionBuilder, ArrayInstructionBuilder>>();
    }

//-------------------------------------------------------------------------------------
    public Instruction buildControlInstruction(AsmParser.ControlInstructionContext _ctx, Primitive _primitive) {
//        log.debug("InstructionBuilder: " + _opcode + ", " + _valueString + ", " + _valueNumber);
        Pair<ControlInstructionBuilder, ArrayInstructionBuilder> _instructionBuilder = getInstructionBuilder(_primitive.getArhCode());
        if(_instructionBuilder == null){
            return null;
        }
        return _instructionBuilder.getLeft().build(_ctx, _primitive);
    }   

//-------------------------------------------------------------------------------------
    public Instruction buildArrayInstruction(AsmParser.ArrayInstructionContext _ctx, Primitive _primitive) {
//        log.debug("InstructionBuilder: " + _opcode + ", " + _valueString + ", " + _valueNumber);
        Pair<ControlInstructionBuilder, ArrayInstructionBuilder> _instructionBuilder = getInstructionBuilder(_primitive.getArhCode());
        if(_instructionBuilder == null){
            return null;
        }
        return _instructionBuilder.getRight().build(_ctx, _primitive);
    }   

//-------------------------------------------------------------------------------------
    private Pair<ControlInstructionBuilder, ArrayInstructionBuilder> getInstructionBuilder(String _arhCode) {
        Pair<ControlInstructionBuilder, ArrayInstructionBuilder> _instructionBuilder = architectures.get(_arhCode);
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
    private Pair<ControlInstructionBuilder, ArrayInstructionBuilder> addArchitecture(String _arhCode) {
        //check if exists
        ArchitectureImplementation _architectureImplementation = context.getArchitectureImplementations().getArchitecture(_arhCode);
        if(_architectureImplementation == null){
            log.error("Unimplemented architecture: " + _arhCode);
            return null;
        }
        Pair<ControlInstructionBuilder, ArrayInstructionBuilder> _architecture = 
            Pair.of(new ControlInstructionBuilder(context, _arhCode), new ArrayInstructionBuilder(context, _arhCode));
        architectures.put(_arhCode, _architecture);
        return _architecture;
    }   

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
