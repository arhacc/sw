//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.isa.flow;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.context.arch.*;
import xpu.sw.tools.sdk.common.xbasics.*;
import xpu.sw.tools.sdk.common.isa.instruction.*;

//-------------------------------------------------------------------------------------
public class Localization extends XBasic {
    protected Callable callable;

    protected String label;
    protected int lineNoInFile;
    protected int relativeAddress;
    protected int absoluteAddress;
    protected String text;

//-------------------------------------------------------------------------------------
    public Localization(Context _context, Callable _callable) {
        this(_context, _callable, -1, null);
    }

//-------------------------------------------------------------------------------------
    public Localization(Context _context, Callable _callable, int _lineNoInFile) {
        this(_context, _callable, _lineNoInFile, null);
    }

//-------------------------------------------------------------------------------------
    public Localization(Context _context, Callable _callable, int _lineNoInFile, String _text) {
        super(_context);        
        callable = _callable;
        lineNoInFile = _lineNoInFile;
        absoluteAddress = -1;
        text = _text;
    }

//-------------------------------------------------------------------------------------
    public int getLineNoInFile() {
        return lineNoInFile;
    }

//-------------------------------------------------------------------------------------
    public Localization copyOf() {
        Localization _localization = new Localization(context, callable, relativeAddress, text);
        return _localization;
    }

//-------------------------------------------------------------------------------------
    public boolean hasLabel() {
        return (label != null);
    }

//-------------------------------------------------------------------------------------
    public String getLabel() {
        return label;
    }

//-------------------------------------------------------------------------------------
    public void setLabel(String _label) {
        label = _label;
    }

//-------------------------------------------------------------------------------------
    public int getRelativeAddress(){
        return relativeAddress;
    }

//-------------------------------------------------------------------------------------
    public void setRelativeAddress(int _relativeAddress){
        relativeAddress = _relativeAddress;
    }

//-------------------------------------------------------------------------------------
    public int getAbsoluteAddress(){
        return absoluteAddress;
    }

//-------------------------------------------------------------------------------------
    public void setAbsoluteAddress(int _absoluteAddress){
        absoluteAddress = _absoluteAddress;;
    }

//-------------------------------------------------------------------------------------
    public String getText() {
        return text;
    }

//-------------------------------------------------------------------------------------
    public void setText(String _text) {
        text = _text;
    }
//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
