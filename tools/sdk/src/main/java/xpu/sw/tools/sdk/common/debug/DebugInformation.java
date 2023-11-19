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

//-------------------------------------------------------------------------------------
public class DebugInformation extends XBasic {
    private String name;
    private Map<String, DebugInformation> debugInformation;

    private Map<Integer, DebugInformationLine> instructionsByLineNo;
    private Map<Integer, DebugInformationLine> instructionsByProgramCounter;
    private int currentProgramCounter;

//-------------------------------------------------------------------------------------
    public DebugInformation(Context _context, String _name) {
        super(_context);
        name =_name;
        debugInformation = new HashMap<String, DebugInformation>();
        instructionsByLineNo = new HashMap<Integer, DebugInformationLine>();
        instructionsByProgramCounter = new HashMap<Integer, DebugInformationLine>();
        currentProgramCounter = 0;
    }

//-------------------------------------------------------------------------------------
    public void add(DebugInformation _debugInformation) {
        if(_debugInformation != null){
            debugInformation.put(_debugInformation.getName(), _debugInformation);
        } else {
            log.warn("Warning: _debugInformation is null!");
        }
    }

//-------------------------------------------------------------------------------------
    public void add(int _lineNo, Callable _callable) {
        DebugInformationLine _debugInformationLine = new DebugInformationLine(currentProgramCounter, _callable);
        instructionsByLineNo.put(_lineNo, _debugInformationLine);
        instructionsByProgramCounter.put(currentProgramCounter, _debugInformationLine);
    }

//-------------------------------------------------------------------------------------
    public String getName() {
        return name;
    }

//-------------------------------------------------------------------------------------
    public DebugInformationLine getByLineNo(int _lineNo) {
        return instructionsByLineNo.get(_lineNo);
    }

//-------------------------------------------------------------------------------------
    public DebugInformationLine getByProgramCounter(int _programCounter) {
        return instructionsByProgramCounter.get(_programCounter);
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
