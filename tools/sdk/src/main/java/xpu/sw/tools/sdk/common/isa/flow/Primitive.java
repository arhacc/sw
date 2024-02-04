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
    private Map<String, Expression> consts;

//-------------------------------------------------------------------------------------
    public Primitive(Context _context, String _name, Application _application, int _enterFuncLineNoInFile) {
        super(_context, _name, _application, _enterFuncLineNoInFile);
        consts = new HashMap<String, Expression>();
    }

//-------------------------------------------------------------------------------------
    public Callable copyOf() {
        Primitive _primitive = new Primitive(context, name, application, localization.getLineNoInFile());
        _primitive.consts = consts;
        _primitive.debugInformation = debugInformation;
        return _primitive;
    }

//-------------------------------------------------------------------------------------
    public Expression getConst(String _constName) {
        return consts.get(_constName);
    }

//-------------------------------------------------------------------------------------
    public void addConst(String _name, AsmParser.ExpressionContext _expressionContext) {
        consts.put(_name, new Expression(this, _expressionContext));
    }

}
//-------------------------------------------------------------------------------------
