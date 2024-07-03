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

//-------------------------------------------------------------------------------------
public class ArchitectureImplementations extends XBasic {
    private HashMap<String, ArchitectureImplementation> architectureImplementations;

    private static final String ARCHITECTURE_IMPLEMENTATIONS_PATH = "architecture_implementations";

//-------------------------------------------------------------------------------------
    public ArchitectureImplementations(Context _context) {
        super(_context);
        loadArchitectureImplementations();
    }
    
//-------------------------------------------------------------------------------------
    private void loadArchitectureImplementations(){
        architectureImplementations = new HashMap<String, ArchitectureImplementation>();
        String _architectureImplementationsPath = context.getPathToSdkHome() + "/etc/architecture_implementations/";
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
                    return _file.isDirectory();
                }
            }));

            _listOfArchitectureImplementations.forEach(_file ->{
                ArchitectureImplementation _architectureImplementation = 
                new ArchitectureImplementation(context, _file);     
                architectureImplementations.put(FilenameUtils.getBaseName(_file.getName()), _architectureImplementation);                
            });

        }catch(Throwable _e){
            log.error("Cannot load architecture implementations from " + _architectureImplementationsPath);
            _e.printStackTrace();
            System.exit(0);
        }
    }

//-------------------------------------------------------------------------------------
    public ArchitectureImplementation getDefault(){
        return getArchitecture(ArchitectureImplementation.DEFAULT_ARCHITECTURE);
    }
    
//-------------------------------------------------------------------------------------
    public ArchitectureImplementation getArchitecture(String _architectureId){
        return architectureImplementations.get(_architectureId);
    }
    
//-------------------------------------------------------------------------------------
    public List<ArchitectureImplementation> getArchitectures(){
        return new ArrayList<ArchitectureImplementation>(architectureImplementations.values());
    }
    
//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
