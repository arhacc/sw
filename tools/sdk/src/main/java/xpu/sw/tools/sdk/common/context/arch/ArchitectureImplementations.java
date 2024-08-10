//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.context.arch;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.net.*;
import java.time.*;
import java.util.*;
import java.nio.*;
import java.nio.charset.*;
import java.nio.file.*;
import java.util.stream.*;

import org.apache.commons.io.*;
import org.apache.commons.cli.*;
import org.apache.commons.configuration2.*;
import org.apache.logging.log4j.*;

import jakarta.json.*;

import xpu.sw.tools.sdk.*;
import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.context.arch.*;
import xpu.sw.tools.sdk.common.xbasics.*;
import xpu.sw.tools.sdk.common.io.*;

//-------------------------------------------------------------------------------------
public class ArchitectureImplementations extends XBasic {
    private HashMap<String, ArchitectureImplementation> architectureImplementations;
    private String defaultArchitecture;

    private static final String ARCHITECTURE_IMPLEMENTATIONS_PATH = "architecture_implementations";
    private static final String XPU_DEFAULT_FILENAME = "xpu_default";

//-------------------------------------------------------------------------------------
    public ArchitectureImplementations(Context _context) {
        super(_context);
        loadArchitectureImplementations();
    }
    
//-------------------------------------------------------------------------------------
    private void loadArchitectureImplementations(){
        architectureImplementations = new HashMap<String, ArchitectureImplementation>();
        String _architectureImplementationsPath = context.getPathToSdkHome() + PathResolver.separator + "etc" + PathResolver.separator + "architecture_implementations" + PathResolver.separator;
//        log.debug("Load architectureImplementations from: [" + _architectureImplementationsPath + "]...");
        try {
/*            ClassLoader _classloader = Thread.currentThread().getContextClassLoader();
            InputStream _is = _classloader.getResourceAsStream(ARCHITECTURE_IMPLEMENTATIONS_PATH);
//            JsonReader _jsonReader = Json.createReader(new StringReader(_is));
            JsonReader _jsonReader = Json.createReader(_is);
            JsonObject _object = _jsonReader.readObject();
            _object.forEach((_key, _value) -> {
                ArchitectureImplementation _architectureImplementation = 
                new ArchitectureImplementation(context, _key, (JsonObject)_value);     
                architectureImplementations.put(_key, _architectureImplementation);
            });
            _jsonReader.close();*/

            File _path = new File(_architectureImplementationsPath);
            List<File> _listOfArchitectureImplementations = Arrays.asList(_path.listFiles(new FileFilter() {
                public boolean accept(File _file) {
//                    return _filename.toLowerCase().endsWith(".def");
/*
                    boolean isSymbolicLink = Files.isSymbolicLink(_file.toPath());
                    if(isSymbolicLink){
                        try{
                            Path _realPath = Files.readSymbolicLink(_file.toPath());
                            log.debug("0._realPath=" + _realPath);
                            defaultArchitecture = _realPath.getFileName().toString();
                            defaultArchitecture = defaultArchitecture.replaceAll(File.separator, "");
//                            defaultArchitecture = FilenameUtils.getBaseName();
                            log.debug("1._realPath=" + defaultArchitecture);
                        }catch(IOException _e){

                        }
                        return false;
                    }*/

                    if(_file.isFile() && _file.getName().equals(XPU_DEFAULT_FILENAME)){
                        try {
                            defaultArchitecture = FileUtils.readFileToString(_file, StandardCharsets.UTF_8).trim();
                        } catch(IOException _e){

                        }
                    }
                    return _file.isDirectory();
                }
            }));

            if(defaultArchitecture == null){
                log.warn("NO DEFAULT ARCHITECTURE DEFINED!!!");
            }


            _listOfArchitectureImplementations.forEach(_file ->{
                String _name = FilenameUtils.getBaseName(_file.getName());
                boolean _isDefault = (_name.equals(defaultArchitecture));
                ArchitectureImplementation _architectureImplementation = new ArchitectureImplementation(context, _file, _isDefault);
                architectureImplementations.put(_name, _architectureImplementation);
            });

        }catch(Throwable _e){
            log.error("Cannot load architecture implementations from " + _architectureImplementationsPath);
            _e.printStackTrace();
            System.exit(1);
        }
    }

//-------------------------------------------------------------------------------------
    public ArchitectureImplementation getDefault(){
        return getArchitecture(defaultArchitecture);
    }
    
//-------------------------------------------------------------------------------------
    public ArchitectureImplementation getArchitecture(String _architectureId){
        if((_architectureId == null) || _architectureId.equals(ArchitectureImplementation.XPU_ANY)){
            return getDefault();
        }
        return architectureImplementations.get(_architectureId);
    }
    
//-------------------------------------------------------------------------------------
    public List<ArchitectureImplementation> getArchitectures(){
        return new ArrayList<ArchitectureImplementation>(architectureImplementations.values());
    }
    
//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
