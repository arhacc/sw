//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.isa.instruction;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import com.opencsv.*;
import com.opencsv.exceptions.*;

import org.apache.lucene.util.*;
import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.utils.*;
import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.context.arch.*;
import xpu.sw.tools.sdk.asm.parser.*;
import xpu.sw.tools.sdk.common.isa.flow.*;

//-------------------------------------------------------------------------------------

public class Instruction {
    private InstructionLine instructionLine;

    private String name;
    private Opcode opcode;
    private Operand operand;
    private Value value;

//    private boolean isHalt;
    private byte[] packedInstruction;

//-------------------------------------------------------------------------------------
    public Instruction(String _name, Opcode _opcode, Operand _operand, Value _value) {
        name = _name;
        opcode = _opcode;
        operand = _operand;
        value = _value;

//        isHalt = _name.equals("halt");
//        int _valueLength = _primitive.getArhCode() - 8;
//        value = Value.getValue(_value, _valueLength);

    }

//-------------------------------------------------------------------------------------
    public String getName(){
        return name;
    }

//-------------------------------------------------------------------------------------
    public Instruction copyOf(){
        return new Instruction(name, opcode, operand, value.copyOf());            
    }

//-------------------------------------------------------------------------------------
    public int getAddress(){
        return instructionLine.getLocalization().getAbsoluteAddress();
    }

//-------------------------------------------------------------------------------------
    public Value getValue(){
        return value;
    }
/*
//-------------------------------------------------------------------------------------
    public boolean isHalt() {
        return isHalt;
    }
*/

//-------------------------------------------------------------------------------------
    public InstructionLine getInstructionLine() {
        return instructionLine;
    }

//-------------------------------------------------------------------------------------
    public void setInstructionLine(InstructionLine _instructionLine) {
        instructionLine = _instructionLine;
    }
/*
//-------------------------------------------------------------------------------------
    public void replaceParametersWithExpressions(List<String> _parameters, List<AsmParser.ExpressionContext>  _expressions) {
        value.replaceParametersWithExpressions(_parameters, _expressions);
    }
*/
//-------------------------------------------------------------------------------------
    public boolean link(InstructionLine _instructionLine) {
//        System.out.println("Linking Instruction:" +this);
        instructionLine = _instructionLine;
        return value.link(this);
    }
    
//-------------------------------------------------------------------------------------
    public boolean resolve() {
        return value.resolve();
    }
    
//-------------------------------------------------------------------------------------
    public boolean pack(ArchitectureImplementation _architectureImplementation) {
        opcode.pack(_architectureImplementation.getOpcodeWidth());
        operand.pack(_architectureImplementation.getOperandWidth());
        value.pack(_architectureImplementation.getValueWidth());
//        System.out.println("Field.toHex.In:\n 1:" + opcode.dump() + "\n 2:" + operand.dump() + "\n 3:" +value.dump() + ",_architectureImplementation=" + _architectureImplementation);
//        System.out.println("Field.toHex.In:\n 1:" + _field1.dump() + "\n 2:" + _field2.dump() + "\n 3:" +_field3.dump());
        FixedBitSet _data;
        _data = BitUtils.concatenate(opcode.data, operand.data);
        _data = BitUtils.concatenate(_data, value.data);
//        System.out.println("Field.toHex.Out:" + _data);
        packedInstruction = BitUtils.toByteArray(_data);
/*        System.out.print("Field._array:");
        for(int i = 0; i < packedInstruction.length; i++){
            System.out.print(i + "=[" + packedInstruction[i] + "], ");
        }
        System.out.println("");*/
//        return byteToInt(_array);
//        packedInstruction = Field.pack(_architectureImplementation, opcode, operand, value);
        return true;
    }

//-------------------------------------------------------------------------------------
    public int toBin() {
        return byteToInt(packedInstruction);
    }

//-------------------------------------------------------------------------------------
    public String toHex() {
        return xpu.sw.tools.sdk.common.utils.StringUtils.bytesToHex(packedInstruction);
    }

//-------------------------------------------------------------------------------------
/** width should be less than 4 (for int) **/
    public static int byteToInt(byte[] _bytes) {
        int val = 0;
        if(_bytes.length > 4) throw new RuntimeException("Too big to fit in int");
        for (int i = 0; i < _bytes.length; i++) {
            val=val<<8;
            val=val|(_bytes[i] & 0xFF);
        }
        return val;
    }
//-------------------------------------------------------------------------------------
    public String toString() {
        return getName();
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
