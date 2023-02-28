//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.context.arch;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;
import java.time.*;

import org.apache.commons.cli.*;
import org.apache.commons.configuration2.*;
import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.xbasics.*;

//-------------------------------------------------------------------------------------
public class ArchitectureDefinitions extends XBasic {
    private static HashMap<String, Integer> architectureDefinitions = loadArchitectureDefinitions();

    private static final String ARCHITECTURE_DEFINITIONS_PATH = "architecture_definitions.txt";

    static {
        loadArchitectureDefinitions();
    }
//-------------------------------------------------------------------------------------
    public ArchitectureDefinitions(Context _context) {
        super(_context);
//        loadArchitectureDefinitions();
    }

//-------------------------------------------------------------------------------------
    private static HashMap<String, Integer> loadArchitectureDefinitions(){
        HashMap<String, Integer> _architectureDefinitions = new HashMap<String, Integer>();
        ClassLoader _classloader = Thread.currentThread().getContextClassLoader();
        InputStream _inputStream = _classloader.getResourceAsStream(ARCHITECTURE_DEFINITIONS_PATH);
        BufferedReader _reader = new BufferedReader(new InputStreamReader(_inputStream));

        try {
            while(_reader.ready()) {
                String _line = _reader.readLine().replaceAll("\\s+", " ");;
//                System.out.println("_row:" + _line);
                String[] _row = _line.split(" ");
                if(_row.length >= 2){
                    try {
                        _architectureDefinitions.put(_row[0], Integer.parseInt(_row[1]));
                    }catch(NumberFormatException _e){

                    }
                }
            }
        } catch(Throwable _e){
//            log.error
            System.out.println("Cannot load architectureDefinitions from ["+ARCHITECTURE_DEFINITIONS_PATH+"]");
            _e.printStackTrace();
            System.exit(0);
        }
        return _architectureDefinitions;
    }
    
//-------------------------------------------------------------------------------------
    public static int get(String _opcode){
    	Integer _value = architectureDefinitions.get(_opcode);
        if(_value == null){
            System.out.println("Error: Unknown instruction opcode: " + _opcode);
            System.exit(0);
        }
        return _value;
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
