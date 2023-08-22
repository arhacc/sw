//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.context.version;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import org.apache.commons.configuration2.*;
import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.io.*;

//-------------------------------------------------------------------------------------
public class VersionApp {
    private Logger log;
    private VersionItem[] versionItems;
    private static final String classpathSeparator = System.getProperty("path.separator");
    private static final String fileSeparator = PathResolver.separator; 

//-------------------------------------------------------------------------------------
    public VersionApp(Logger _log, String _item){
        this(_log, new String[]{_item});
    }

// _items must be filled in an inclusion aware order
//Example: _items = new String[2]{"xpu-sdk-libs-", "xpu-sdk-"};
//-------------------------------------------------------------------------------------
    public VersionApp(Logger _log, String[] _items){
        log = _log;
        versionItems = new VersionItem[_items.length];
        for (int i = 0; i < _items.length; i++) {
            versionItems[i] = new VersionItem(_log, _items[i]);
        }
        try {
            String _cp = System.getProperty("java.class.path");
//            _log.info("_cp=" + _cp);
            String[] _classpathEntries = _cp.split(classpathSeparator);
            for (String _classpathEntry : _classpathEntries) {
//                _log.info(">>>>" + _classpathEntry);
                scanClasspathEntry(_classpathEntry);
            }
        } catch (Exception _e) {
            log.warn("Warning: Cannot load versions: " + _e.getMessage());   
            _e.printStackTrace();
        }
    }
/*
//-------------------------------------------------------------------------------------
    public VersionApp copyOf(){
        VersionApp _versionApp = new VersionApp(log);
        _versionApp.versionItems = new VersionApp[versionItems.length];
        for(int i = 0; i < versionItems.length; i++){
            _versionApp.versionItems[i] = versionItems[i].copyOf();
        }
    }
*/
//-------------------------------------------------------------------------------------
    public void scanClasspathEntry(String _classpathEntry){
        for (int i = 0; i < versionItems.length; i++) {
            String _item = versionItems[i].getName();
//            System.out.println(_p);
            String[] _files = _classpathEntry.split(fileSeparator);
            String _filename = _files[_files.length - 1];
            if(_filename.indexOf("/") >= 0){
                _files = _filename.split("/");
                _filename = _files[_files.length - 1];
            }
//            log.info(":--->" + _filename);
            if(_filename.startsWith(_item)){
                versionItems[i].setValue(_filename.replace(_item + "-", "").replace(".jar:", "").replace(".jar", ""));
                break;
            }
        }
    }

//-------------------------------------------------------------------------------------
    public String getVersion(){
        return getVersion(0);
    }

//-------------------------------------------------------------------------------------
    public String getVersion(int _index){
        return versionItems[_index].getValue();
    }

//no need for HashMap
//usually called once per run app
//-------------------------------------------------------------------------------------
    public String getVersion(String _key){
        return getVersionItem(_key).getValue();
    }

//-------------------------------------------------------------------------------------
    public VersionItem getVersionItem(String _key){
        for(int i = 0; i < versionItems.length; i++){
            if(versionItems[i].getName().equals(_key)){
                return versionItems[i];
            }
        }
        return null;
    }

//-------------------------------------------------------------------------------------
    public VersionItem[] getVersionItems(){
        return versionItems;
    }
/*
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
    public String toString(){
        return String.join(".", versions);
    }


//-------------------------------------------------------------------------------------
    public static String getVersionFromUrl(String _url) {
        String[] _array = _url.split("/");
        _array = _array[_array.length - 1].split("-");
        String _version = _array[_array.length - 1].replace(".jar","");
        return _version;
    }

//-------------------------------------------------------------------------------------
    private static boolean compare(String _version, String _currentLastVersion) {
        return _version.trim().compareTo(_currentLastVersion.trim()) > 0;
    }
*/
//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
