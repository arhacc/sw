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
import xpu.sw.tools.sdk.common.project.*;
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
    public int remoteRun(Project _project, File _file) {
        log.debug("RemoteRun ["+_project+"][" + _file + "]...");
        if(_file == null){
            _file = selectDefaultRunningFileFromProject(_project);
        }
        if(_file == null){
            log.error("No file to be run in project: " + _project);
            return Command.COMMAND_ERROR;
        }
        String _extension = FilenameUtils.getExtension(_file.getPath());
        switch(_extension){
            case HexFile.EXTENSION : {
                return remoteHexRun(_project, _file);
            }
            case JsonFile.EXTENSION: {
                return remoteJsonRun(_project, _file);
            }
            case ObjFile.EXTENSION: {
                return remoteObjRun(_project, _file);
            }
            case OnnxFile.EXTENSION: {
                return remoteOnnxRun(_project, _file);
            }
            default: {
                log.error("Invalid extension to execute: " + _extension);
                return Command.COMMAND_ERROR;
            }
        }
    }

//-------------------------------------------------------------------------------------
    private File selectDefaultRunningFileFromProject(Project _project) {
        //TBD!
        List<String> _files = _project.getConfiguration().getList(String.class, "files");
        if(_files == null){
            return null;
        }
        return null;
    }

//-------------------------------------------------------------------------------------
    private int remoteHexRun(Project _project, File _file) {
//        log.error("Not-implemented!!!: " + _file.getPath());
        HexFile _hexFile = new HexFile(log, _file.getPath());
        _hexFile.load();
        if(!_hexFile.isValid()){
            log.error("Invalid hex file: " + _file.getPath());
            return Command.COMMAND_ERROR;
        }
//        if(connect()){
        send(_hexFile);
        return run(_hexFile.getMainFunctionName());
//            disconnect();            
//        }        
    }

//-------------------------------------------------------------------------------------
    private int remoteJsonRun(Project _project, File _file) {
        JsonFile _jsonFile = new JsonFile(log, _file.getPath());
        _jsonFile.load();
        if(!_jsonFile.isValid()){
            log.error("Invalid json file: " + _file.getPath());
            return Command.COMMAND_ERROR;
        }
//        if(connect()){
        send(_jsonFile);
        return run(_jsonFile.getMainFunctionName());
//            disconnect();            
//        }
    }

//-------------------------------------------------------------------------------------
    private int remoteObjRun(Project _project, File _file) {
        ObjFile _objFile = new ObjFile(log, _file.getPath());
        _objFile.load();
        if(!_objFile.isValid()){
            log.error("Invalid obj file: " + _file.getPath());
            return Command.COMMAND_ERROR;
        }
//        if(connect()){
        send(_objFile);
        return run(_objFile.getMainFunctionName());
//            disconnect();            
//        }
    }

//-------------------------------------------------------------------------------------
    private int remoteOnnxRun(Project _project, File _file) {
        OnnxFile _onnxFile = new OnnxFile(log, _file.getPath());
        _onnxFile.load();
//        if(connect()){
        send(_onnxFile);
        return run(_onnxFile.getMainFunctionName());
//            disconnect();            
//        }
    }
//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
