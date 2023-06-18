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

//-------------------------------------------------------------------------------------
public class InstructionLine {
    private Callable callableParent;
    private int address;
    private Instruction controlInstruction;
    private Instruction arrayInstruction;

//-------------------------------------------------------------------------------------
    public InstructionLine() {
    }

//-------------------------------------------------------------------------------------
    public boolean isEmpty(){
        return (controlInstruction == null) || (arrayInstruction == null);
    }

//-------------------------------------------------------------------------------------
    public InstructionLine copyOf(){
        InstructionLine _instructionLine = new InstructionLine();
        _instructionLine.callableParent = callableParent;
        _instructionLine.address = address;
        _instructionLine.controlInstruction = controlInstruction.copyOf();
        _instructionLine.arrayInstruction = arrayInstruction.copyOf();
        return _instructionLine;
    }

//-------------------------------------------------------------------------------------
    public int getAddress(){
        return address;
    }

//-------------------------------------------------------------------------------------
    public void setControllerInstruction(Instruction _instruction) {
        controlInstruction = _instruction;
    }

//-------------------------------------------------------------------------------------
    public void setArrayInstruction(Instruction _instruction) {
        arrayInstruction = _instruction;
    }

//-------------------------------------------------------------------------------------
    public void replaceParametersWithExpressions(List<String> _parameters, List<AsmParser.ExpressionContext> _expressions) {
        controlInstruction.replaceParametersWithExpressions(_parameters, _expressions);
        arrayInstruction.replaceParametersWithExpressions(_parameters, _expressions);
    }

//-------------------------------------------------------------------------------------
    public boolean link(Context _context, int _address) {
        address = _address;
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
        return _success;
    }
    
//-------------------------------------------------------------------------------------
    public boolean resolve() {
        return controlInstruction.resolve() & 
                arrayInstruction.resolve();
    }
    
//-------------------------------------------------------------------------------------
    public boolean pack(ArchitectureImplementation _architectureImplementation) {
        return controlInstruction.pack(_architectureImplementation) & 
                arrayInstruction.pack(_architectureImplementation);
    }

//-------------------------------------------------------------------------------------
    public long toBin() {
        long _dataHi = (long)controlInstruction.toBin();
        int _dataLo = arrayInstruction.toBin();
        return (_dataHi << 32) | _dataLo;
    }

//-------------------------------------------------------------------------------------
    public String toHex() {
        return controlInstruction.toHex() + " " + arrayInstruction.toHex();
    }

//-------------------------------------------------------------------------------------
    public String toString() {
        return toHex();
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
