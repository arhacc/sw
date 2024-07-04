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
import xpu.sw.tools.sdk.common.project.*;
import xpu.sw.tools.sdk.common.fileformats.core.*;
import xpu.sw.tools.sdk.common.fileformats.asm.*;
import xpu.sw.tools.sdk.common.fileformats.cpp.*;
import xpu.sw.tools.sdk.common.fileformats.obj.*;
import xpu.sw.tools.sdk.common.fileformats.json.*;
import xpu.sw.tools.sdk.common.fileformats.hex.*;
import xpu.sw.tools.sdk.common.fileformats.xpuprj.*;
import xpu.sw.tools.sdk.common.fileformats.abstractexecutable.*;
import xpu.sw.tools.sdk.common.isa.flow.*;
import xpu.sw.tools.sdk.common.isa.instruction.*;
import xpu.sw.tools.sdk.common.utils.*;

import xpu.sw.tools.sdk.gui.*;
import xpu.sw.tools.sdk.gui.components.common.*;

//-------------------------------------------------------------------------------------
public class EditorTabDebugInformation extends GuiBasic {
    private Project project;
    private XpuFile xpuFile;

    private Primitive primitive;
    private boolean isEligibleForDebug;
    private int initialLineNo;
    private int executionLineNo;

//-------------------------------------------------------------------------------------
    public EditorTabDebugInformation(Gui _gui, Context _context, Project _project, XpuFile _xpuFile) {
        super(_context, _gui);
        project = _project;
        xpuFile = _xpuFile;
        init();
    }

//-------------------------------------------------------------------------------------
    public void init() {
        if(xpuFile == null){
            log.warn("Warning: no valid obj file!");
            return;
        }
        String _extension = xpuFile.getExtension();
        if(_extension == null){
            log.warn("Warning: extension is null!");
            return;
        }
        String _path = xpuFile.getPath();
        isEligibleForDebug = (_extension.equals(AsmFile.EXTENSION) ||
                            _extension.equals(CppFile.EXTENSION) ||
                            _extension.equals(HexFile.EXTENSION)) && !xpuFile.isConfiguration();


        if(!isEligibleForDebug){
            return;
        }
//        String _path = xpuFile.getPath();
        Map<String, Primitive> _primitives = getDebugInformation().getPrimitives();
        if(_primitives == null){
            log.error("Cannot extract primitives info for: " + _path);
            return;
        }
        primitive  = _primitives.get(xpuFile.getName());
        if(primitive == null){
            log.error("No primitive named: [" + xpuFile.getName() + "] found in path: " + _path);
            return;
        }
//        String _extension = xpuFile.getExtension();
        switch (_extension) {
            case AsmFile.EXTENSION: {
                initialLineNo = primitive.getLocalization().getLineNoInFile();
                break;
            }
            case HexFile.EXTENSION: {
                initialLineNo = 0;
                break;
            }
            default: {
                initialLineNo = -1;
                break;
            }
        }
        executionLineNo = -1;
        refresh();
        log.debug("EditorTabDebugInformation: initialLineNo=" + initialLineNo + ", executionLineNo=" + executionLineNo);
    }

//-------------------------------------------------------------------------------------
    public XpuFile getXpuFile() {
        return xpuFile;
    }

//-------------------------------------------------------------------------------------
    public Primitive getPrimitive() {
        return primitive;
    }

//-------------------------------------------------------------------------------------
    public DebugInformation getDebugInformation() {
        return project.getDebugInformation();
    }

//-------------------------------------------------------------------------------------
    public void toggleBreakpoint(int _lineNo) {
        String _extension = xpuFile.getExtension();
        int _programCounter;
        if(_extension == HexFile.EXTENSION){
            _programCounter = _lineNo;
        } else {
            _programCounter = getDebugInformation().getPcForLine(primitive, _lineNo);
        }
        getDebugInformation().toggleBreakpoint(primitive, _programCounter);
    }

//-------------------------------------------------------------------------------------
    public java.util.List<BreakpointInformation> getBreakpointInformations() {
        return getDebugInformation().getBreakpointInformations();
/*        java.util.List<BreakpointInformation> _breakpointInformations = 
            getDebugInformation()
            .getBreakpointInformations()
            .stream()
            .map(_b -> transformBreakpoint(_b)).toList();
            return _breakpointInformations;
    }

//-------------------------------------------------------------------------------------
    private BreakpointInformation transformBreakpoint(BreakpointInformation _b) {
        int _lineNo = _b.getLineNo();
        int _programCounter = _b.getPc();
        return new BreakpointInformation(context, getDebugInformation(), _b.getFunctionName(), _lineNo, _programCounter);
*/        
    }

//-------------------------------------------------------------------------------------
    public int getCurrentExecutionLineNo() {
        return executionLineNo;
    }

//-------------------------------------------------------------------------------------
    public int getLineNoByPc(int _programCounter) {
        if(xpuFile.getExtension() == HexFile.EXTENSION){
            return initialLineNo + _programCounter + 1;
        }
        return getPrimitive().getLineNoByPc(_programCounter);
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
    public void refresh() {
        executionLineNo = initialLineNo + getDebugInformation().getProgramCounter();
//        if(_executionLineNo != executionLineNo){
//            lastExecutionLineNo = executionLineNo;
//            executionLineNo = _executionLineNo;
//        }
    }

//-------------------------------------------------------------------------------------
    public boolean isEligibleForDebug(int _lineNo) {
        refresh();
        if(xpuFile.isConfiguration()){
            return false;
        }
        switch (xpuFile.getExtension()) {
            case AsmFile.EXTENSION: {
                String _line = primitive.getLineTextByIndex(_lineNo);
//                log.debug("EditorTabDebugInformationASM.isEligibleForDebug:" + _lineNo + " : " + _line);
                return (_line != null);
            }
            case HexFile.EXTENSION: {
                String _line = primitive.getLineTextByPc(_lineNo);
                log.debug("EditorTabDebugInformationHEX.isEligibleForDebug:" + _lineNo + " : " + _line);
                return (_line != null);
            }
            default: {
//                log.debug("EditorTabDebugInformation.isEligibleForDebug:" + _lineNo);
                return false;
            }
        }
    }

//-------------------------------------------------------------------------------------
    public boolean isEligibleForDebug() {
        return isEligibleForDebug;
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
