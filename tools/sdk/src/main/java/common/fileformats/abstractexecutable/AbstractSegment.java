//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.fileformats.abstractexecutable;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;
import org.apache.lucene.util.*;


//-------------------------------------------------------------------------------------
public class AbstractSegment {
    private transient Logger log;

    private int address;
    private long[] data;

//-------------------------------------------------------------------------------------
    public AbstractSegment() {
        this(null, 0, null);
    }

//-------------------------------------------------------------------------------------
    public AbstractSegment(Logger _log) {
        this(_log, 0, null);
    }

//-------------------------------------------------------------------------------------
    public AbstractSegment(Logger _log, int _address) {
        this(_log, _address, null);
    }

//-------------------------------------------------------------------------------------
    public AbstractSegment(Logger _log, int _address, long[] _data) {
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