//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.components.debugger.stack;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.nio.file.*;
import java.util.*;
import java.util.stream.*;

import org.apache.commons.configuration2.*;
import org.apache.logging.log4j.*;

import org.apache.commons.cli.*;
import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;

import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.tree.*;

import xpu.sw.tools.sdk.*;
import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.project.*;
import xpu.sw.tools.sdk.common.fileformats.hex.*;
import xpu.sw.tools.sdk.common.fileformats.obj.*;
import xpu.sw.tools.sdk.gui.*;
import xpu.sw.tools.sdk.asm.parser.*;

//-------------------------------------------------------------------------------------
public class DebugLayer extends ApplicationLayer {
    private Gui gui;
    private Project currentProject;

//-------------------------------------------------------------------------------------
    public DebugLayer(Gui _gui, Context _context, Project _currentProject) {
        super(_context);
        currentProject = _currentProject;
        targetConnection = _gui.getServices().getTargetManager().getTargetConnection();
        load();
    }

//-------------------------------------------------------------------------------------
    protected void send(ObjFile _objFile) {
/*        while(status != STATUS_RUNNING){
            try{
                sleep(1000);
            }catch(InterruptedException _e){

            }
        }*/
        sendCode(_objFile.getCodeSegment());
        sendData(_objFile.getDataSegment());
    }

//-------------------------------------------------------------------------------------
    protected void disconnect() {
        closeConnection();
    }

//-------------------------------------------------------------------------------------
    public Project getProject() {
        return currentProject;
    }

//-------------------------------------------------------------------------------------
    public void load() {

        Path _path = currentProject.getRootFile().toPath();
        try {
//            log.debug("inputDirectory=" + inputDirectory);
            List<String> _files = findFiles(_path, "obj");
//            log.debug("_files.size=" + _files.size());
            if(_files.size() == 0){
                log.error("No obj files found in directory: " + _path);
                return;
            } else if(_files.size() > 1){
                log.error("Multiple obj files detected in directory: " + _path);                
                return;
            }
            debug(_files.get(0));
        } catch (Exception _e) {
            log.error("Exception:" + _e.getMessage());
            _e.printStackTrace();
        }      
    }

//-------------------------------------------------------------------------------------
    public List<String> findFiles(Path _path, String _fileExtension) throws IOException {
        
//        System.out.println(">>>" + path.toString());
        if(_path.toString().endsWith(".prj")){
            return findFilesInProject(_path, _fileExtension);
        } else if (!Files.isDirectory(_path)) {
            throw new IllegalArgumentException("Path must be a directory:" + _path);
        } else {
            return findFilesInDirectory(_path, _fileExtension);
        }
    }

//-------------------------------------------------------------------------------------
    public List<String> findFilesInProject(Path _path, String _fileExtension) throws IOException {
        _path = _path.getParent();
        return findFilesInDirectory(_path, _fileExtension);

    }

//-------------------------------------------------------------------------------------
    public List<String> findFilesInDirectory(Path _path, String _fileExtension) throws IOException {
        List<String> _result;

        try (Stream<Path> _walk = Files.walk(_path)) {
            _result = _walk
                    .filter(p -> !Files.isDirectory(p))
                    // this is a path, not string,
                    // this only test if path end with a certain path
                    //.filter(p -> p.endsWith(fileExtension))
                    // convert path to string first
                    .map(p -> p.toString().toLowerCase())
                    .filter(f -> f.endsWith(_fileExtension))
                    .collect(Collectors.toList());
        }

        return _result;
    }

//-------------------------------------------------------------------------------------
    public void debug(String _filePath) {
        log.debug("Debuging " + _filePath + "...");
        ObjFile _obj = new ObjFile(log, _filePath, ObjFile.EXTENSION);
        _obj.load();
        if(!_obj.isValid()){
            log.error("Invalid obj file: " + _filePath);
            return;
        }
//        if(connect()){
            send(_obj);
//            disconnect();            
//        }
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
