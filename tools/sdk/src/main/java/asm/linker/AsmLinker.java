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
import xpu.sw.tools.sdk.common.isa.*;
import xpu.sw.tools.sdk.common.fileformats.hex.*;
import xpu.sw.tools.sdk.common.fileformats.obj.*;
import xpu.sw.tools.sdk.common.fileformats.json.*;

import xpu.sw.tools.sdk.asm.parser.*;

//-------------------------------------------------------------------------------------
public class AsmLinker {
    private Context context;
    private Logger log;
    private ANTLRErrorListener errorListener;

    private String archHash;
    private Application app;
    private Path rootPath;

//-------------------------------------------------------------------------------------
    public AsmLinker(Context _context, ANTLRErrorListener _errorListener) {
        context = _context;
        log = _context.getLog();
        errorListener = (_errorListener == null) ? (new AsmErrorListener()) : _errorListener;

        String _outputFileName;
        /*if (!_commandLine.hasOption("o")) {
            _outputFileName = _commandLine.getOptionValue("o");
        }*/
        String[] _args = _context.getCommandLine().getArgs();
        if(_args.length == 0){
            log.error("Please specify at least one input file!");
            return;
        }
        _outputFileName = _args[0];

//        arhCode = 16; //default architecture
        archHash = _context.getArchitectureImplementations().getDefault().getName();

        app = new Application(log, _outputFileName);
//        app.addFeature((long)(Math.log(arhCode) / Math.log(2)));
        boolean _success = true;
        for (int i = 0; i < _args.length; i++) {
            _success &= loadFirst(_args[i]);
        }
        if(_success){
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
    }

//-------------------------------------------------------------------------------------
    private boolean loadFirst(String _filename) {
        //String _path = (new File(_filename).isAbsolute()) ? "" : ".";
        Path _path = Paths.get(_filename);
        if(_path.toString().endsWith(".asm")){
            rootPath = _path.toAbsolutePath().getParent();
            return load(_path);            
        } else if(_path.toString().endsWith(".prj")){
            rootPath = _path.toAbsolutePath().getParent();
            Project _project = Project.loadFrom(context, _filename);
            Configuration _config = _project.getConfiguration();
            java.util.List<String> _projectFiles = _config.getList(String.class, "files");
            if(_projectFiles != null){
                return _projectFiles.stream()
                .map(Paths::get)
                .map(this::load)
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
    public boolean loadByLinker(String _filename) {
        //String _path = (new File(_filename).isAbsolute()) ? "" : ".";
        Path _path = Paths.get(_filename);
        if(!_path.isAbsolute()){
            _path = rootPath.resolve(_filename);
        }
        return load(_path);
    }

//-------------------------------------------------------------------------------------
    public boolean load(Path _path) {
        log.debug("Loading "+_path.toString()+"...");
        if(_path.toString().endsWith(".asm")){
            CharStream _charStream = null;
            try {
                _charStream = CharStreams.fromFileName(_path.toString());
                AsmLexer _lexer = new AsmLexer(_charStream);
                CommonTokenStream _toks = new CommonTokenStream(_lexer);
                AsmParser _parser = new AsmParser(_toks);
                AsmLinkerListener _listener = new AsmLinkerListener(context, this, app);
                _parser.addParseListener(_listener);
                _parser.removeErrorListeners();
                _parser.addErrorListener(errorListener);
                _parser.parse().enterRule(_listener);
                return _listener.getSuccess();
            } catch(IOException _e0){
                log.debug("Error opening "+_path.toString() + ": " + _e0.getMessage());
    //            System.exit(0);
                return false;
            } catch(Exception _e1){
                log.debug("Error parsing "+_path.toString() + ": " + _e1.getMessage());
    //            System.exit(0);
                return false;
            }
        } else {
            log.debug("Wrong filetype: " + _path.toString());
            return false;
        }
    }

//-------------------------------------------------------------------------------------
    public void setArchHash(String _archHash){
        archHash = _archHash;
    }

//-------------------------------------------------------------------------------------
    public String getArchHash(){
        return archHash;
    }

//-------------------------------------------------------------------------------------
    public void addData(int _address, String _filename){
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
}
//-------------------------------------------------------------------------------------
