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
public class Primitive extends XBasic {
    private String architectureId;
    private String name;

    private List<InstructionLine> instructionLines;
    private Map<String, Integer> labels;
    private int index;
    private ArchitectureImplementation architectureImplementation;

//-------------------------------------------------------------------------------------
    public Primitive(Context _context, String _architectureId, String _name) {
        super(_context);
        architectureId = _architectureId;
        name = _name;

        instructionLines = new ArrayList<InstructionLine>();
        labels = new HashMap<String, Integer>();
        index = 0;
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
    public void addInstruction(InstructionLine _instructionLine) {
//        log.info("Add instruction: " + _instructionLine);
        instructionLines.add(_instructionLine);
        index++;
    }

//-------------------------------------------------------------------------------------
    public void addLabel(String _label) {
//        log.info("Add label [" + _label + "] at address " + index);
        labels.put(_label, index);
    }

//-------------------------------------------------------------------------------------
    public int getByLabel(String _label) {
        Integer _int = labels.get(_label);
        if(_int == null){
            log.error("Cannot find address for label: " + _label);    
//            (new Throwable()).printStackTrace();        
            return Integer.MIN_VALUE;
        }
//        log.error("Reaching label ["+_label+"] at address " + _int);
        return _int;
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
}
//-------------------------------------------------------------------------------------
