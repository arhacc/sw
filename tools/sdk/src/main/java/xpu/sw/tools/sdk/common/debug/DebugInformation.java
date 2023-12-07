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

import xpu.sw.tools.sdk.common.isa.flow.*;
import xpu.sw.tools.sdk.common.isa.instruction.*;
import xpu.sw.tools.sdk.common.fileformats.core.*;
import xpu.sw.tools.sdk.common.fileformats.abstractexecutable.*;
import xpu.sw.tools.sdk.common.utils.*;

//-------------------------------------------------------------------------------------
public class DebugInformation extends XBasic {
//    private Map<CompositeKey, DebugInformation> debugInformation;

    private String name;
    private int lineNo;
    private int programCounter;
    private Callable callable;

//-------------------------------------------------------------------------------------
    public DebugInformation(Context _context, String _name) {
        super(_context);
//        debugInformation = new HashMap<CompositeKey, DebugInformation>();

        name =_name;
        lineNo = -1;
        programCounter = 0;
        callable = null;
        log.debug("create DebugInformation: name="+name);
//        new Throwable().printStackTrace();
    }

//-------------------------------------------------------------------------------------
    public DebugInformation(Context _context, int _lineNo, int _programCounter, Callable _callable) {
        super(_context);
//        debugInformation = null;
        name = null;
        lineNo = _lineNo;
        programCounter = _programCounter;
        callable = _callable;
//        log.debug("create DebugInformation: _lineNo="+_lineNo + ", programCounter="+_programCounter +", _callable="+_callable);
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
/*
//-------------------------------------------------------------------------------------
    public DebugInformation getDebugInformation(String _name) {
        return debugInformation.get(_name);
    }

//-------------------------------------------------------------------------------------
    public DebugInformation getByLineNo(int _lineNo) {
        return debugInformation.get(_lineNo);
    }

//-------------------------------------------------------------------------------------
    public DebugInformation getByProgramCounter(int _programCounter) {
        return debugInformation.get(_programCounter);
    }
*/
//-------------------------------------------------------------------------------------
    public String toString() {
        String _text = name + " : " + lineNo +  " : " + programCounter + "\n";        
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
