//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.fileformats.obj;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;
import org.apache.lucene.util.*;

import com.esotericsoftware.kryo.kryo5.*;
import com.esotericsoftware.kryo.kryo5.io.*;

import xpu.sw.tools.sdk.common.isa.*;
import xpu.sw.tools.sdk.common.fileformats.hex.*;

//-------------------------------------------------------------------------------------
public class ObjSegment {
    private transient Logger log;

    private int address;
    private long[] data;

//-------------------------------------------------------------------------------------
    public ObjSegment() {
        this(null, 0, null);
    }

//-------------------------------------------------------------------------------------
    public ObjSegment(Logger _log) {
        this(_log, 0, null);
    }

//-------------------------------------------------------------------------------------
    public ObjSegment(Logger _log, int _address) {
        this(_log, _address, null);
    }

//-------------------------------------------------------------------------------------
    public ObjSegment(Logger _log, int _address, long[] _data) {
        log = _log;
        address = _address;
        data = _data;
    }

//-------------------------------------------------------------------------------------
    public int getAddress() {
        return address;
    }

//-------------------------------------------------------------------------------------
    public void setAddress(int _address) {
        address = _address;
    }

//-------------------------------------------------------------------------------------
    public long[] getData() {
        return data;
    }

//-------------------------------------------------------------------------------------
    public void setData(long[] _data) {
        data = _data;
    }

//-------------------------------------------------------------------------------------
    public int getLength(){
        return data.length;
    }

//-------------------------------------------------------------------------------------
    public void setData(List<Long> _data) {
        data = _data.stream().mapToLong(Long::longValue).toArray();
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
