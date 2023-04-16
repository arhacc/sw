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

import xpu.sw.tools.sdk.asm.parser.*;

//-------------------------------------------------------------------------------------
public abstract class InstructionBuilder extends AbstractBuilder {
    protected String architectureId;
    protected Map<String, Instruction> instructions;
    protected OpcodeBuilder opcodeBuilder;
    protected OperandBuilder operandBuilder;
    protected ValueBuilder valueBuilder;

    protected ValueFormat standardNoArgsValueFormat;
    protected ValueFormat standardOneArgValueFormat;
    protected ValueFormat shrightValueFormat;
    protected ValueFormat rotrightLocalValueFormat;
    protected ValueFormat grshiftWobValueFormat;
    protected ValueFormat jmpValueFormat;
    protected ValueFormat whereZeroValueFormat;
    protected ValueFormat scannopValueFormat;
    protected ValueFormat scansplitValueFormat;
    protected ValueFormat addrRegStackDuplicateValueFormat;
    protected ValueFormat selAddrregValueFormat;
    protected ValueFormat ccStartWHaltValueFormat;
    protected ValueFormat addrStoreValueFormat;
    protected ValueFormat setvalValueFormat;
    protected ValueFormat ixLoadValueFormat;
    protected ValueFormat stackPopValueFormat;

//-------------------------------------------------------------------------------------
    public InstructionBuilder(Context _context, String _architectureId) {
        super(_context);
        architectureId = _architectureId;
        instructions = new HashMap<String, Instruction>();
        opcodeBuilder = new OpcodeBuilder(_context, _architectureId);
        operandBuilder = new OperandBuilder(_context, _architectureId);
        valueBuilder = new ValueBuilder(_context, _architectureId);
        init();        
    }

//-------------------------------------------------------------------------------------
    protected void init(){
        standardNoArgsValueFormat = new ValueFormat(
            new String[]{"INSTR_VALUE_NR_BITS",          "INSTR_VALUE_LOC"}
            );

        standardOneArgValueFormat = new ValueFormat(
            new String[]{"INSTR_VALUE_NR_BITS",          "INSTR_VALUE_LOC"}
            );

        shrightValueFormat = new ValueFormat(
            new String[]{"INSTR_FIXED_SHIFTING_NR_BITS",          "INSTR_FIXED_SHIFTING_LOC"}
            );
        rotrightLocalValueFormat = new ValueFormat(
            new String[]{"INSTR_SH_ROTATE_LOCAL_CTRL_BITS_NR_BITS"      , "INSTR_SH_ROTATE_LOCAL_CTRL_LOC"},
            new String[]{"INSTR_SH_ROTATE_LOCAL_SPLIT_POINT_NR_BITS"    , "INSTR_SH_ROTATE_LOCAL_SPLIT_POINT_LOC"},
            new String[]{"INSTR_SH_ROTATE_LOCAL_AMOUNT_NR_BITS"         , "INSTR_SH_ROTATE_LOCAL_SHIFT_AMOUNT_LOC"}
            );
        grshiftWobValueFormat = new ValueFormat(
            new String[]{"INSTR_GLOBAL_NR_BITS"      , "INSTR_GLOBAL_LOC"}
            );
        jmpValueFormat = new ValueFormat(
            new String[]{"INSTR_JMP_FUNCTION_NR_BITS",          "INSTR_JMP_FUNCTION_LOC"},
            new String[]{"INSTR_JMP_FUNCTION_BR_w_VAL_NR_BITS", "INSTR_JMP_FUNCTION_BR_w_VAL_LOC"},
            new String[]{"INSTR_JMP_VALUE_NR_BITS",             "INSTR_JMP_VALUE_LOC"}
            );
        whereZeroValueFormat = new ValueFormat(
            new String[]{"INSTR_SPATIAL_SELECT_FUNCTION_NR_BITS",          "INSTR_SPATIAL_SELECT_FUNCTION_LOC"},
            new String[]{"INSTR_SPATIAL_SELECT_WHERE_COND_NR_BITS",        "INSTR_SPATIAL_SELECT_WHERE_COND_LOC"}
            );
        scannopValueFormat = new ValueFormat(
            new String[]{"NETWORK_NR_OPCODE_BITS",          "INSTR_SETSCAN_OPCODE_LOC"}
            );
        scansplitValueFormat = new ValueFormat(
            new String[]{"NETWORK_NR_OPCODE_BITS",              "INSTR_SETSCAN_OPCODE_LOC"},
            new String[]{"NETWORK_SCAN_MODES_NR_BITS",          "INSTR_SETSCAN_ADDR_MODE_LOC"}
            );
        addrRegStackDuplicateValueFormat = new ValueFormat(
            new String[]{"INSTR_MISC_STORE_LOAD_NR_BITS",          "INSTR_MISC_STORE_LOAD_LOC"}
            );
        selAddrregValueFormat = new ValueFormat(
            new String[]{"INSTR_MISC_STORE_LOAD_NR_BITS",          "INSTR_MISC_STORE_LOAD_LOC"},
            new String[]{"CONTROLLER_ADDR_REG_SELECTOR_NR_BITS", "INSTR_MISC_STORE_LOAD_VALUE_LOC"}
            );
        ccStartWHaltValueFormat = new ValueFormat(
            new String[]{"INSTR_MISC_TESTING_SEL_NR_BITS",          "INSTR_MISC_TESTING_SEL_LOC"}
            );
        addrStoreValueFormat = new ValueFormat(
            new String[]{"INSTR_MISC_STORE_LOAD_NR_BITS",          "INSTR_MISC_STORE_LOAD_LOC"}
            );
        setvalValueFormat = new ValueFormat(
            new String[]{"INSTR_MISC_STORE_LOAD_NR_BITS",          "INSTR_MISC_STORE_LOAD_LOC"},
            new String[]{"INSTR_JMP_FUNCTION_BR_w_VAL_NR_BITS",    "INSTR_MISC_STORE_LOAD_BR_VAL_REG_SEL_LOC"}
            );
        ixLoadValueFormat = new ValueFormat(
            new String[]{"INSTR_MISC_TESTING_SEL_NR_BITS",          "INSTR_MISC_TESTING_SEL_LOC"}
            );
        stackPopValueFormat = new ValueFormat(
            new String[]{"ISA_stack_operations_CTL_val_nr_bits",          "ISA_stack_operations_CTL_val_LOC"}
            );
    }

//-------------------------------------------------------------------------------------
    public Instruction build(String _opcode, String[] _args, Primitive _primitive) {
//        log.debug("InstructionBuilder: " + _opcode + ", " + _valueString + ", " + _valueNumber);
        Instruction _instruction = instructions.get(_opcode);
        if(_instruction == null){
            log.error("Error: cannot find instruction: " + _opcode);
            System.exit(1);
        }
        _instruction = _instruction.clone();
        _instruction.setPrimitive(_primitive);
        _instruction.setArgs(_args);
        return _instruction;
    }   

//-------------------------------------------------------------------------------------
    protected void addInstruction(String _instructionName, String _opcode, String _operand) {
        addInstruction(_instructionName, _opcode, _operand, standardNoArgsValueFormat);
    }

//-------------------------------------------------------------------------------------
    protected void addInstruction(String _instructionName, String _opcode, String _operand, ValueFormat _valueFormat) {
        addInstruction(_instructionName, _opcode, _operand, _valueFormat, "ZERO");
    }

//-------------------------------------------------------------------------------------
    protected void addInstruction(String _instructionName, String _opcode, String _operand, ValueFormat _valueFormat, String _value) {
        addInstruction(_instructionName, _opcode, _operand, _valueFormat, new String[]{_value});
    }

//-------------------------------------------------------------------------------------
    protected void addInstruction(String _instructionName, String _opcode, String _operand, ValueFormat _valueFormat, String[] _values) {
        Opcode _opcodeObj = opcodeBuilder.get(_opcode);
        Operand _operandObj = operandBuilder.get(_operand);
        Value _valueObj = valueBuilder.get(_valueFormat, _values);

        Instruction _instruction = new Instruction(_instructionName, _opcodeObj, _operandObj, _valueObj);
        instructions.put(_instructionName, _instruction);
    }

//-------------------------------------------------------------------------------------
    protected String extractValue(AsmParser.ValueContext _valueContext) {
        if(_valueContext != null){
            AsmParser.NameContext _nameContext = _valueContext.name();
            if(_nameContext != null){
                return _nameContext.NAME().getText();
            }

            AsmParser.NumberContext _numberContext = _valueContext.number();
            if(_numberContext != null){
                return _numberContext.NUMBER().getText();
            }
        }
        return "";
    }

//-------------------------------------------------------------------------------------
    protected String extractLabel(AsmParser.LbContext _lbContext) {
        if(_lbContext != null){
            AsmParser.NameContext _nameContext = _lbContext.name();
            if(_nameContext != null){
                return _nameContext.NAME().getText();
            }
        }
        return "";
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
