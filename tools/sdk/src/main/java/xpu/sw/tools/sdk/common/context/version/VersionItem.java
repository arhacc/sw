//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.context.version;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import org.apache.commons.configuration2.*;
import org.apache.logging.log4j.*;

//-------------------------------------------------------------------------------------
public class VersionItem {
    private Logger log;
    private String name;
    private String value;
    private int[] valueArray;

//-------------------------------------------------------------------------------------
    public VersionItem(Logger _log, String _name){
        this(_log, _name, null, null);
    }

//-------------------------------------------------------------------------------------
    public VersionItem(Logger _log, String _name, String _value, int[] _valueArray){
        log = _log;
        name = _name;
        value = _value;
        valueArray = _valueArray;
    }

//-------------------------------------------------------------------------------------
    public VersionItem copyOf(){
        return new VersionItem(log, name, value, Arrays.copyOf(valueArray, valueArray.length));
    }

//-------------------------------------------------------------------------------------
    public String getName(){
        return name;
    }

//-------------------------------------------------------------------------------------
    public String getValue(){
        return value;
    }

//-------------------------------------------------------------------------------------
    public void setValue(String _value){
        value = _value;
        extractValueArray();
    }

//-------------------------------------------------------------------------------------
    public int compareTo(VersionItem _versionItem){
        for(int i = 0 ; i < valueArray.length; i++){
//            log.debug("valueArray["+i+"]="+valueArray[i] + ",_versionItem.valueArray["+i+"]="+_versionItem.valueArray[i]);
            if(valueArray[i] > _versionItem.valueArray[i]){
                return 1;
            } else if(valueArray[i] < _versionItem.valueArray[i]){
                return -1;
            }
        }
        return 0;
    }

//-------------------------------------------------------------------------------------
    public void extractValueArray(){
        String[] _splitted = value.split("\\.");
        if(valueArray == null){
            valueArray = new int[_splitted.length];
        }
        for(int i = 0; i < _splitted.length; i++){
            valueArray[i] = Integer.parseInt(_splitted[i]);
        }
    }

//-------------------------------------------------------------------------------------
    public String toString(){
        return value;
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
