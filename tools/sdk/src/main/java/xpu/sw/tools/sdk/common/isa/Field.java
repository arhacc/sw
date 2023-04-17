//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.isa;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import org.apache.lucene.util.*;
import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.utils.*;
import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.context.arch.*;


//-------------------------------------------------------------------------------------
public class Field {
    protected String name;
    protected FixedBitSet data;
    protected int intdata;

//-------------------------------------------------------------------------------------
    public Field(String _name) {
        this(_name, 0);
    }

//-------------------------------------------------------------------------------------
    public Field(String _name, int _data) {
        setName(_name);
        setData(_data);
/*        if(_width > 32){
            throw new Throwable("width of the field cannot be longer than 32!");
        }
//        width = _width;
        int _bytewidth = _width/8;
        _bytewidth = (_bytewidth == 0) ? 1 : _bytewidth;
        data = new byte[_bytewidth];
        for(int i = 0; i < _bytewidth; i++){
            data[i] = (byte)((_data >> (i*8)) & 0x000000FF);
        }*/
    }

//-------------------------------------------------------------------------------------
    public String getName() {
        return name;
    }

//-------------------------------------------------------------------------------------
    public void setName(String _name) {
        name = _name;
    }

//-------------------------------------------------------------------------------------
    public int getData() {
        return intdata;
    }

//-------------------------------------------------------------------------------------
    public void setData(int _data) {
        intdata = _data;
    }

//-------------------------------------------------------------------------------------
    public void pack(int _width) {
        data = new FixedBitSet(_width);
        int _data = intdata;
        for(int i = 0; i < _width; i++){
            boolean _set = (_data & 0x01) == 0x01;
            if(_set){
                data.set(i);
            }
            _data >>>= 1;
        }
    }

//-------------------------------------------------------------------------------------
    public String toString() {
        return getName();
    }

//-------------------------------------------------------------------------------------
    public String dump() {
        return getName() + ": ["+intdata+"][" + data.toString() + "]";
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
