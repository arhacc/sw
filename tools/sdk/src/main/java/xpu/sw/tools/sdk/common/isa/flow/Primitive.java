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

import xpu.sw.tools.sdk.asm.parser.*;


//-------------------------------------------------------------------------------------
public class Primitive extends Callable {

//-------------------------------------------------------------------------------------
    public Primitive(Context _context, String _name, Application _application) {
        super(_context, _name, _application);
    }

//-------------------------------------------------------------------------------------
    public Callable copyOf() {
        Primitive _primitive = new Primitive(context, name, application);
        _primitive.debugInformation = debugInformation;
        return _primitive;
    }

}
//-------------------------------------------------------------------------------------
