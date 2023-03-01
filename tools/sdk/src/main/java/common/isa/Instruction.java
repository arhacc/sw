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
    private String name;
    private Opcode opcode;
    private Operand operand;
    private Value value;
    private Primitive primitive;

    private boolean isHalt;

//-------------------------------------------------------------------------------------
    public Instruction(String _name, Opcode _opcode, Operand _operand, Value _value) {
        name = _name;
        opcode = _opcode;
        operand = _operand;
        value = _value;

        isHalt = _name.equals("halt");
//        int _valueLength = _primitive.getArhCode() - 8;
//        value = Value.getValue(_value, _valueLength);
    }

//-------------------------------------------------------------------------------------
    public Instruction clone(){
        return new Instruction(name, opcode, operand, value.clone());            
    }

//-------------------------------------------------------------------------------------
    public boolean isHalt() {
        return isHalt;
    }

//-------------------------------------------------------------------------------------
    public void setPrimitive(Primitive _primitive){
        primitive = _primitive;
    }

//-------------------------------------------------------------------------------------
    public void setValues(String _valueString, int _valueNumber){
        value.set(_valueString, _valueNumber);
    }

//-------------------------------------------------------------------------------------
    public boolean resolve() {
        if(opcode.isJump()){
//            int _valueLength = primitive.getArhCode() - 8;
            if(isHalt()){
//                value = Value.getValue("0");
            } else {
                System.out.println("search for:["+opcode.getName()+"][" + value.getName()+"]");
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
