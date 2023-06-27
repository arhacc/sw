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
    private Map<String, Pair<ControllerInstructionBuilder, ArrayInstructionBuilder>> architectures;

//-------------------------------------------------------------------------------------
    public ArchitectureBuilder(Context _context) {
        super(_context);
        architectures = new HashMap<String, Pair<ControllerInstructionBuilder, ArrayInstructionBuilder>>();
    }

//-------------------------------------------------------------------------------------
    public Instruction buildControllerInstruction(AsmParser.ControllerInstructionContext _ctx, Callable _callable) {
//        log.debug("InstructionBuilder: " + _opcode + ", " + _valueString + ", " + _valueNumber);
        Pair<ControllerInstructionBuilder, ArrayInstructionBuilder> _architecture = getInstructionBuilder(_callable.getApplication().getArchitectureId());
//        log.debug("ArchitectureBuilder.buildControllerInstruction: " + _ctx + ", _architecture=" + _architecture);
        if(_architecture == null){
            return null;
        }
        return _architecture.getLeft().build(_ctx, _callable);
    }   

//-------------------------------------------------------------------------------------
    public Instruction buildArrayInstruction(AsmParser.ArrayInstructionContext _ctx, Callable _callable) {
//        log.debug("InstructionBuilder: " + _opcode + ", " + _valueString + ", " + _valueNumber);
        Pair<ControllerInstructionBuilder, ArrayInstructionBuilder> _architecture = getInstructionBuilder(_callable.getApplication().getArchitectureId());
//        log.debug("ArchitectureBuilder,buildArrayInstruction: " + _ctx + ", _architecture=" + _architecture);
        if(_architecture == null){
            return null;
        }
        return _architecture.getRight().build(_ctx, _callable);
    }   

//-------------------------------------------------------------------------------------
    private Pair<ControllerInstructionBuilder, ArrayInstructionBuilder> getInstructionBuilder(String _architectureId) {
        Pair<ControllerInstructionBuilder, ArrayInstructionBuilder> _architecture = architectures.get(_architectureId);
        if(_architecture == null){
            _architecture = addArchitecture(_architectureId);
        }
        return _architecture;
    }   
/*
//-------------------------------------------------------------------------------------
    public Opcode getOpcode(String _architectureId, int _opcode) {
        InstructionBuilder _instructionBuilder = getInstructionBuilder(_architectureId);
        if(_instructionBuilder == null){
            return null;
        }
        return _instructionBuilder.getOpcode(_opcode);
    }
*/
//-------------------------------------------------------------------------------------
    private Pair<ControllerInstructionBuilder, ArrayInstructionBuilder> addArchitecture(String _architectureId) {
        //check if exists
        ArchitectureImplementation _architectureImplementation = context.getArchitectureImplementations().getArchitecture(_architectureId);
        if(_architectureImplementation == null){
            log.error("Unimplemented architecture: " + _architectureId);
            return null;
        }
        Pair<ControllerInstructionBuilder, ArrayInstructionBuilder> _architecture = 
            Pair.of(new ControllerInstructionBuilder(context, _architectureId), new ArrayInstructionBuilder(context, _architectureId));
        architectures.put(_architectureId, _architecture);
        return _architecture;
    }   

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
