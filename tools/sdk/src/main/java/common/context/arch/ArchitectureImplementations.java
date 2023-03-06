//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.context.arch;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;
import java.time.*;

import org.apache.commons.cli.*;
import org.apache.commons.configuration2.*;
import org.apache.logging.log4j.*;

import jakarta.json.*;

import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.xbasics.*;

//-------------------------------------------------------------------------------------
public class ArchitectureImplementations extends XBasic {
    private HashMap<String, ArchitectureImplementation> architectureImplementations;

    private static final String ARCHITECTURE_IMPLEMENTATIONS_PATH = "architecture_implementations.json";

//-------------------------------------------------------------------------------------
    public ArchitectureImplementations(Context _context) {
        super(_context);
        loadArchitectureImplementations();
    }
    
//-------------------------------------------------------------------------------------
    private void loadArchitectureImplementations(){
        architectureImplementations = new HashMap<String, ArchitectureImplementation>();
        try {
            ClassLoader _classloader = Thread.currentThread().getContextClassLoader();
            InputStream _is = _classloader.getResourceAsStream(ARCHITECTURE_IMPLEMENTATIONS_PATH);
//            JsonReader _jsonReader = Json.createReader(new StringReader(_is));
            JsonReader _jsonReader = Json.createReader(_is);
            JsonObject _object = _jsonReader.readObject();
            _object.forEach((_key, _value) -> {
                ArchitectureImplementation _architectureImplementation = 
                new ArchitectureImplementation(context, _key, (JsonObject)_value);     
                architectureImplementations.put(_key, _architectureImplementation);
            });
            _jsonReader.close();
        }catch(Throwable _e){
            log.error("Cannot load architecture implementations from " + ARCHITECTURE_IMPLEMENTATIONS_PATH);
            _e.printStackTrace();
            System.exit(0);
        }
    }

//-------------------------------------------------------------------------------------
    public ArchitectureImplementation getDefault(){
        return getArchitecture(ArchitectureImplementation.DEFAULT_ARCHITECTURE);
    }
    
//-------------------------------------------------------------------------------------
    public ArchitectureImplementation getArchitecture(String _archHash){
        return architectureImplementations.get(_archHash);
    }
    
//-------------------------------------------------------------------------------------
    public List<ArchitectureImplementation> getArchitectures(){
        return new ArrayList<ArchitectureImplementation>(architectureImplementations.values());
    }
    
//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
