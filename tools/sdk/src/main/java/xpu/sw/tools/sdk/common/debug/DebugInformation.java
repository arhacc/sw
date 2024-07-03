//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.debug;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;
import org.apache.lucene.util.*;

import com.esotericsoftware.kryo.kryo5.*;
import com.esotericsoftware.kryo.kryo5.io.*;

import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.xbasics.*;
import xpu.sw.tools.sdk.common.io.*;

import xpu.sw.tools.sdk.common.project.*;

import xpu.sw.tools.sdk.common.isa.flow.*;
import xpu.sw.tools.sdk.common.isa.instruction.*;
import xpu.sw.tools.sdk.common.fileformats.core.*;
import xpu.sw.tools.sdk.common.fileformats.abstractexecutable.*;
import xpu.sw.tools.sdk.common.utils.*;

//-------------------------------------------------------------------------------------
public class DebugInformation extends XBasic {
    private Project project;

    private String name;
    private int lineNo;
    private int programCounter;

    private Map<String, Primitive> primitives;
    private Map<Integer, BreakpointInformation> breakpointInformations;
/*
//-------------------------------------------------------------------------------------
    public DebugInformation(Context _context, XpuFile _xpuFile) {
        this(_context, _xpuFile.getName());
//        debugInformation = new HashMap<CompositeKey, DebugInformation>();
        xpuFile = _xpuFile;
    }
*/

//-------------------------------------------------------------------------------------
    public DebugInformation(Context _context, Project _project) {
        this(_context, _project.getName());
//        debugInformation = new HashMap<CompositeKey, DebugInformation>();
        project = _project;
    }

//-------------------------------------------------------------------------------------
    public DebugInformation(Context _context, String _name) {
        super(_context);
        name = _name;
        lineNo = -1;
        programCounter = 0;
        breakpointInformations = new HashMap<Integer, BreakpointInformation>();
//        log.debug("create DebugInformation: name="+name);
//        new Throwable().printStackTrace();
    }    
/*
//-------------------------------------------------------------------------------------
    public DebugInformation(Context _context, int _lineNo, int _programCounter, Callable _callable) {
        super(_context);
//        debugInformation = null;
        name = null;
        lineNo = _lineNo;
        programCounter = _programCounter;
        callable = _callable;
        breakpointInformations = new HashMap<Integer, BreakpointInformation>();
//        log.debug("create DebugInformation: _lineNo="+_lineNo + ", programCounter="+_programCounter +", _callable="+_callable);
    }
*/
//-------------------------------------------------------------------------------------
    public int getProgramCounter() {
        return programCounter;
    }

//-------------------------------------------------------------------------------------
    public Map<String, Primitive> getPrimitives() {
        return primitives;
    }

//-------------------------------------------------------------------------------------
    public void setPrimitives(Map<String, Primitive> _primitives) {
        primitives = _primitives;
    }

//-------------------------------------------------------------------------------------
    public void toggleBreakpoint(Primitive _primitive, int _programCounter) {
        BreakpointInformation _breakpointInformation = breakpointInformations.get(_programCounter);
        if(_breakpointInformation == null){
            String _functionName = _primitive.getName();
//            int _pc = getPcForLine(_primitive, _lineNo);
            log.debug("Set breakpoint to[" + _functionName+ "] @pc=" + _programCounter +", DebugInformation=" + this);       
            _breakpointInformation = new BreakpointInformation(context, this, _functionName, _programCounter);
            breakpointInformations.put(_programCounter, _breakpointInformation);        
        }
        _breakpointInformation.toggle();
    }

/*
//-------------------------------------------------------------------------------------
    public void add(DebugInformation _debugInformation) {
        if(_debugInformation != null){
            String _name = _debugInformation.getName();
            CompositeKey _key = new CompositeKey(_name);
            debugInformation.put(_key, _debugInformation);
        } else {
            log.warn("Warning: _debugInformation is null!");
        }
    }

//-------------------------------------------------------------------------------------
    public void add(int _lineNo, Callable _callable) {
        DebugInformation _debugInformation = new DebugInformation(context, _lineNo, programCounter, _callable);
        CompositeKey _key = new CompositeKey(null, _lineNo, programCounter);
        debugInformation.put(_key, _debugInformation);
//        instructionsByProgramCounter.put(programCounter, _debugInformationLine);
    }
*/
//-------------------------------------------------------------------------------------
    public String getName() {
        return name;
    }

//-------------------------------------------------------------------------------------
    public List<BreakpointInformation> getBreakpointInformations() {
//        return new ArrayList<BreakpointInformation>(breakpointInformations.values().stream().filter(_b -> _b.isEnabled()).toList());
        return new ArrayList<BreakpointInformation>(breakpointInformations.values());
    }

//-------------------------------------------------------------------------------------
    public BreakpointInformation getBreakpointInformation(int _breakpointId) {
        for (Map.Entry<Integer, BreakpointInformation> _breakpointInformationEntry : breakpointInformations.entrySet()) {
            BreakpointInformation _breakpointInformation = _breakpointInformationEntry.getValue();
            if(_breakpointInformation.getId() == _breakpointId){
                return _breakpointInformation;
            }
        }
        return null;
    }


//-------------------------------------------------------------------------------------
    public int getPcForLine(Primitive _primitive, int _lineNo) {
        Callable _line = _primitive.getLineByIndex(_lineNo);        
        log.debug("DebugInformation.getPcForLine:" + _lineNo + " : " + _line);
        if(_line == null){
            return -1;
        }
        Localization _localization = _line.getLocalization();
        if(_line == null){
            return -1;
        }
        return _localization.getRelativeAddress();
    }

//-------------------------------------------------------------------------------------
    public void refresh(RemoteRunResponse _remoteRunResponse) {
        if(_remoteRunResponse.getCommandCode() == Command.COMMAND_BREAKPOINT_HIT){
            int _breakpointId = _remoteRunResponse.getParameter();
            BreakpointInformation _breakpointInformation = getBreakpointInformation(_breakpointId);
            if(_breakpointInformation != null){
//                lineNo = _breakpointInformation.getLineNo();
//                programCounter = _breakpointInformation.getPc();
                _breakpointInformation.clear();
                programCounter = _breakpointInformation.getPc();
            } else {
                log.warn("DebugInformation: cannot find BreakpointInformation by id:" + _breakpointId);
            }
        }
    }

//-------------------------------------------------------------------------------------
    public String toString() {
        String _text = getName() + " : " + lineNo +  " : " + programCounter + "\n";        
/*        for (Map.Entry<CompositeKey, DebugInformation> _entry : debugInformation.entrySet()) {
            CompositeKey _key = _entry.getKey();
            DebugInformation _value = _entry.getValue();
            _text = _key + " : " + _value.getName() + "\n";
        }        */
        return _text;
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
