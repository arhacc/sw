//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.rexec.remotehandler.resolver;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.nio.file.*;
import java.util.*;
import java.util.stream.*;
import java.security.*;

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
    private MessageDigest md5Digest;
    private Map<String, Path> repositories;

//-------------------------------------------------------------------------------------
    public Resolver(Context _context) {
        super(_context);
        try{
            md5Digest = MessageDigest.getInstance("MD5");            
        } catch(NoSuchAlgorithmException _e){
            log.error("Error: NoSuchAlgorithmException in ApplicationLayer");
            System.exit(1);
        }
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
    public void checkResources(Project _project) {
        for (int i = 0; i < _project.getIO().getNumberOfInputs(); i++) {
            checkResource(_project, i, _project.getIO().getInputName(i), _project.getIO().getInputResourceName(i));
        }
    }

//-------------------------------------------------------------------------------------
    public void checkResource(Project _project, int _index, String _name, String _resourceName) {
        if(_resourceName.contains("#")){
            return;
        }
        String _pathData = _project.getRootPath() + "/" + _name + ".data";
        String _pathMessagePack = _project.getRootPath() + "/" + _name + ".xpu_tensor";
        try {
            convertDataToMessagePack(_pathData, _pathMessagePack);            
        }catch(IOException _e){
            log.error("Cannot convert: ["+_pathData+"] ->[" + _pathMessagePack + "]: " +_e.getMessage());
            System.exit(1);
        }
        byte[] _md5 = null;
        try{
            _md5 = getMD5(_pathMessagePack);
        }catch(IOException _e){
            log.error("Cannot getMD5 of:" + _pathMessagePack);
            System.exit(1);
        }
        String _hash = xpu.sw.tools.sdk.common.utils.StringUtils.bytesToHex(_md5).toLowerCase(); 
        _resourceName = _name + ".xpu_tensor@1.0.0#" + _hash;
        _project.getIO().setInputResourceName(_index, _resourceName);
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
/*
//if we cannot find MessagePack we try to find "filename".data and converted to mp
        if(_graphNodeDescriptorParts[0].endsWith(".xpu_tensor")){
            return resolveMessagePack(_repositoryName, _repositoryPath, _graphNodeDescriptorParts);
        }*/
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
        log.debug("Convert: ["+_pathData+"] ->[" + _pathMessagePack + "]");
        // Create a MesagePacker (encoder) instance
        File _mpFile = new File(_pathMessagePack);
        // Write packed data to a file. No need exists to wrap the file stream with BufferedOutputStream, since MessagePacker has its own buffer
        MessagePacker _packer = MessagePack.newDefaultPacker(new FileOutputStream(_mpFile));
        _packer.packMapHeader(2); 
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
    public byte[] getMD5(String _path) throws IOException {
      //Get file input stream for reading the file content
        File _file = new File(_path);
        FileInputStream _fis = new FileInputStream(_file);
       
      //Create byte array to read data in chunks
      byte[] _byteArray = new byte[1024];
      int _bytesCount = 0; 
        
      //Read file data and update in message digest
      while ((_bytesCount = _fis.read(_byteArray)) != -1) {
        md5Digest.update(_byteArray, 0, _bytesCount);
      };
       
      //close the stream; We don't need it now.
      _fis.close();
       
      //Get the hash's bytes
      return md5Digest.digest();
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
