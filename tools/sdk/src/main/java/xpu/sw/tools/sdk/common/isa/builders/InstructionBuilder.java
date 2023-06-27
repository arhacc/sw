//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.isa.builders;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import com.opencsv.*;
import com.opencsv.exceptions.*;

import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.isa.flow.*;
import xpu.sw.tools.sdk.common.isa.instruction.*;
import xpu.sw.tools.sdk.common.context.*;

import xpu.sw.tools.sdk.asm.parser.*;

//-------------------------------------------------------------------------------------
public abstract class InstructionBuilder extends AbstractBuilder {
    protected Map<String, Instruction> instructions;

    protected OpcodeBuilder opcodeBuilder;
    protected OperandBuilder operandBuilder;
    protected ValueBuilder valueBuilder;

    protected static final AsmParser.ExpressionContext ZERO = null;

//-------------------------------------------------------------------------------------
    public InstructionBuilder(Context _context, String _architectureId) {
        super(_context, _architectureId);
        instructions = new HashMap<String, Instruction>();
        opcodeBuilder = new OpcodeBuilder(_context, _architectureId);
        operandBuilder = new OperandBuilder(_context, _architectureId);
        valueBuilder = new ValueBuilder(_context, _architectureId);
        init();        
    }

//-------------------------------------------------------------------------------------
    protected void init(){

    }

//-------------------------------------------------------------------------------------
    public Instruction build(String _opcode, String _label, AsmParser.ExpressionContext _expression) {
//        log.debug("InstructionBuilder: " + _opcode + ", _args=" + _argumentValues);
        Instruction _instruction = instructions.get(_opcode);
        if(_instruction == null){
            log.error("Error: cannot find instruction: " + _opcode);
//            System.exit(1);
            return null;
        }
        _instruction = _instruction.copyOf();
        _instruction.getValue().setArgumentValues(_label, _expression);
        return _instruction;
    }   

//-------------------------------------------------------------------------------------
    protected void addInstruction(String _instructionName, String _opcode, String _operand) {
        addInstruction(_instructionName, _opcode, _operand, "standardValueFormat");
    }

//-------------------------------------------------------------------------------------
    protected void addInstruction(String _instructionName, String _opcode, String _operand, String _valueFormat) {
        addInstruction(_instructionName, _opcode, _operand, _valueFormat, "ZERO");
    }

//-------------------------------------------------------------------------------------
    protected void addInstruction(String _instructionName, String _opcode, String _operand, String _valueFormat, String _argumentReference) {
        addInstruction(_instructionName, _opcode, _operand, _valueFormat, new String[]{_argumentReference});
    }

//-------------------------------------------------------------------------------------
    protected void addInstruction(String _instructionName, String _opcode, String _operand, String _valueFormat, String[] _argumentReferences) {
        Opcode _opcodeObj = opcodeBuilder.get(_opcode);
        Operand _operandObj = operandBuilder.get(_operand);
        Value _valueObj = valueBuilder.get(_valueFormat, _argumentReferences);

        Instruction _instruction = new Instruction(_instructionName, _opcodeObj, _operandObj, _valueObj);
        instructions.put(_instructionName, _instruction);
    }
/*
//-------------------------------------------------------------------------------------
    protected String extractExpression(AsmParser.ValueContext _valueContext) {
        if(_valueContext != null){
            AsmParser.NameContext _nameContext = _valueContext.name();
            if((_nameContext != null) && (_nameContext.NAME() != null)){
                return _nameContext.NAME().getText();
            }

            AsmParser.NumberContext _numberContext = _valueContext.number();
            if((_numberContext != null) && (_numberContext.NUMBER() != null)){
                String _number = "";
                _number += _numberContext.NUMBER().getText();
                return _number;
            }
        }
        return "";
    }
*/
//-------------------------------------------------------------------------------------
    protected String extractLabel(AsmParser.LbContext _lbContext) {
        if(_lbContext != null){
            AsmParser.NameContext _nameContext = _lbContext.name();
            if((_nameContext != null) && (_nameContext.NAME() != null)){
                return _nameContext.NAME().getText();
            }
        }
        return "";
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
