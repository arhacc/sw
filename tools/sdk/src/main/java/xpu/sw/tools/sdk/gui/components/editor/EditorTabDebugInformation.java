//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.components.editor;

//-------------------------------------------------------------------------------------
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.nio.charset.*;
import java.nio.file.*;
import java.net.*;
import java.util.*;
import javax.swing.*;
import javax.swing.text.*;
import javax.imageio.*;

import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.debug.*;
import xpu.sw.tools.sdk.common.fileformats.core.*;
import xpu.sw.tools.sdk.common.fileformats.asm.*;
import xpu.sw.tools.sdk.common.fileformats.cpp.*;
import xpu.sw.tools.sdk.common.fileformats.obj.*;
import xpu.sw.tools.sdk.common.fileformats.json.*;
import xpu.sw.tools.sdk.common.fileformats.xpuprj.*;
import xpu.sw.tools.sdk.common.fileformats.abstractexecutable.*;
import xpu.sw.tools.sdk.common.isa.flow.*;
import xpu.sw.tools.sdk.common.isa.instruction.*;
import xpu.sw.tools.sdk.common.utils.*;

import xpu.sw.tools.sdk.gui.*;
import xpu.sw.tools.sdk.gui.components.common.*;

//-------------------------------------------------------------------------------------
public class EditorTabDebugInformation extends GuiBasic {
    private XpuFile xpuFile;

    private DebugInformation debugInformation;
    private Primitive primitive;
    private boolean isEligibleForDebug;

//-------------------------------------------------------------------------------------
    public EditorTabDebugInformation(Gui _gui, Context _context, XpuFile _xpuFile) {
        super(_context, _gui);
        xpuFile = _xpuFile;
        debugInformation = new DebugInformation(_context, _xpuFile);
        refresh();
    }

//-------------------------------------------------------------------------------------
    public XpuFile getXpuFile() {
        return xpuFile;
    }

//-------------------------------------------------------------------------------------
    public DebugInformation getDebugInformation() {
        return debugInformation;
    }

//-------------------------------------------------------------------------------------
    public void toggleBreakpoint(int _lineNo) {
        debugInformation.toggleBreakpoint(_lineNo);
    }

//-------------------------------------------------------------------------------------
    public void refresh() {
        if(xpuFile == null){
            log.warn("Warning: no valid obj file!");
            return;
        }
        String _extension = xpuFile.getExtension();
        if(_extension == null){
            log.warn("Warning: extension is null!");
            return;
        }
        isEligibleForDebug = _extension.equals(AsmFile.EXTENSION) ||
                            _extension.equals(CppFile.EXTENSION);
        String _path = xpuFile.getPath();
        ObjFile _objFile = new ObjFile(log, _path);
        _objFile.load();
        Map<String, Primitive> _primitives = _objFile.getPrimitives();
        if(_primitives == null){
            log.error("Cannot extract primitives info for: " + _path);
            return;
        }/* else {
            DebugInformation _debugInformation  = debugInformation.getDebugInformation(_objFile.getName());
            if(_debugInformation == null){
                log.error("Cannot extract debug[1] info for: " + _objFile.getName() + "\n debugInformation.dump:" + debugInformation);
            } else {
                debugInformation = _debugInformation;
            }
        }*/
        primitive  = _primitives.get(xpuFile.getName());
        if(primitive == null){
            log.error("No primitive named: [" + xpuFile.getName() + "] found in path: " + _path);
        } else {
            debugInformation.setPrimitive(primitive);
        }
    }


//-------------------------------------------------------------------------------------
    public void debugStep() {

    }

//-------------------------------------------------------------------------------------
    public void debugRun() {

    }

//-------------------------------------------------------------------------------------
    public void debugContinue() {
        
    }

//-------------------------------------------------------------------------------------
    public boolean isEligibleForDebug(int _lineNo) {
        String _line = primitive.getLineTextByIndex(_lineNo);
        log.debug("EditorTabDebugInformation.isEligibleForDebug:" + _lineNo + " : " + _line);
        return (_line != null);
    }

//-------------------------------------------------------------------------------------
    public boolean isEligibleForDebug() {
        return isEligibleForDebug;
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
