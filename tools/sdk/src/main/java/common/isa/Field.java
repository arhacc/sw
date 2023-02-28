//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.isa;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import org.apache.lucene.util.*;
import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.utils.*;

//-------------------------------------------------------------------------------------
public class Field {
    protected String name;
    protected FixedBitSet data;
    protected final int intdata;

//-------------------------------------------------------------------------------------
    public Field(String _name) {
        this(_name, 0, 0);
    }

//-------------------------------------------------------------------------------------
    public Field(String _name, int _length) {
        this(_name, 0, _length);
    }

//-------------------------------------------------------------------------------------
    public Field(String _name, int _data, int _length) {
        name = _name;
/*        if(_length > 32){
            throw new Throwable("Length of the field cannot be longer than 32!");
        }
//        length = _length;
        int _byteLength = _length/8;
        _byteLength = (_byteLength == 0) ? 1 : _byteLength;
        data = new byte[_byteLength];
        for(int i = 0; i < _byteLength; i++){
            data[i] = (byte)((_data >> (i*8)) & 0x000000FF);
        }*/
        intdata = _data;
        data = new FixedBitSet(_length);
        for(int i = 0; i < _length; i++){
            boolean _set = (_data & 0x01) == 0x01;
            if(_set){
                data.set(i);
            }
            _data >>= 1;
        }    
    }

//-------------------------------------------------------------------------------------
    public String getName() {
        return name;
    }

//-------------------------------------------------------------------------------------
    public int getLength() {
        return data.length();
    }

//-------------------------------------------------------------------------------------
    public int getData() {
        return intdata;
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
    public static int toBin(Field _field1, Field _field2, Field _field3) {
//        System.out.println("Field.toHex.In:\n 1:" + _field1.dump() + "\n 2:" + _field2.dump() + "\n 3:" +_field3.dump());
        FixedBitSet _data;
        _data = BitUtils.concatenate(_field1.data, _field2.data);
        _data = BitUtils.concatenate(_data, _field3.data);
//        System.out.println("Field.toHex.Out:" + _data.length());
        byte[] _array = BitUtils.toByteArray(_data);
/*        System.out.print("Field._array:");
        for(int i = 0; i < _array.length; i++){
            System.out.print(i + "=[" + _array[i] + "], ");
        }
        System.out.println("");*/
        return byteToInt(_array);
    }

//-------------------------------------------------------------------------------------
    public static String toHex(Field _field1, Field _field2, Field _field3) {
//        System.out.println("Field.toHex.In:\n 1:" + _field1.dump() + "\n 2:" + _field2.dump() + "\n 3:" +_field3.dump());
        FixedBitSet _data;
        _data = BitUtils.concatenate(_field1.data, _field2.data);
        _data = BitUtils.concatenate(_data, _field3.data);
//        System.out.println("Field.toHex.Out:" + _data.length());
        byte[] _array = BitUtils.toByteArray(_data);
/*        System.out.print("Field._array:");
        for(int i = 0; i < _array.length; i++){
            System.out.print(i + "=[" + _array[i] + "], ");
        }
        System.out.println("");*/
        return xpu.sw.tools.sdk.common.utils.StringUtils.bytesToHex(_array);
    }

//-------------------------------------------------------------------------------------
/** length should be less than 4 (for int) **/
    public static int byteToInt(byte[] _bytes) {
        int val = 0;
        if(_bytes.length>4) throw new RuntimeException("Too big to fit in int");
        for (int i = 0; i < _bytes.length; i++) {
            val=val<<8;
            val=val|(_bytes[i] & 0xFF);
        }
        return val;
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
