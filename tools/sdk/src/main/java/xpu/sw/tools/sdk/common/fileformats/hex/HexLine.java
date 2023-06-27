//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.fileformats.hex;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.isa.flow.*;
import xpu.sw.tools.sdk.common.isa.instruction.*;

//-------------------------------------------------------------------------------------
public class HexLine {
    private String line;

    public static final HexLine EMPTY_HEXLINE = new HexLine("00000000_00000000");

//-------------------------------------------------------------------------------------
    public HexLine() {
    }

//-------------------------------------------------------------------------------------
    public HexLine(InstructionLine _instructionLine) {
        this(_instructionLine.toHex());
    }

//-------------------------------------------------------------------------------------
    public HexLine(DataLine _dataLine) {
        this(_dataLine.toHex());
    }

//-------------------------------------------------------------------------------------
    public HexLine(String _line) {
        line = _line;
    }

//-------------------------------------------------------------------------------------
    public void add(String _line) {
        if(line == null){
            line = _line;
        } else {
            line += _line;
        }
    }

//-------------------------------------------------------------------------------------
    public String toString() {
        return line;
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
