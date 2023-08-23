//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.io;
//-------------------------------------------------------------------------------------
import java.io.*;


//-------------------------------------------------------------------------------------
public class PathResolver {

    public static final String HOME = System.getenv("HOME");
    public static final String XPU_LIBRARIES_PATH = System.getenv("XPU_LIBRARIES_PATH");
    public static final String separator = File.separator.replace("\\","\\\\");

//-------------------------------------------------------------------------------------
    public static String importPath(String _path) {
        _path = _path.replace("~", HOME);
        _path = _path.replace("$HOME", HOME);
        _path = _path.replace("$XPU_LIBRARIES_PATH", XPU_LIBRARIES_PATH);
        return _path;
    }

//-------------------------------------------------------------------------------------
    public static String exportPath(String _path) {
        _path = _path.replace(XPU_LIBRARIES_PATH, "$XPU_LIBRARIES_PATH");
        _path = _path.replace(HOME, "$HOME");
        return _path;
    }

//-------------------------------------------------------------------------------------
    public static String add(String _pathBase, String _addition) {
        if(!_pathBase.endsWith(separator) && !_addition.startsWith(separator)){
            _pathBase = _pathBase + separator;
        }
        return _pathBase + _addition;
    }

//-------------------------------------------------------------------------------------
    
//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
