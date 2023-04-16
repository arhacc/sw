//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.isa;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.context.arch.*;

//-------------------------------------------------------------------------------------
public class Value extends Field {
    private ValueFormat format;

//-------------------------------------------------------------------------------------
    public Value() {
        super("", 0);
    }

//-------------------------------------------------------------------------------------
    public Value(String _valueString, int _valueNumber) {
        super(_valueString, _valueNumber);
    }

//-------------------------------------------------------------------------------------
    public Value(int _value) {
        super("const", _value);
    }

//-------------------------------------------------------------------------------------
    public Value clone(){
        return new Value(name, intdata);
    }

//-------------------------------------------------------------------------------------
    public void set(String[] _args){
        setName(_args[0]);
        if(_args.length > 1) {
//            try{
                int _data = Integer.parseInt(_args[1]);
                setData(_data);
/*            }catch(NumberFormatException _e){

            }*/
        }
    }

//-------------------------------------------------------------------------------------
    public String getName() {
        return name;
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
