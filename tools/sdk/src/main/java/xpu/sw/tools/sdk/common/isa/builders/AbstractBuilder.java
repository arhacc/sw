//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.isa.builders;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import com.opencsv.*;
import com.opencsv.exceptions.*;

import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.isa.*;
import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.context.arch.*;
import xpu.sw.tools.sdk.common.xbasics.*;

//-------------------------------------------------------------------------------------
public class AbstractBuilder extends XBasic {
    protected String architectureId;

    protected ArchitectureImplementation architectureImplementation;
    protected int dataSize;

//-------------------------------------------------------------------------------------
    public AbstractBuilder(Context _context) {
        this(_context, null);
    }

//-------------------------------------------------------------------------------------
    public AbstractBuilder(Context _context, String _architectureId) {
        super(_context);
        if(_architectureId != null){
            architectureId = _architectureId;
            architectureImplementation = context.getArchitectureImplementations().getArchitecture(_architectureId);
            dataSize = architectureImplementation.get("DATA_SIZE");
        }
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
