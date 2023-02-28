//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.isa;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import com.opencsv.*;
import com.opencsv.exceptions.*;

import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;

//-------------------------------------------------------------------------------------

public class Instruction {
//    private String opcode;
    private Opcode opcode;
    private Operand operand;
    private Value value;

    private Primitive primitive;

//-------------------------------------------------------------------------------------
    public Instruction(Opcode _opcode, Operand _operand, Value _value, Primitive _primitive) {
        opcode = _opcode;
        operand = _operand;
        value = _value;
        primitive = _primitive;
//        int _valueLength = _primitive.getArhCode() - 8;
//        value = Value.getValue(_value, _valueLength);
    }

//-------------------------------------------------------------------------------------
    public boolean resolve() {
        if(opcode.isJump()){
//            int _valueLength = primitive.getArhCode() - 8;
            if(opcode.equals("halt")){
//                value = Value.getValue("0");
            } else {
                int _address = primitive.getByLabel(value.getName());
                value = new Value(_address);                
                if(_address == Integer.MIN_VALUE){
                    return false;
                }
            }
        }
        return true;
    }
    
//-------------------------------------------------------------------------------------
    public int toBin() {
        return Field.toBin(opcode, operand, value);
    }

//-------------------------------------------------------------------------------------
    public String toHex() {
        return Field.toHex(opcode, operand, value);
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
