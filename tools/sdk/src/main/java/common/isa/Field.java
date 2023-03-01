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
    protected int width;

//-------------------------------------------------------------------------------------
    public Field(String _name) {
        this(_name, 0, 0);
    }

//-------------------------------------------------------------------------------------
    public Field(String _name, int _width) {
        this(_name, 0, _width);
    }

//-------------------------------------------------------------------------------------
    public Field(String _name, int _data, int _width) {
        setName(_name);
        setData(_data, _width);
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
    public int getWidth() {
        return data.length();
    }

//-------------------------------------------------------------------------------------
    public void setWidth(int _width) {
//        return data.length();
        width = _width;
    }

//-------------------------------------------------------------------------------------
    public int getData() {
        return intdata;
    }

//-------------------------------------------------------------------------------------
    public void setData(int _data, int _width) {
        intdata = _data;
        data = new FixedBitSet(_width);
        for(int i = 0; i < _width; i++){
            boolean _set = (_data & 0x01) == 0x01;
            if(_set){
                data.set(i);
            }
            _data >>= 1;
        }
        width = _width;
    }

/*
//-------------------------------------------------------------------------------------
    public final int getData() {
        return intdata;
    }
*/
//-------------------------------------------------------------------------------------
    public String toString() {
        return getName();
    }
//-------------------------------------------------------------------------------------
    public String dump() {
        return getName() + ": ["+name+"][" + data + "]";
    }

//-------------------------------------------------------------------------------------
    public static byte[] pack(ArchitectureImplementation _architectureImplementation, Field _field1, Field _field2, Field _field3) {
//        System.out.println("Field.toHex.In:\n 1:" + _field1.dump() + "\n 2:" + _field2.dump() + "\n 3:" +_field3.dump());
        FixedBitSet _data;
        _data = BitUtils.concatenate(_field1.data, _field2.data);
        _data = BitUtils.concatenate(_data, _field3.data);
//        System.out.println("Field.toHex.Out:" + _data.width());
        byte[] _array = BitUtils.toByteArray(_data);
/*        System.out.print("Field._array:");
        for(int i = 0; i < _array.width; i++){
            System.out.print(i + "=[" + _array[i] + "], ");
        }
        System.out.println("");*/
//        return byteToInt(_array);
        return _array;
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
