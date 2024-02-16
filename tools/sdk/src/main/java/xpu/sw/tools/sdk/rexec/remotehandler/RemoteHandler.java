//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.rexec.remotehandler;

//-------------------------------------------------------------------------------------
import java.io.*;
import java.nio.file.*;
import java.util.*;
import java.util.stream.*;

import org.apache.commons.configuration2.*;
import org.apache.logging.log4j.*;

import org.apache.commons.cli.*;
import org.apache.commons.lang3.*;
import org.apache.commons.io.*;
import org.apache.logging.log4j.*;

import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.tree.*;

import xpu.sw.tools.sdk.*;
import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.debug.*;
import xpu.sw.tools.sdk.common.io.*;
import xpu.sw.tools.sdk.common.fileformats.hex.*;
import xpu.sw.tools.sdk.common.fileformats.obj.*;
import xpu.sw.tools.sdk.common.fileformats.json.*;
import xpu.sw.tools.sdk.common.fileformats.onnx.*;
import xpu.sw.tools.sdk.common.io.targetmanager.*;
import xpu.sw.tools.sdk.common.project.*;
//import xpu.sw.tools.sdk.asm.parser.*;

import xpu.sw.tools.sdk.rexec.remotehandler.stack.*;

//-------------------------------------------------------------------------------------
public class RemoteHandler extends ApplicationLayer {
    
    public static final String DEFAULT_RUN_EXTENSION = JsonFile.EXTENSION;

//-------------------------------------------------------------------------------------
    public RemoteHandler(Context _context) {
        this(_context, null);
    }
//-------------------------------------------------------------------------------------
    public RemoteHandler(Context _context, TargetManager _targetManager) {
        super(_context, _targetManager);
    }

//-------------------------------------------------------------------------------------
    public int remoteRun(Project _project, File _file, DebugInformation _debugInformation) {
        log.debug("RemoteRun ["+_project+"][" + _file + "]...");
        if(_file == null){
            log.error("No file to be run in project: " + _project);
            return Command.COMMAND_ERROR;
        }

//                        _rexec.getRemoteHandler().debugRemoveAllBreakpoints();


        String _extension = FilenameUtils.getExtension(_file.getPath());
        String _functionName = "noname";
        switch(_extension){
            case HexFile.EXTENSION : {
                _functionName =  loadHexFile(_project, _file);
                break;
            }
            case JsonFile.EXTENSION: {
                _functionName = loadJsonFile(_project, _file);
                break;
            }
            case ObjFile.EXTENSION: {
                _functionName = loadObjFile(_project, _file);
                break;
            }
            case OnnxFile.EXTENSION: {
                _functionName = loadOnnxFile(_project, _file);
                break;
            }
            default: {
                log.error("Invalid extension to execute: " + _extension);
                return Command.COMMAND_ERROR;
            }
        }

        if(_functionName == null){
            log.error("Invalid functionName to execute!");
            return Command.COMMAND_ERROR;
        }

        if(_debugInformation == null){
            log.debug("No debug informations...");
        } else {
            List<BreakpointInformation> _breakpointInformations = _debugInformation.getBreakpointInformations();
            if(_breakpointInformations.size() == 0){
                log.debug("No breakpoints enabled...");
            } else {
                for(int i = 0; i < _breakpointInformations.size(); i++){
                    BreakpointInformation _breakpointInformation = _breakpointInformations.get(i);
                    if(_breakpointInformation.isEnabled()){
        //                String _functionName = _breakpointInformation.getFunctionName();//editorTabDebugInformation.getXpuFile().getName();
                        int _pc = _breakpointInformation.getPc();
                        log.debug("Set breakpoint to[" + _functionName+ "] @pc=" + _pc);
                        debugAddBreakpoint(_functionName, _pc, 1);
                    }
                }
            }
        }

        return run(_functionName);
    }

//-------------------------------------------------------------------------------------
    private String loadHexFile(Project _project, File _file) {
//        log.error("Not-implemented!!!: " + _file.getPath());
        HexFile _hexFile = new HexFile(log, _file.getPath());
        _hexFile.load();
        if(!_hexFile.isValid()){
            log.error("Invalid hex file: " + _file.getPath());
            return null;
        }
        send(_hexFile);
        return _hexFile.getMainFunctionName();
    }

//-------------------------------------------------------------------------------------
    private String loadJsonFile(Project _project, File _file) {
        JsonFile _jsonFile = new JsonFile(log, _file.getPath());
        _jsonFile.load();
        if(!_jsonFile.isValid()){
            log.error("Invalid json file: " + _file.getPath());
            return null;
        }
        send(_jsonFile);
        return _jsonFile.getMainFunctionName();
    }

//-------------------------------------------------------------------------------------
    private String loadObjFile(Project _project, File _file) {
        ObjFile _objFile = new ObjFile(log, _file.getPath());
        _objFile.load();
        if(!_objFile.isValid()){
            log.error("Invalid obj file: " + _file.getPath());
            return null;
        }
        send(_objFile);
        return _objFile.getMainFunctionName();
    }

//-------------------------------------------------------------------------------------
    private String loadOnnxFile(Project _project, File _file) {
        OnnxFile _onnxFile = new OnnxFile(log, _file.getPath());
        _onnxFile.load();
        send(_onnxFile);
        return _onnxFile.getMainFunctionName();
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
