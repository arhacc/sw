//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.isa;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.context.arch.*;

//-------------------------------------------------------------------------------------
public class InstructionLine {
    private int address;
    private Instruction controlInstruction;
    private Instruction arrayInstruction;

//-------------------------------------------------------------------------------------
    public InstructionLine() {
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
    public boolean link(int _address) {
        address = _address;
        return controlInstruction.link(this) & 
                arrayInstruction.link(this);
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
        return controlInstruction.toHex() + "_" + arrayInstruction.toHex();
    }

//-------------------------------------------------------------------------------------
    public String toString() {
        return toHex();
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
