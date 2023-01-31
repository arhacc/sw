//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.isa;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;


//-------------------------------------------------------------------------------------
public class Value extends Cell {


//-------------------------------------------------------------------------------------
    public Value(String _name, int _length, int _data) {
        super(_name, _length, _data);
    }

//-------------------------------------------------------------------------------------
    public Value(int _data, int _length) {
        super("V", _length, _data);
    }

//-------------------------------------------------------------------------------------
    public static Value getValue(String _name, int _length) {
        int _data = 0;
        try{
            _data = Integer.parseInt(_name);
        }catch(Exception _e){
//            System.out.println("Exception:...[" + _name + "]");
            return new Value(_name, _length, -1);
        }
        return getValue(_data, _length);
    }

//-------------------------------------------------------------------------------------
    public static Value getValue(int _value, int _length) {
        return new Value(_value, _length);
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
