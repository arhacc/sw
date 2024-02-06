//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.io;
//-------------------------------------------------------------------------------------
import java.io.*;


//-------------------------------------------------------------------------------------
public class PathResolver {

    public static final String HOME = System.getenv("HOME");
    public static final String ARHACC_PATH = importPathNonRecursive(System.getenv("ARHACC_PATH"));
    public static final String separator = File.separator.replace("\\","\\\\");

//-------------------------------------------------------------------------------------
    public static String importPathNonRecursive(String _path) {
        if(_path == null){
            System.out.println("ARHACC_PATH not defined!\nExiting...");            
            System.exit(1);
            return null;
        }
        _path = _path.replace("~", HOME);
        _path = _path.replace("$HOME", HOME);
        return _path;
    }

//-------------------------------------------------------------------------------------
    public static String importPath(String _path) {
        _path = importPathNonRecursive(_path);
        _path = _path.replace("$ARHACC_PATH", ARHACC_PATH);
        return _path;
    }

//-------------------------------------------------------------------------------------
    public static String exportPath(String _path) {
        _path = _path.replace(ARHACC_PATH, "$ARHACC_PATH");
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
    public static int compare(String _path0, String _path1) {
        return exportPath(_path0).compareTo(exportPath(_path1));
    }

//-------------------------------------------------------------------------------------
    
//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
