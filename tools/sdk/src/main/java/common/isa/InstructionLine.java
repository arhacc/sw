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
    private Instruction[] instructions;

    private int index;

    private static final int LENGTH = 2;
//-------------------------------------------------------------------------------------
    public InstructionLine() {
        instructions = new Instruction[LENGTH];
        index = 0;
    }

//-------------------------------------------------------------------------------------
    public void add(Instruction _instruction) {
        instructions[index] = _instruction;
        index++;
    }

//-------------------------------------------------------------------------------------
    public boolean resolve() {
        for (int i = 0; i < LENGTH; i++) {
            if(!instructions[i].resolve()){
                return false;
            }
        }
        return true;
    }
    
//-------------------------------------------------------------------------------------
    public boolean pack(ArchitectureImplementation _architectureImplementation) {
        return instructions[0].pack(_architectureImplementation) & 
                instructions[1].pack(_architectureImplementation);
    }

//-------------------------------------------------------------------------------------
    public long toBin() {
        long _dataHi = (long)instructions[0].toBin();
        int _dataLo = instructions[1].toBin();
        return (_dataHi << 32) | _dataLo;
    }

//-------------------------------------------------------------------------------------
    public String toHex() {
        return instructions[0].toHex() + "_" + instructions[1].toHex();
    }

//-------------------------------------------------------------------------------------
    public String toString() {
        return toHex();
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
