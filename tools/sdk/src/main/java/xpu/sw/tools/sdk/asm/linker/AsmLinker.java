//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.asm.linker;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.nio.file.*;
import java.util.*;

import org.apache.commons.configuration2.*;
import org.apache.logging.log4j.*;

import org.apache.commons.cli.*;
import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;

import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.tree.*;

import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.project.*;
import xpu.sw.tools.sdk.common.isa.flow.*;
import xpu.sw.tools.sdk.common.isa.instruction.*;
import xpu.sw.tools.sdk.common.fileformats.hex.*;
import xpu.sw.tools.sdk.common.fileformats.obj.*;
import xpu.sw.tools.sdk.common.fileformats.json.*;

import xpu.sw.tools.sdk.asm.parser.*;

//-------------------------------------------------------------------------------------
public class AsmLinker {
    private Context context;
    private Logger log;
    private ANTLRErrorListener errorListener;

    private String architectureId;
    private Application app;
    private Path rootPath;

//    private AsmParser parser;
    private AsmLinkerListener listener;
    private int numberOfLinkErrors;

//-------------------------------------------------------------------------------------
    public AsmLinker(Context _context, ANTLRErrorListener _errorListener) {
        context = _context;
        log = _context.getLog();
        errorListener = (_errorListener == null) ? (new AsmErrorListener()) : _errorListener;
        numberOfLinkErrors = 0;
        load(_context.getCommandLine().getArgs());
}

//-------------------------------------------------------------------------------------
    public int getNumberOfErrors(){
        return getNumberOfLinkErrors();
    }

//-------------------------------------------------------------------------------------
    public int getNumberOfLinkErrors(){
        return numberOfLinkErrors;
    }

//-------------------------------------------------------------------------------------
    public void incNumberOfLinkErrors(){
        numberOfLinkErrors++;
    }

//-------------------------------------------------------------------------------------
    private boolean load(String[] _args){
        if(_args.length == 0){
            log.error("Please specify at least one input file!");
            return false;
        }
        boolean _success = true;
        for (int i = 0; i < _args.length; i++) {
            _success &= load(_args[i]);
        }
        return _success;
}

//-------------------------------------------------------------------------------------
    private boolean load(String _path){
        if(Files.isDirectory(Paths.get(_path))){
            return loadDirectory(_path);
        } else {
            return loadFile(_path);
        }
    }

//-------------------------------------------------------------------------------------
    private boolean loadDirectory(String _path){
        List<File> _listOfAsmFiles = Arrays.asList(new File(_path).listFiles(new FilenameFilter() {
            public boolean accept(File _dirFiles, String _filename) {
                return _filename.toLowerCase().endsWith(".asm");
            }
        }));
        boolean _success = true;
        for(int i = 0; i < _listOfAsmFiles.size(); i++){
            _success &= loadFile(_listOfAsmFiles.get(i).getAbsolutePath());
        }
        return _success;
    }

//-------------------------------------------------------------------------------------
    private boolean loadFile(String _path){

//        architectureId = 16; //default architecture
//        architectureId = _context.getArchitectureImplementations().getDefault().getName();

//        log.error("Load File: " + _path);
        boolean _success = true;
        try {
            app = new Application(context, _path);
    //        app.addFeature((long)(Math.log(architectureId) / Math.log(2)));
            _success = loadTop(_path, app);
            if(_success){
                if(app.link()){
                    if(app.resolve()){
                        if(app.pack()){
                            HexFile _hex = app.exportHexFile();
                            _hex.save();

                            ObjFile _obj = app.exportObjFile();
                            _obj.save();

                            JsonFile _json = app.exportJsonFile();
                            _json.save();
                        }
                    }
                }
            } else {
                log.error("Error in file: " + _path);
                _success = false;
            }
        }catch(Throwable _t){
            log.error("Error: " + _t.getMessage());
            _t.printStackTrace();
            _success = false;
        }
        return _success;
    }

//-------------------------------------------------------------------------------------
    private boolean loadTop(String _filename, Application app) {
        //String _path = (new File(_filename).isAbsolute()) ? "" : ".";
        Path _path = Paths.get(_filename);
        if(_path.toString().endsWith(".asm")){
            rootPath = _path.toAbsolutePath().getParent();
            return load(_path, app);            
        } else if(_path.toString().endsWith(".xpuprj")){
            rootPath = _path.toAbsolutePath().getParent();
            Project _project = Project.loadFrom(context, _filename);
            Configuration _config = _project.getConfiguration();
            java.util.List<String> _projectFiles = _config.getList(String.class, "files");
            if(_projectFiles != null){
                return _projectFiles.stream()
                .map(Paths::get)
//                .map(this::load)
                .map(_path1 -> load(Paths.get(_project.getRootPath(), _path1.toString()), app))
                .reduce(Boolean.TRUE, Boolean::logicalAnd);
            } else {
                log.debug("Error: Project ["+_filename+"] has no asm files");
                return false;
            }
        } else {
            log.debug("Error: Cannot open ["+_path.toString() + "]: unknown file type");
            return false;
        }
    }

//-------------------------------------------------------------------------------------
    public boolean loadByLinker(String _filename, Application app) {
        //String _path = (new File(_filename).isAbsolute()) ? "" : ".";
        _filename.replace("/", File.separator);
        Path _path = Paths.get(_filename);
        if(!_path.isAbsolute()){
            _path = rootPath.resolve(_filename);
        }
        return load(_path, app);
    }

//-------------------------------------------------------------------------------------
    public boolean load(Path _path, Application app) {
        log.debug("Loading "+_path.toString()+"...");
        if(_path.toString().endsWith(".asm")){
            CharStream _charStream = null;
            try {
                _charStream = CharStreams.fromFileName(_path.toString());
                AsmLexer _lexer = new AsmLexer(_charStream);
                CommonTokenStream _toks = new CommonTokenStream(_lexer);
                AsmParser _parser = new AsmParser(_toks);
                listener = new AsmLinkerListener(context, this, app);
//                parser.setErrorHandler(new DefaultErrorStrategy());
                _parser.addParseListener(listener);
                _parser.removeErrorListeners();
                _parser.addErrorListener(errorListener);
                _parser.parse().enterRule(listener);
//                log.debug("listener.getSuccess()=" + getNumberOfLinkErrors());
//                log.debug("parser.getNumberOfSyntaxErrors()=" + _parser.getNumberOfSyntaxErrors() +", getNumberOfLinkErrors()=" + getNumberOfLinkErrors());
//                return getSuccess();
                numberOfLinkErrors += _parser.getNumberOfSyntaxErrors();
                return (getNumberOfLinkErrors() == 0);
            } catch(IOException _e0){
                log.debug("Error opening "+_path.toString() + ": " + _e0.getMessage());
    //            System.exit(0);
                return false;
            } catch(Exception _e1){
                log.debug("Error parsing "+_path.toString() + ": " + _e1.getMessage());
//                _e1.printStackTrace();
    //            System.exit(0);
                return false;
            }
        } else {
            log.debug("Wrong filetype: " + _path.toString());
            return false;
        }
    }
/*
//-------------------------------------------------------------------------------------
    public boolean getSuccess(){
        return (parser.getNumberOfSyntaxErrors() == 0) & (getNumberOfLinkErrors() == 0);
    }
*/
/*
//-------------------------------------------------------------------------------------
    public String getArchitectureId(){
        return architectureId;
    }

//-------------------------------------------------------------------------------------
    public void setArchitectureId(String _architectureId){
        architectureId = _architectureId;
    }
*/
//-------------------------------------------------------------------------------------
    public void addData(int _address, String _filename, Application app){
        log.debug("AsmLinker.addData:" + _filename);
        _filename = _filename.replace("\"","");
        _filename = _filename.replace("\'","");
        Path _path = Paths.get(_filename);
        if(!_path.isAbsolute()){
            _path = rootPath.resolve(_filename);
        }
        app.addData(_address, _path);
    }

//-------------------------------------------------------------------------------------
    public String getLineTextByPc(int _pc){
        return app.getLineTextByPc(_pc);
    }
    
//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
