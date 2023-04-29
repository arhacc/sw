//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.fileformats.cpp;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.nio.file.*;
import java.net.*;
import java.util.*;

import org.apache.commons.configuration2.*;
import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.fileformats.core.*;

//-------------------------------------------------------------------------------------
public class CppFile extends XpuFile {
    private String name;
    public static final String EXTENSION = "cpp";

//-------------------------------------------------------------------------------------
    public CppFile(Logger _log, String _path) {
        super(_log, _path, EXTENSION);
        name = _path;
    }

//-------------------------------------------------------------------------------------
    public CppFile(Logger _log, String _path, String _name) {
        super(_log, _path + _name, EXTENSION);
        name = _name;
    }

//-------------------------------------------------------------------------------------
    public String createTemplateFile() {
        lines.add("//-------------------------------------------------------------------------------------");
        lines.add("//XPU kernel mid level library");
        lines.add("//");
        lines.add("//" + name + " CPP implementation");
        lines.add("//");
        lines.add("//-------------------------------------------------------------------------------------");
        lines.add("//-------------------------------------------------------------------------------------");
        save();
        return getText();
    }

//-------------------------------------------------------------------------------------
    public void load() {
    }

//-------------------------------------------------------------------------------------
    public void save() {
        int i = 0;
        try{
            BufferedWriter writer = new BufferedWriter(new FileWriter(path));
            for(i = 0; i < lines.size(); i++){
                writer.write(lines.get(i) + "\n");
            }
            writer.close();
        }catch(IOException _e){
            log.debug("Cannot write file: " + path +" @ line " + i);
//            return null;
        }
//        return fullPath;
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
