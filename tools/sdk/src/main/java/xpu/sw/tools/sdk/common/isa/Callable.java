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
    protected Map<String, Integer> labels;
    protected int index;
    protected ArchitectureImplementation architectureImplementation;

//-------------------------------------------------------------------------------------
    public Callable(Context _context, String _architectureId, String _name, Application _application) {
        super(_context);
        architectureId = _architectureId;
        name = _name;
        application = _application;

        instructionLines = new ArrayList<InstructionLine>();
        instructionLinesText = new HashMap<Integer, String>();
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

//-------------------------------------------------------------------------------------
    public void addMacro(Macro _macro) {
        List<InstructionLine> _macroInstructionLines = _macro.getAll();
        for(int i = 0; i < _macroInstructionLines.size(); i++){
            InstructionLine _instructionLine = _macroInstructionLines.get(i);
            addInstruction(_instructionLine, _macro.instructionLinesText.get(i));
        }
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
    public boolean link() {
        for(int i = 0; i < instructionLines.size(); i++){
            instructionLines.get(i).link(context, i);
        }
        return true;
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
