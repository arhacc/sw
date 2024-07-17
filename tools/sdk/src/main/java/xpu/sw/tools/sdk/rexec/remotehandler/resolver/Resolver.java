//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.rexec.remotehandler.resolver;
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

import org.msgpack.core.*;
import org.msgpack.value.*;

import xpu.sw.tools.sdk.common.utils.*;

import xpu.sw.tools.sdk.*;
import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.project.*;
import xpu.sw.tools.sdk.common.xbasics.*;
import xpu.sw.tools.sdk.common.fileformats.core.*;
import xpu.sw.tools.sdk.common.fileformats.obj.*;
import xpu.sw.tools.sdk.common.fileformats.json.*;
import xpu.sw.tools.sdk.common.fileformats.onnx.*;
import xpu.sw.tools.sdk.common.io.*;
import xpu.sw.tools.sdk.common.io.targetmanager.*;
import xpu.sw.tools.sdk.asm.parser.*;

//-------------------------------------------------------------------------------------
public class Resolver extends XBasic {
    private Map<String, Path> repositories;

//-------------------------------------------------------------------------------------
    public Resolver(Context _context) {
        super(_context);
        repositories = new HashMap<String, Path>();
        addLocalLibraries();
    }

//-------------------------------------------------------------------------------------
    private void addLocalLibraries() {
        addLocalLibrary("app_level");
        addLocalLibrary("mid_level");
        addLocalLibrary("low_level");
    }

//-------------------------------------------------------------------------------------
    private void addLocalLibrary(String _name) {
        repositories.put(_name, Paths.get(PathResolver.ARHACC_PATH + PathResolver.separator + "libraries" + PathResolver.separator + _name));
    }

//-------------------------------------------------------------------------------------
    public String resolve(String _graphNodeDescriptor) {
        String[] _graphNodeDescriptorParts = _graphNodeDescriptor.split("[@#-]");

        for (Map.Entry<String, Path> _entry : repositories.entrySet()) {
            String _path = resolve(_entry.getKey(), _entry.getValue(), _graphNodeDescriptorParts);
            if(_path != null){
                return _path;
            }
        }
        return null;
    }

//-------------------------------------------------------------------------------------
    private String resolve(String _repositoryName, Path _repositoryPath, String[] _graphNodeDescriptorParts) {
//        log.debug("Resolve into [" + _repositoryName + "][" + _repositoryPath + "]["+_graphNodeDescriptorParts[0]+"]...");
        File _file = new File(_repositoryPath.toString());
        if(_file.isFile()){
            if(_repositoryPath.getFileName().toString().equals(_graphNodeDescriptorParts[0])){
                return _repositoryPath.toString();
            } else {
                return null;
            }
        }
        File[] _files = new File(_repositoryPath.toString()).listFiles();        
        if(_files == null){
            return null;
        }
        for (File _file0 : _files) {
            if(_file0 != null) {
                String _path = resolve(_repositoryName, Paths.get(_file0.getPath()), _graphNodeDescriptorParts);
                if(_path != null){
                    return _path;
                }
            }
        }
//if we cannot find MessagePack we try to find "filename".data and converted to mp
        if(_graphNodeDescriptorParts[0].endsWith(".mp")){
            return resolveMessagePack(_repositoryName, _repositoryPath, _graphNodeDescriptorParts);
        }
        return null;
    }

//-------------------------------------------------------------------------------------
    private String resolveMessagePack(String _repositoryName, Path _repositoryPath, String[] _graphNodeDescriptorParts) {
        File[] _files = new File(_repositoryPath.toString()).listFiles();        
        if(_files == null){
            return null;
        }
        String _pathData = _graphNodeDescriptorParts[0].substring(0, _graphNodeDescriptorParts[0].length() - "mp".length());
        _pathData = _pathData + "data";
        for (File _file0 : _files) {
            if(_file0.getPath().equals(_pathData)) {
                try {
                    return convertDataToMessagePack(_pathData, _graphNodeDescriptorParts[0]);                    
                }catch(IOException _e){
                    log.error("Cannot convertDataToMessagePack: " + _pathData);
                }
            }
        }
        return null;
    }

//-------------------------------------------------------------------------------------
    private String convertDataToMessagePack(String _pathData, String _pathMessagePack) throws IOException{
        // Create a MesagePacker (encoder) instance
        File _mpFile = new File(_pathMessagePack);
        // Write packed data to a file. No need exists to wrap the file stream with BufferedOutputStream, since MessagePacker has its own buffer
        MessagePacker _packer = MessagePack.newDefaultPacker(new FileOutputStream(_mpFile));
        _packer.packString("type");
        _packer.packString("FLOAT");
        _packer.packString("shape");
        long[] _arr = new long[] {32, 32};
        _packer.packArrayHeader(_arr.length);
        for (long _v : _arr) {
            _packer.packLong(_v);
        }
        
        BufferedReader _reader = new BufferedReader(new FileReader(_pathData));
        String _line = null;
        while((_line = _reader.readLine()) != null){
            String[] _data = _line.split(",");
            for (int i = 0; i < _data.length; i++) {
                _packer.packFloat(Float.parseFloat(_data[i]));
            }
        }
        _packer.close();
        return _pathMessagePack;
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
