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
public class BreakpointInformation extends XBasic {
    private DebugInformation debugInformation;
    private String functionName;
    private int lineNo;
    private int programCounter;
    private boolean isEnabled;

//-------------------------------------------------------------------------------------
    public BreakpointInformation(Context _context, DebugInformation _debugInformation) {
        this(_context, _debugInformation, "noname", -1, -1);
    }

//-------------------------------------------------------------------------------------
    public BreakpointInformation(Context _context, DebugInformation _debugInformation, String _functionName, int _lineNo, int _programCounter) {
        super(_context);
        debugInformation = _debugInformation;        
        functionName = _functionName;
        lineNo = _lineNo;
        programCounter = _programCounter;
        isEnabled = false;
//        log.debug("create DebugInformation: _lineNo="+_lineNo + ", programCounter="+_programCounter +", _callable="+_callable);
    }

//-------------------------------------------------------------------------------------
    public String getFunctionName() {
        return functionName;
    }

//-------------------------------------------------------------------------------------
    public int getPc() {
        return programCounter;
    }

//-------------------------------------------------------------------------------------
    public boolean isEnabled() {
        return isEnabled;
    }

//-------------------------------------------------------------------------------------
    public boolean toggle() {
        isEnabled = !isEnabled;
        log.debug("Breakpoint ["+functionName+"][pc="+programCounter+"][lineNo="+lineNo+"] is set to: ["+isEnabled+"]...");
        return isEnabled;
    }

//-------------------------------------------------------------------------------------
    public String toString() {
        String _text = functionName + " : " + lineNo +  " : " + programCounter + "\n";        
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
