//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.isa.flow;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;

//import xpu.sw.tools.sdk.common.isa.*;
import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.context.arch.*;
import xpu.sw.tools.sdk.common.xbasics.*;
import xpu.sw.tools.sdk.common.isa.instruction.*;


//-------------------------------------------------------------------------------------
public class Callable extends XBasic {
    protected Callable parent;
    protected Application application;
    protected String name;

    protected List<InstructionLine> instructionLines;
    protected Map<Integer, String> instructionLinesText;
    protected Map<String, Integer> labelsByName;
    protected Map<Integer, Integer> labelsByRelativeAddress;
    protected int index;
    protected int size;

//-------------------------------------------------------------------------------------
    public Callable(Context _context, String _name, Application _application) {
        super(_context);
        name = _name;
        application = _application;

        instructionLines = new ArrayList<InstructionLine>();
        instructionLinesText = new HashMap<Integer, String>();
        labelsByName = new HashMap<String, Integer>();
        labelsByRelativeAddress = new HashMap<Integer, Integer>();
        index = 0;
        size = 0;
    }

//-------------------------------------------------------------------------------------
    public Application getApplication(){
        return application;
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
//        _instructionLine.setCallableParent(this);
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
//        Location _location = new Location(context, index);
        labelsByName.put(_label, index);
        labelsByRelativeAddress.put(index, -1);
    }

//-------------------------------------------------------------------------------------
    public int getByLabel(String _label) {
        Integer _locationIndex = labelsByName.get(_label);
        if((_locationIndex == null) || (_locationIndex == -1)){
            log.error("Cannot find relative address for label[100]: " + _label);    
//            (new Throwable()).printStackTrace();        
            return -1;
        }
        Integer _locationAbsoluteAddress = labelsByRelativeAddress.get(_locationIndex);
        if((_locationAbsoluteAddress == null) || (_locationAbsoluteAddress == -1)){
            log.error("Cannot find absolute address for label[101]: " + _label + ", _locationIndex="+_locationIndex+", callable="+this);    
//            (new Throwable()).printStackTrace();        
            return -1;
        }
//        log.error("Reaching label ["+_label+"] at address " + _int);
//        int _address = _location.getAbsoluteAddress();
//        log.debug("address for label["+_label+"]: " + _locationAbsoluteAddress);    
        return _locationAbsoluteAddress;
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
//        log.debug("Linking callable:" +this);
        int _currentAddress = _startAddress;
        for(int i = 0; i < instructionLines.size(); i++){
            Integer _location = labelsByRelativeAddress.get(i);
            if(_location != null){
//                log.debug(">>> link: i="+i+",_location="+_location + " ==> _currentAddress="+_currentAddress+", callable="+this);
                labelsByRelativeAddress.put(i, _currentAddress);
            }
            _currentAddress = instructionLines.get(i).link(this, _currentAddress);
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
                return _instructionLine.pack(application.getArchitectureImplementation());
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
