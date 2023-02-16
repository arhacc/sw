//-------------------------------------------------------------------------------------
package codex.common.utils;
//-------------------------------------------------------------------------------------

import java.net.InetAddress;
import java.net.UnknownHostException;
//-------------------------------------------------------------------------------------

public class SystemUtils {

//-------------------------------------------------------------------------------------
    public static String getUsername() {
        return System.getenv("USER");
    }

//-------------------------------------------------------------------------------------
    public static String getHomeDirectory() {
        String _home = System.getenv("HOME");
        return _home;
    }

//-------------------------------------------------------------------------------------
    public static String getMachinename() {
//    return System.getenv("HOSTNAME");
        try {
            InetAddress address = InetAddress.getLocalHost();
            return address.getHostName();
//            System.out.println("Hostname: " + address.getHostName());
        } catch (UnknownHostException e) {
            return "noname";
        }
    }
//-------------------------------------------------------------------------------------

    public static String getCurrentDirectory() {
        String _path = System.getenv("PWD");
        String[] _dirs = _path.split​("/");
        if (!_dirs[_dirs.length - 1].trim().equals("")) {
            _path = _dirs[_dirs.length - 1];
        } else {
            _path = _dirs[_dirs.length - 2];
        }
        _path = _path.trim();
        return _path;
    }

//-------------------------------------------------------------------------------------
    public static final boolean packageExists(String _packageName) {
        try {
            Class.forName(_packageName);
            return true;
        } catch (Exception e) {
            return false;
        }
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------

