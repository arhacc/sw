//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.isa;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.context.arch.*;
import xpu.sw.tools.sdk.asm.parser.*;
import xpu.sw.tools.sdk.common.xbasics.*;

//-------------------------------------------------------------------------------------
public class InstructionLine extends XBasic {
    private Callable callableParent;
    private int type;
    private int address;
    private Instruction controlInstruction;
    private Instruction arrayInstruction;
    private Macro macro;

    public static final int TYPE_UNKNOWN        = 0;
    public static final int TYPE_INSTRUCTION    = 1;
    public static final int TYPE_MACRO          = 2;

//-------------------------------------------------------------------------------------
    public InstructionLine(Context _context) {
        super(_context);
        type = TYPE_UNKNOWN;
    }

/*
//-------------------------------------------------------------------------------------
    public boolean isEmpty(){
        return (controlInstruction == null) || (arrayInstruction == null);
    }
*/
//-------------------------------------------------------------------------------------
    public InstructionLine copyOf(){
        InstructionLine _instructionLine = new InstructionLine(context);
        _instructionLine.callableParent = callableParent;
        _instructionLine.type = type;
        _instructionLine.address = address;
        if(type == TYPE_INSTRUCTION){
            _instructionLine.controlInstruction = controlInstruction.copyOf();
            _instructionLine.arrayInstruction = arrayInstruction.copyOf();
        } else {
            _instructionLine.macro = macro.copyOf();
        }
        return _instructionLine;
    }

//-------------------------------------------------------------------------------------
    public int getAddress(){
        return address;
    }

//-------------------------------------------------------------------------------------
    public void setControllerInstruction(Instruction _instruction) {
        controlInstruction = _instruction;
        type = TYPE_INSTRUCTION;
    }

//-------------------------------------------------------------------------------------
    public void setArrayInstruction(Instruction _instruction) {
        arrayInstruction = _instruction;
        type = TYPE_INSTRUCTION;
    }

//-------------------------------------------------------------------------------------
    public void setMacro(Macro _macro) {
        macro = _macro;
        type = TYPE_MACRO;
    }

/*
//-------------------------------------------------------------------------------------
    public void replaceParametersWithExpressions(List<String> _parameters, List<AsmParser.ExpressionContext> _expressions) {
        controlInstruction.replaceParametersWithExpressions(_parameters, _expressions);
        arrayInstruction.replaceParametersWithExpressions(_parameters, _expressions);
    }
*/
//-------------------------------------------------------------------------------------
    public int link(int _address) {
        address = _address;
        if(type == TYPE_MACRO){
            return macro.link(_address);
        }
/*        if(controlInstruction == null){
            _context.getLog().error("controlInstruction is null, address=" + address);
            return false;
        }
        if(arrayInstruction == null){
            _context.getLog().error("arrayInstruction is null, address=" + address);
            return false;
        }*/
        boolean _success = controlInstruction.link(this) & arrayInstruction.link(this);
//        _context.getLog().error("link address=" + address + ", controlInstruction=" + controlInstruction + ", arrayInstruction="+ arrayInstruction);
        return address + 1;
    }
    
//-------------------------------------------------------------------------------------
    public boolean resolve() {
        if(type == TYPE_MACRO){
            return macro.resolve();
        }
        return controlInstruction.resolve() & 
                arrayInstruction.resolve();
    }
    
//-------------------------------------------------------------------------------------
    public boolean pack(ArchitectureImplementation _architectureImplementation) {
        if(type == TYPE_MACRO){
            return macro.pack();
        }
        return controlInstruction.pack(_architectureImplementation) & 
                arrayInstruction.pack(_architectureImplementation);
    }

//-------------------------------------------------------------------------------------
    public List<Long> toBin() {
        if(type == TYPE_MACRO){
            return macro.toBin();
        }
        long _dataHi = (long)controlInstruction.toBin();
        int _dataLo = arrayInstruction.toBin();
        List<Long> _bin = new ArrayList<Long>();
        _bin.add((_dataHi << 32) | _dataLo);
        return _bin;
    }

//-------------------------------------------------------------------------------------
    public String toHex() {
        if(type == TYPE_MACRO){
            return macro.toHex();
        }
//        return address + ":" + controlInstruction.toHex() + " " + arrayInstruction.toHex() + "\n";
        return controlInstruction.toHex() + " " + arrayInstruction.toHex() + "\n";
    }

//-------------------------------------------------------------------------------------
    public String toString() {
        return toHex();
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
