//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.io;
//-------------------------------------------------------------------------------------
import java.io.*;


//-------------------------------------------------------------------------------------
public class PathResolver {

    public static final String HOME = System.getenv("HOME");
    public static final String XPU_LIBRARIES = importPath(System.getenv("XPU_LIBRARIES_PATH"));
    public static final String separator = File.separator.replace("\\","\\\\");

//-------------------------------------------------------------------------------------
    public static String importPath(String _path) {
        _path = _path.replace("~", HOME);
        return _path;
    }

//-------------------------------------------------------------------------------------
    public static String exportPath(String _path) {
        _path = _path.replace(HOME, "~");
        return _path;
    }

//-------------------------------------------------------------------------------------
    
//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
