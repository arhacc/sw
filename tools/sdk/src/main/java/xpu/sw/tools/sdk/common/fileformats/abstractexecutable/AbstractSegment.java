//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.fileformats.abstractexecutable;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;
import org.apache.lucene.util.*;


//-------------------------------------------------------------------------------------
public class AbstractSegment implements Serializable {
    private transient Logger log;

    private String name;
    private int address;
    private long[] data;

//-------------------------------------------------------------------------------------
    public AbstractSegment() {
        this(null, "", 0, null);
    }

//-------------------------------------------------------------------------------------
    public AbstractSegment(Logger _log) {
        this(_log, "", 0, null);
    }

//-------------------------------------------------------------------------------------
    public AbstractSegment(Logger _log, int _address) {
        this(_log, "", _address, null);
    }

//-------------------------------------------------------------------------------------
    public AbstractSegment(Logger _log, String _name) {
        this(_log, _name, 0, null);
    }

//-------------------------------------------------------------------------------------
    public AbstractSegment(Logger _log, int _address, long[] _data) {
        this(_log, "", _address, _data);
    }

//-------------------------------------------------------------------------------------
    public AbstractSegment(Logger _log, String _name, int _address, long[] _data) {
        log = _log;
        name = _name;
        address = _address;
        data = _data;
    }

//-------------------------------------------------------------------------------------
    public String getName() {
        return name;
    }

//-------------------------------------------------------------------------------------
    public int getAddress() {
        return address;
    }

//-------------------------------------------------------------------------------------
    public void setName(String _name) {
        name = _name;
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
