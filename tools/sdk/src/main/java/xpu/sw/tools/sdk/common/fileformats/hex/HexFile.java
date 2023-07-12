//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.fileformats.hex;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.isa.flow.*;
import xpu.sw.tools.sdk.common.isa.instruction.*;
import xpu.sw.tools.sdk.common.fileformats.core.*;
import xpu.sw.tools.sdk.common.fileformats.abstractexecutable.*;

//-------------------------------------------------------------------------------------
public class HexFile extends AbstractExecutableFile {
    private Map<Integer, HexLine> lines;
    private int highestAddress;

    public static final String EXTENSION = "hex";

//-------------------------------------------------------------------------------------
    public HexFile(Logger _log, String _path) {
        super(_log, _path, EXTENSION);
        lines = new HashMap<Integer, HexLine>();
        highestAddress = 0;
    }

//-------------------------------------------------------------------------------------
    public HexFile(Logger _log, String _path, Map<String, Primitive> _primitives, List<Data> _datas, List<Long> _features) {
        super(_log, _path, EXTENSION);
        lines = new HashMap<Integer, HexLine>();
        highestAddress = 0;

        int _address = 0;
        for(int i = 0; i < _primitives.size(); i++){
            Primitive _primitive = new ArrayList<Primitive>(_primitives.values()).get(i);
            List<String> _instructionLines = _primitive.toHex();
            for(int j = 0; j < _instructionLines.size(); j++){
                String _instructionLine = _instructionLines.get(j);
                HexLine _hexLine = new HexLine(_instructionLine);
                add(_address, _hexLine);
                _address++;
            }
        }

        for(int i = 0; i < _datas.size(); i++){
            Data _data = _datas.get(i);
            List<DataLine> _dataLines = _data.getAll();
            _address = _data.getAddress();
            log.debug("Write "+_dataLines.size()+" datalines @ " + _address);
            for(int j = 0; j < _dataLines.size(); j++){
                DataLine _dataLine = _dataLines.get(j);
                HexLine _hexLine = new HexLine(_dataLine);
                add(_address, _hexLine);
                _address++;
            }
        }
    }
    
//-------------------------------------------------------------------------------------
    public boolean isValid() {
        return (lines != null) && (lines.size() > 0);
    }

//-------------------------------------------------------------------------------------
    public Map<Integer, HexLine> get() {
        return lines;
    }

//-------------------------------------------------------------------------------------
    public void add(int _address, HexLine _line) {
        lines.put(_address, _line);
        if(_address > highestAddress){
            highestAddress = _address;
        }
    }

//-------------------------------------------------------------------------------------
    public void load() {
        int i = 0;
        log.debug("Load " + path + "...");
        try{
                int _address = 0;
                BufferedReader reader = new BufferedReader(new FileReader(path));

                String _line = reader.readLine();
                do {
                    HexLine _hexline = new HexLine(_line);
                    lines.put(_address, _hexline);
                    _address++;
                    _line = reader.readLine();
                } while (_line != null);
                highestAddress= _address;
                reader.close();
        }catch(IOException _e){
            log.debug("Cannot read hex file: " + path +" @ line " + i);
        }
    }

//-------------------------------------------------------------------------------------
    public int getOpcode(int Instruction) {
        return Instruction >> (3 + 8);
    }

//-------------------------------------------------------------------------------------
    public int getOperand(int Instruction) {
        return (Instruction >> 8) & 0x07;
    }

//-------------------------------------------------------------------------------------
    public void deasm() {
        for (int i = 0; i < highestAddress; i++) {
            HexLine hl = lines.get(i);
            String []Instructions = hl.toString().split(" ");

            //int OpcodeSize = Opcode.ALL_OPERATIONS.get(0).getLength();
            int ControllerInstruction = Integer.parseInt(Instructions[0], 16);
            int ArrayInstruction = Integer.parseInt(Instructions[1], 16);

// Let's not break incapsulation!!!
//If you really need Opcode use a method instead!
/*            
            log.info(
                    Opcode.ALL_OPERATIONS.get(getOpcode(ControllerInstruction)) + " " +
                            Operand.ALL_OPERANDS.get(getOperand(ControllerInstruction)) + "\t\t" +
                    Opcode.ALL_OPERATIONS.get(getOpcode(ArrayInstruction)) + " " +
                            Operand.ALL_OPERANDS.get(getOperand(ArrayInstruction)));
*/                            
        }
    }
/*
func get_matrix
				param        		nop
				nop          		nop
				nop          		cload
				param        		nop
				vsub 1       		vsub 1
				store 0      		addrld
label0:
				vload 8      		nop
				getv          		nop
				load 0        		ioload
				brzdec label1		ristore 1
				store 0      		nop
				jmp label0   		nop
label1:
				halt         		nop
endfunc
*/

//-------------------------------------------------------------------------------------
    public void save() {
        int i = 0;
        log.debug("Save " + path + "...");
        try{
            BufferedWriter writer = new BufferedWriter(new FileWriter(path));
            for(i = 0; i <= highestAddress; i++){
                HexLine _line = lines.get(i);
                if(_line == null){
                    _line = HexLine.EMPTY_HEXLINE;
                }
                writer.write(_line.toString());
            }
            writer.close();
        }catch(IOException _e){
            log.debug("Cannot write hex file: " + path +" @ line " + i);
        }
    }

//-------------------------------------------------------------------------------------
    public String toString() {
        String s = "";
        for(int i = 0; i < highestAddress; i++){
            HexLine _line = lines.get(i);
            if(_line == null){
                _line = HexLine.EMPTY_HEXLINE;
            }
            s += _line.toString() + "\n";
        }
        return s;
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
