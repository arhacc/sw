//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.wrappers.version;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import org.apache.commons.configuration2.*;
import org.apache.logging.log4j.*;

//-------------------------------------------------------------------------------------
public class Version {
    private Logger log;
    private String[] items;

    private String[] versions;
    private int[][] versionsArray;

//-------------------------------------------------------------------------------------
    public Version(Logger _log, String _item){
        this(_log, new String[]{_item});
    }

// _items must be filled in an inclusion aware order
//Example: _items = new String[2]{"dao-libs-", "dao-"};
//-------------------------------------------------------------------------------------
    public Version(Logger _log, String[] _items){
        log = _log;
        items = _items;

        String _cp = System.getProperty("java.class.path");
        String[] _classpathEntries = _cp.split(File.pathSeparator);
        versions = new String[3];
        for (String _p: _classpathEntries) {
//            _log.info(">>>>" + _p);
            scanClasspathEntry(_p);
        }
        extractVersionsArray();
    }

//-------------------------------------------------------------------------------------
    public void scanClasspathEntry(String _p){
        for (int i = 0; i < items.length; i++) {
            String _item = items[i];
//            System.out.println(_p);
            String[] _files = _p.split(File.separator);
            String _filename = _files[_files.length - 1];
//            log.info(":--->" + _filename);
            if(_filename.startsWith(_item)){
                versions[i] = _filename.replace(_item, "").replace(".jar", "");
                break;
            }
        }
//        extractVersionsArray();
    }

//-------------------------------------------------------------------------------------
    public String getVersion(){
        return getVersion(0);
    }

//-------------------------------------------------------------------------------------
    public String getVersion(int _index){
        return versions[_index];
    }

//-------------------------------------------------------------------------------------
    public String getVersion(String _key){
        for(int i = 0; i < items.length; i++){
            if(items[i].equals(_key)){
                return getVersion(i);
            }
        }
        return null;
    }

//-------------------------------------------------------------------------------------
    public String[] getItems(){
        return items;
    }

//-------------------------------------------------------------------------------------
    public int compareTo(Version _version){
        for(int i = 0 ; i < versionsArray.length; i++){
            for (int j = 0; j < versionsArray[i].length; j++) {
                if(versionsArray[i][j] > _version.versionsArray[i][j]){
                    return 1;
                }
            }
        }
        return 0;
    }

//-------------------------------------------------------------------------------------
    private void extractVersionsArray(){
        extractVersionsArray(3);
    }

//-------------------------------------------------------------------------------------
    private void extractVersionsArray(int _size){
        versionsArray = new int[items.length][_size];
        for(int i = 0; i < items.length; i++){
            versionsArray[i] = extractVersionsArray(versions[i], _size);
        }
    }

//-------------------------------------------------------------------------------------
    public int[] extractVersionsArray(String _version, int _size){
        if(_version == null){
            log.error("_version is null!");
            return null;
        }
        String[] _splitted = _version.split("\\.");
        int[] _versionInt = new int[_size];
        if(_splitted.length == _size){
            for(int i = 0; i < _size; i++){
                _versionInt[i] = Integer.parseInt(_splitted[i]);
            }
            return _versionInt;
        } else {
            return null;
        }     
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
