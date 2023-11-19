//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.fileformats.core;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.nio.charset.*;
import java.nio.file.*;
import java.util.*;

import org.apache.commons.io.*;
import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.fileformats.asm.*;
import xpu.sw.tools.sdk.common.fileformats.cpp.*;
import xpu.sw.tools.sdk.common.fileformats.hex.*;
import xpu.sw.tools.sdk.common.fileformats.obj.*;
import xpu.sw.tools.sdk.common.fileformats.json.*;
import xpu.sw.tools.sdk.common.fileformats.onnx.*;

//-------------------------------------------------------------------------------------
public abstract class XpuFile implements Serializable {
    protected transient Logger log;
    protected String path;
    protected String extension;

    protected List<String> lines;
    protected transient File file;
    protected String text;

//-------------------------------------------------------------------------------------
    public XpuFile(Logger _log, String _path, String _extension) {
        log = _log;
        extension = _extension;
        setPath(_path);
        lines = new ArrayList<String>();
        file = new File(_path);
    }

//-------------------------------------------------------------------------------------
    public String getPath() {
        return path;
    }

//-------------------------------------------------------------------------------------
    public void setPath(String _path) {
        if(_path != null){
            int _index = _path.lastIndexOf('.');
            if(_index == -1){
                path = _path;
            } else {
                path = _path.substring(0, _index) + "." + extension;
            }
        }
    }

//-------------------------------------------------------------------------------------
    public String getExtension() {
        return extension;
    }

//-------------------------------------------------------------------------------------
    public static XpuFile loadFrom(Context _context, String _path) {
        String _extension = FilenameUtils.getExtension(_path);
        if(_extension.equals(AsmFile.EXTENSION)){
            return new AsmFile(_context.getLog(), _path);
        } else if(_extension.equals(CppFile.EXTENSION)){
            return new CppFile(_context.getLog(), _path);
        } else if(_extension.equals(HexFile.EXTENSION)){
            return new HexFile(_context.getLog(), _path);
        } else if(_extension.equals(ObjFile.EXTENSION)){
            return new ObjFile(_context.getLog(), _path);
        } else if(_extension.equals(JsonFile.EXTENSION)){
            return new JsonFile(_context.getLog(), _path);
        } else if(_extension.equals(OnnxFile.EXTENSION)){
            return new OnnxFile(_context.getLog(), _path);
        }
        return null;
    }

//-------------------------------------------------------------------------------------
    public String getText(){
        if(text != null){
            return text;
        }
        text = "";
        try{
            lines = Files.readAllLines(file.toPath(), StandardCharsets.ISO_8859_1);
            for(int i = 0; i < lines.size(); i++){
                String _line = lines.get(i) + "\n";
                lines.set(i, _line);
                text += _line;
//                log.debug("->>" + _lines.get(i));
            }
        } catch(IOException _e){
            log.debug("Cannot read: [" + file.toPath() + "]" + _e.getMessage());
            _e.printStackTrace();
        } catch(Exception _e1){
            _e1.printStackTrace();
        }
        return text;
    }

//-------------------------------------------------------------------------------------
    public abstract void load();

//-------------------------------------------------------------------------------------
    public abstract void save();

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
