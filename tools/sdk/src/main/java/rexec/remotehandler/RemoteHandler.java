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
import xpu.sw.tools.sdk.common.fileformats.hex.*;
import xpu.sw.tools.sdk.common.fileformats.obj.*;
import xpu.sw.tools.sdk.common.fileformats.onnx.*;
import xpu.sw.tools.sdk.common.io.targetmanager.*;
import xpu.sw.tools.sdk.asm.parser.*;
import xpu.sw.tools.sdk.rexec.remotehandler.stack.*;

//-------------------------------------------------------------------------------------
public class RemoteHandler extends ApplicationLayer {

//-------------------------------------------------------------------------------------
    public RemoteHandler(Context _context) {
        this(_context, null);
    }
//-------------------------------------------------------------------------------------
    public RemoteHandler(Context _context, TargetManager _targetManager) {
        super(_context, _targetManager);
    }

//-------------------------------------------------------------------------------------
    public void remoteRun(String _filePath) {
        log.debug("RemoteRun ["+_filePath+"]...");
        String _extension = FilenameUtils.getExtension(_filePath);
        if(_extension.equals(HexFile.EXTENSION)){
            remoteHexRun(_filePath);
        } else if(_extension.equals(ObjFile.EXTENSION)){
            remoteObjRun(_filePath);
        } else if(_extension.equals(OnnxFile.EXTENSION)){
            remoteOnnxRun(_filePath);
        }
    }

//-------------------------------------------------------------------------------------
    private void remoteHexRun(String _filePath) {
        log.error("Not-implemented!!!: " + _filePath);
    }

//-------------------------------------------------------------------------------------
    private void remoteObjRun(String _filePath) {
        ObjFile _objFile = new ObjFile(log, _filePath);
        _objFile.load();
        if(!_objFile.isValid()){
            log.error("Invalid obj file: " + _filePath);
            return;
        }
//        if(connect()){
            send(_objFile);
//            disconnect();            
//        }
    }

//-------------------------------------------------------------------------------------
    private void remoteOnnxRun(String _filePath) {
        OnnxFile _onnxFile = new OnnxFile(log, _filePath);
        _onnxFile.load();
//        if(connect()){
            send(_onnxFile);
//            disconnect();            
//        }
    }
//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
