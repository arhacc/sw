//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.rexec;

//-------------------------------------------------------------------------------------
import java.io.*;
import java.nio.file.*;
import java.util.*;
import java.util.stream.*;

import org.apache.commons.cli.*;
import org.apache.commons.lang3.*;
import org.apache.commons.io.*;
import org.apache.logging.log4j.*;

import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.tree.*;

import xpu.sw.tools.sdk.*;
import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.fileformats.hex.*;
import xpu.sw.tools.sdk.common.fileformats.obj.*;
import xpu.sw.tools.sdk.common.fileformats.onnx.*;
import xpu.sw.tools.sdk.common.io.targetmanager.*;
import xpu.sw.tools.sdk.rexec.remotehandler.*;

//-------------------------------------------------------------------------------------
public class Rexec {
    private Sdk sdk;
    private Logger log;
    private Context context;
    private RemoteHandler remoteHandler;
    private Path inputDirectory;

//-------------------------------------------------------------------------------------
    public Rexec(Context _context) {
        this(_context, true, null);
    }

//-------------------------------------------------------------------------------------
    public Rexec(Context _context, boolean _autoLoadAndRun, TargetManager _targetManager) {
        sdk = _context.getSdk();
        log = _context.getLog();
        context = _context;
        log.debug("Starting XPU Rexec " + _context.getVersion() + "...");
//        String[] _args = _cmd.getArgs();
//        CommandLine _commandLine = _context.getCommandLine();
        remoteHandler = new RemoteHandler(_context, _targetManager);
//        uploader.upload();
//        System.exit(0);
        if(_autoLoadAndRun){
            loadAndRunFromCurrentDirectory();
        }
    }

//-------------------------------------------------------------------------------------
    public void remoteRun(String _path) {
        remoteHandler.remoteRun(_path);
    }

//-------------------------------------------------------------------------------------
    public boolean isRunnable(String _path) {
        String _extension = FilenameUtils.getExtension(_path);
        if(_extension.equals(HexFile.EXTENSION)){
            return true;
        } else if(_extension.equals(ObjFile.EXTENSION)){
            return true;
        } else if(_extension.equals(OnnxFile.EXTENSION)){
            return true;
        } else {
            return false;            
        }
    }

//-------------------------------------------------------------------------------------
    private void loadAndRunFromCurrentDirectory(){

        String _inputDirectory;
        /*if (!_commandLine.hasOption("o")) {
            _outputFileName = _commandLine.getOptionValue("o");
        }*/
        String[] _args = context.getCommandLine().getArgs();
        if(_args.length == 0){
//            log.error("Please specify directory to upload");
            _inputDirectory = ".";
//            return;
        } else {
            _inputDirectory = _args[0];
        }

        inputDirectory = Path.of(_inputDirectory);
        try {
//            log.debug("inputDirectory=" + inputDirectory);
            List<String> _files = findFiles(inputDirectory, "obj");
//            log.debug("_files.size=" + _files.size());
            if(_files.size() == 0){
                log.error("No obj files found in directory: " + inputDirectory);
                return;
            } else if(_files.size() > 1){
                log.error("Multiple obj files detected in directory: " + inputDirectory);                
                return;
            }
            remoteRun(_files.get(0));
        } catch (Exception _e) {
            log.error("Exception:" + _e.getMessage());
            _e.printStackTrace();
        }
    }

//-------------------------------------------------------------------------------------
    public List<String> findFiles(Path _path, String _fileExtension) throws IOException {

/*        if (!Files.isDirectory(path)) {
            throw new IllegalArgumentException("Path must be a directory!");
        }*/
        
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
        /*
        List<String> _result;

        Project _project = Project.loadFrom(context, _path.toString());
        Configuration _config = _project.getConfiguration();
        java.util.List<String> _projectFiles = _config.getList(String.class, "files");
        if(_projectFiles != null){
            try (Stream<String> _objFiles = _projectFiles.stream()) {
                _result = _objFiles
                        .filter(p -> !Files.isDirectory(Path.of(p)))
                        // this is a path, not string,
                        // this only test if path end with a certain path
                        //.filter(p -> p.endsWith(fileExtension))
                        // convert path to string first
                        .map(p -> p.toLowerCase())
                        .filter(f -> f.endsWith(_fileExtension))
                        .collect(Collectors.toList());
            }
        } else {
//            log.debug("Error: Project ["+_filename+"] has no files");
            return null;
        }
        return _result;*/
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
                    .map(p -> p.toString())
                    .filter(f -> f.endsWith(_fileExtension))
                    .collect(Collectors.toList());
        }

        return _result;
    }

//-------------------------------------------------------------------------------------

}
//-------------------------------------------------------------------------------------
