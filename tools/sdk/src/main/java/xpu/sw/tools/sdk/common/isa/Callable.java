//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.isa;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.isa.*;
import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.context.arch.*;
import xpu.sw.tools.sdk.common.xbasics.*;


//-------------------------------------------------------------------------------------
public class Callable extends XBasic {
    protected Application application;
    protected String architectureId;
    protected String name;

    protected List<InstructionLine> instructionLines;
    protected Map<Integer, String> instructionLinesText;
    protected Map<String, Location> labelsByName;
    protected Map<Integer, Location> labelsByRelativeAddress;
    protected int index;
    protected int size;
    protected ArchitectureImplementation architectureImplementation;

//-------------------------------------------------------------------------------------
    public Callable(Context _context, String _architectureId, String _name, Application _application) {
        super(_context);
        architectureId = _architectureId;
        name = _name;
        application = _application;

        instructionLines = new ArrayList<InstructionLine>();
        instructionLinesText = new HashMap<Integer, String>();
        labelsByName = new HashMap<String, Location>();
        labelsByRelativeAddress = new HashMap<Integer, Location>();
        index = 0;
        size = 0;
        architectureImplementation = _context.getArchitectureImplementations().getArchitecture(_architectureId);
    }

//-------------------------------------------------------------------------------------
    public String getArhCode(){
        return architectureId;
    }

//-------------------------------------------------------------------------------------
    public void setArhCode(String _architectureId){
        architectureId = _architectureId;
    }

//-------------------------------------------------------------------------------------
    public ArchitectureImplementation getArchitectureImplementation(){
        return architectureImplementation;
    }

//-------------------------------------------------------------------------------------
    public String getName(){
        return name;
    }

//-------------------------------------------------------------------------------------
    public String getLineAt(int _index){
        return instructionLinesText.get(_index);
    }
    
//-------------------------------------------------------------------------------------
    public void addInstruction(InstructionLine _instructionLine, String _instructionLineText) {
//        log.info("Add instruction: " + _instructionLine + ", _instructionLineText="+_instructionLineText);
        instructionLines.add(_instructionLine);
        instructionLinesText.put(index, _instructionLineText);
        index++;
    }
/*
//-------------------------------------------------------------------------------------
    public void addMacro(Macro _macro) {
        List<InstructionLine> _macroInstructionLines = _macro.getAll();
        for(int i = 0; i < _macroInstructionLines.size(); i++){
            InstructionLine _instructionLine = _macroInstructionLines.get(i);
            addInstruction(_instructionLine, _macro.instructionLinesText.get(i));
        }
    }
*/

//-------------------------------------------------------------------------------------
    public void addLabel(String _label) {
//        log.info("Add label [" + _label + "] at address " + index);
        Location _location = new Location(context, index);
        labelsByName.put(_label, _location);
        labelsByRelativeAddress.put(index, _location);
    }

//-------------------------------------------------------------------------------------
    public int getByLabel(String _label) {
        Location _location = labelsByName.get(_label);
        if(_location == null){
            log.error("Cannot find relative address for label[100]: " + _label);    
//            (new Throwable()).printStackTrace();        
            return Integer.MIN_VALUE;
        }
        _location = labelsByRelativeAddress.get(_location.getRelativeAddress());
        if(_location == null){
            log.error("Cannot find absolute address for label[101]: " + _label);    
//            (new Throwable()).printStackTrace();        
            return Integer.MIN_VALUE;
        }
//        log.error("Reaching label ["+_label+"] at address " + _int);
        int _address = _location.getAbsoluteAddress();
//        log.debug("address for label: " + _address);    
        return _address;
    }

//-------------------------------------------------------------------------------------
    public InstructionLine getByIndex(int _index) {
        return instructionLines.get(_index);
    }

//-------------------------------------------------------------------------------------
    public List<InstructionLine> getAll() {
        return instructionLines;
    }
    
//-------------------------------------------------------------------------------------
    public int size() {
        return size;
    }
    
//-------------------------------------------------------------------------------------
    public int link() {
        return link(0);
    }

//-------------------------------------------------------------------------------------
    public int link(int _startAddress) {
        int _currentAddress = _startAddress;
        for(int i = 0; i < instructionLines.size(); i++){
            Location _location = labelsByRelativeAddress.get(i);
            if(_location != null){
                _location.setAbsoluteAddress(_currentAddress);
            }
            _currentAddress = instructionLines.get(i).link(_currentAddress);
        }
        size = _currentAddress - _startAddress;
        return _currentAddress;
    }

//-------------------------------------------------------------------------------------
    public boolean resolve() {
        return instructionLines.stream()
            .map(InstructionLine::resolve)
            .reduce(Boolean.TRUE, Boolean::logicalAnd);
    }
    
//-------------------------------------------------------------------------------------
    public boolean pack() {
/*        if(architectureId.equals(ArchitectureImplementation.DEFAULT_ARCHITECTURE)){
            log.error("Primitive [" + name + "] has no architecture defined(" + ArchitectureImplementation.DEFAULT_ARCHITECTURE + ")");
            System.exit(0);
        }*/
        return instructionLines.stream()
            .map(_instructionLine -> {
//                log.debug("_instructionLine=" + _instructionLine);
                return _instructionLine.pack(architectureImplementation);
            })
            .reduce(Boolean.TRUE, Boolean::logicalAnd);
    }

//-------------------------------------------------------------------------------------
    public List<Long> toBin() {
        List<Long> _bin = new ArrayList<Long>();
        for(int i = 0; i < instructionLines.size(); i++){
            _bin.addAll(instructionLines.get(i).toBin());
        }
        return _bin;
    }

//-------------------------------------------------------------------------------------
    public String toHex() {
        String _hex = "";
        for(int i = 0; i < instructionLines.size(); i++){
            _hex += instructionLines.get(i).toHex();
        }
        return _hex;
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
