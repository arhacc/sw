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
    private Instruction controlInstruction;
    private Instruction arrayInstruction;

//-------------------------------------------------------------------------------------
    public InstructionLine() {
    }

//-------------------------------------------------------------------------------------
    public void setControlInstruction(Instruction _instruction) {
        controlInstruction = _instruction;
    }

//-------------------------------------------------------------------------------------
    public void setArrayInstruction(Instruction _instruction) {
        arrayInstruction = _instruction;
    }

//-------------------------------------------------------------------------------------
    public boolean resolve() {
        boolean _success = controlInstruction.resolve();
        if(!_success){
            return false;
        }
        return arrayInstruction.resolve();
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
