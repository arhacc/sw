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
import xpu.sw.tools.sdk.common.debug.*;
import xpu.sw.tools.sdk.common.xbasics.*;
import xpu.sw.tools.sdk.common.isa.instruction.*;

//-------------------------------------------------------------------------------------
public abstract class Callable extends XBasic {
    protected Callable parent;
    protected transient Application application;
    protected String name;

    protected List<Callable> lines;
    protected Map<String, Integer> labels;
    protected Localization localization;
    protected int size;

    protected DebugInformation debugInformation;
    
//-------------------------------------------------------------------------------------
    public Callable(Context _context, String _name, Application _application, int _lineNoInFile) {
        super(_context);
        name = _name;
        application = _application;

        lines = new ArrayList<Callable>();
//        linesText = new HashMap<Integer, String>();
        labels = new HashMap<String, Integer>();
        localization = new Localization(_context, this, _lineNoInFile);
//        labelsByRelativeAddress = new HashMap<Integer, Integer>();
        debugInformation = new DebugInformation(_context, _name);
        size = 0;
    }

//-------------------------------------------------------------------------------------
    public abstract Callable copyOf();

//-------------------------------------------------------------------------------------
    public Application getApplication(){
        return application;
    }

//-------------------------------------------------------------------------------------
    public String getName(){
        return name;
    }

//-------------------------------------------------------------------------------------
    public Callable getParent(){
        return parent;
    }

//-------------------------------------------------------------------------------------
    public void setParent(Callable _callable){
        parent = _callable;
    }

//-------------------------------------------------------------------------------------
    public Localization getLocalization(){
        return localization;
    }

//-------------------------------------------------------------------------------------
    public int getLineNoByPc(int _pc){
//        int _firstInstructionLineNo = localization.getLineNoInFile();
//        log.error("Calable.getLineTextAt: name="+getName() + ", _firstInstructionLineNo="+_firstInstructionLineNo + ", _index="+_index);
//        _pc -= _firstInstructionLineNo;
        if(_pc < 0){
            return -1;
        } else if(_pc < lines.size()){
            Callable _callable = lines.get(_pc);
            out("Callable.getLineNoByPc:_pc="+_pc+", Callable=" + _callable);
            if(_callable == null){
                return -1;
            }
            return _callable.getLocalization().getLineNoInFile();
        } else {
            return -1;
        }
    }
    
//-------------------------------------------------------------------------------------
    public String getLineTextByPc(int _pc){
        int _firstInstructionLineNo = localization.getLineNoInFile();
//        log.error("Calable.getLineTextAt: name="+getName() + ", _firstInstructionLineNo="+_firstInstructionLineNo + ", _index="+_index);
//        _pc -= _firstInstructionLineNo;
        if(_pc < 0){
            return null;
        } else if(_pc < lines.size()){
            Callable _callable = lines.get(_pc);
//            log.error("Callable:" + _callable);
            if(_callable == null){
                return null;
            }
            return _callable.getLocalization().getText();
        } else {
            return null;
        }
    }

//-------------------------------------------------------------------------------------
    public String getLineTextByIndex(int _index){
        int _firstInstructionLineNo = localization.getLineNoInFile();
//        log.error("Calable.getLineTextAt: name="+getName() + ", _firstInstructionLineNo="+_firstInstructionLineNo + ", _index="+_index);
        _index -= _firstInstructionLineNo;
        if(_index < 0){
            return null;
        } else if(_index < lines.size()){
            Callable _callable = lines.get(_index);
//            log.error("Callable:" + _callable);
            if(_callable == null){
                return null;
            }
            return _callable.getLocalization().getText();
        } else {
            return null;
        }
    }

//-------------------------------------------------------------------------------------
    public Callable getLineByIndex(int _index){
        int _firstInstructionLineNo = localization.getLineNoInFile();
//        log.error("Calable.getLineTextAt: name="+getName() + ", _firstInstructionLineNo="+_firstInstructionLineNo + ", _index="+_index);
        _index -= _firstInstructionLineNo;
        if(_index < 0){
            return null;
        } else if(_index < lines.size()){
            Callable _callable = lines.get(_index);
//            log.error("Callable:" + _callable);
            if(_callable == null){
                return null;
            }
            return _callable;
        } else {
            return null;
        }
    }

//-------------------------------------------------------------------------------------
    public void addLine(int _lineNo, Callable _line) {
//        log.info("Add instruction: " + _instructionLine + ", _instructionLineText="+_instructionLineText);
//        _instructionLine.setCallableParent(this);
        int _index = lines.size();
        lines.add(_line);
        _line.setParent(this);
        _line.getLocalization().setRelativeAddress(_index);
//        debugInformation.add(_lineNo, _line);
//        linesText.put(index, _instructionLineText);
//        index++;
    }
/*
//-------------------------------------------------------------------------------------
    public void addMacro(Macro _macro) {
        List<InstructionLine> _macroInstructionLines = _macro.getAll();
        for(int i = 0; i < _macroInstructionLines.size(); i++){
            InstructionLine _instructionLine = _macroInstructionLines.get(i);
            addInstruction(_instructionLine, _macro.linesText.get(i));
        }
    }
*/

//-------------------------------------------------------------------------------------
    public void addLabel(String _label) {
//        log.info("Add label [" + _label + "] at address " + index);
//        Location _location = new Location(context, index);
//        Callable _callable = lines.get(lines.size() - 1);
//        _callable.getLocalization().setLabel(_label);
        labels.put(_label, lines.size());

//        labelsByRelativeAddress.put(index, -1);
    }

//-------------------------------------------------------------------------------------
    public int getByLabel(String _label) {
        Integer _relativeAddress = labels.get(_label);
        if((_relativeAddress == null) || (_relativeAddress == -1)){
            log.error("Cannot find relative address for label[100]: " + _label);    
//            (new Throwable()).printStackTrace();        
            return -1;
        }
/*        Integer _locationAbsoluteAddress = labelsByRelativeAddress.get(_locationIndex);
        if((_locationAbsoluteAddress == null) || (_locationAbsoluteAddress == -1)){
            log.error("Cannot find absolute address for label[101]: " + _label + ", _locationIndex="+_locationIndex+", callable="+this);    
//            (new Throwable()).printStackTrace();        
            return -1;
        }*/
//        log.error("Reaching label ["+_label+"] at address " + _int);
//        int _address = _location.getAbsoluteAddress();
//        log.debug("address for label["+_label+"]: " + _locationAbsoluteAddress);    
        Localization _localization = lines.get(_relativeAddress).getLocalization();
        int _absoluteAddress = _localization.getAbsoluteAddress();
        return _absoluteAddress;
    }

//-------------------------------------------------------------------------------------
    public Callable getByIndex(int _index) {
        return lines.get(_index);
    }

//-------------------------------------------------------------------------------------
    public List<Callable> getAll() {
        return lines;
    }
    
//-------------------------------------------------------------------------------------
    public int size() {
        return size;
    }
    
//-------------------------------------------------------------------------------------
    public int link() {
        return link(this, 0);
    }

//-------------------------------------------------------------------------------------
    public int link(Callable _callable, int _absoluteStartAddress) {
//        log.debug("Linking callable:" +this);
        int _absoluteCurrentAddress = _absoluteStartAddress;
        for(int i = 0; i < lines.size(); i++){
            Callable _line = lines.get(i);
            _absoluteCurrentAddress = _line.link(this, _absoluteCurrentAddress);
        }
        size = _absoluteCurrentAddress - _absoluteStartAddress;
        return _absoluteCurrentAddress;
    }

//-------------------------------------------------------------------------------------
    public DebugInformation getDebugInformation(){
        return debugInformation;
    }

//-------------------------------------------------------------------------------------
    public boolean resolve() {
        return lines.stream()
            .map(Callable::resolve)
            .reduce(Boolean.TRUE, Boolean::logicalAnd);
    }
    
//-------------------------------------------------------------------------------------
    public boolean pack() {
/*        if(architectureId.equals(ArchitectureImplementation.DEFAULT_ARCHITECTURE)){
            log.error("Primitive [" + name + "] has no architecture defined(" + ArchitectureImplementation.DEFAULT_ARCHITECTURE + ")");
            System.exit(0);
        }*/
        return lines.stream()
            .map(Callable::pack)
            .reduce(Boolean.TRUE, Boolean::logicalAnd);
    }

//-------------------------------------------------------------------------------------
    public List<Long> toBin() {
        List<Long> _bin = new ArrayList<Long>();
        for(int i = 0; i < lines.size(); i++){
            _bin.addAll(lines.get(i).toBin());
        }
        return _bin;
    }

//-------------------------------------------------------------------------------------
    public List<String> toHex() {
        List<String> _hex = new ArrayList<String>();
//        String _hex = "";
        for(int i = 0; i < lines.size(); i++){
            _hex.addAll(lines.get(i).toHex());
        }
        return _hex;
    }

//-------------------------------------------------------------------------------------
    public void out(String _text){
        System.out.println(_text);
    }
//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
