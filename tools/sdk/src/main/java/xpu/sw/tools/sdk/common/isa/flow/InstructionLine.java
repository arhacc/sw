//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.isa.flow;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.context.arch.*;
import xpu.sw.tools.sdk.asm.parser.*;
import xpu.sw.tools.sdk.common.xbasics.*;
import xpu.sw.tools.sdk.common.isa.instruction.*;

//-------------------------------------------------------------------------------------
public class InstructionLine extends Callable {
    private Instruction controlInstruction;
    private Instruction arrayInstruction;

//-------------------------------------------------------------------------------------
    public InstructionLine(Context _context, Application _application) {
        super(_context, "", _application);
    }

//-------------------------------------------------------------------------------------
    public Callable copyOf(){
        InstructionLine _instructionLine = new InstructionLine(context, application);
        _instructionLine.controlInstruction = controlInstruction.copyOf();
        _instructionLine.arrayInstruction = arrayInstruction.copyOf();
        return _instructionLine;
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
    public int link(Callable _parent, int _absoluteStartAddress) {
//        log.debug("Linking InstructionLine:" +this);
        Localization _localization = getLocalization();
        _localization.setAbsoluteAddress(_absoluteStartAddress);
        boolean _success = controlInstruction.link(this) & arrayInstruction.link(this);
//        _context.getLog().error("link address=" + address + ", controlInstruction=" + controlInstruction + ", arrayInstruction="+ arrayInstruction);
        return _absoluteStartAddress + 1;
    }
    
//-------------------------------------------------------------------------------------
    public boolean resolve() {
        return controlInstruction.resolve() & arrayInstruction.resolve();
    }
    
//-------------------------------------------------------------------------------------
    public boolean pack() {
        ArchitectureImplementation _architectureImplementation = application.getArchitectureImplementation();
        return controlInstruction.pack(_architectureImplementation) & 
                arrayInstruction.pack(_architectureImplementation);
    }

//-------------------------------------------------------------------------------------
    public List<Long> toBin() {
        long _dataHi = (long)controlInstruction.toBin();
        int _dataLo = arrayInstruction.toBin();
        List<Long> _bin = new ArrayList<Long>();
        _bin.add((_dataHi << 32) | _dataLo);
        return _bin;
    }

//-------------------------------------------------------------------------------------
    public List<String> toHex() {
//        return address + ":" + controlInstruction.toHex() + " " + arrayInstruction.toHex() + "\n";
        return Arrays.asList(controlInstruction.toHex() + " " + arrayInstruction.toHex() + "\n");
    }

//-------------------------------------------------------------------------------------
    public String toString() {
        return toHex().get(0);
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
