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

import xpu.sw.tools.sdk.common.isa.flow.*;
import xpu.sw.tools.sdk.common.isa.instruction.*;
import xpu.sw.tools.sdk.common.fileformats.core.*;
import xpu.sw.tools.sdk.common.fileformats.abstractexecutable.*;

//-------------------------------------------------------------------------------------
public class DebugInformationLine implements Serializable {
    private int programCounter;
    private Callable callable;
        
//-------------------------------------------------------------------------------------
    public DebugInformationLine(int _programCounter, Callable _callable) {
        programCounter = _programCounter;
        callable = _callable;
    }

//-------------------------------------------------------------------------------------
    public int getProgramCounter() {
        return programCounter;
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
