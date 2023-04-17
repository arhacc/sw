//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.isa;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.nio.*;
import java.nio.file.*;
import java.nio.charset.*;
import java.util.*;
import java.util.stream.*;

import com.esotericsoftware.kryo.kryo5.io.Input;
import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;


//-------------------------------------------------------------------------------------
public class Data {
    private transient Logger log;
    private int address;

    private List<DataLine> dataLines;

//-------------------------------------------------------------------------------------
    public Data(Logger _log, int _address, Path _path) {
        log = _log;
        address = _address;
        dataLines = new ArrayList<DataLine>();
        loadDataFromPath(_path);
    }

//-------------------------------------------------------------------------------------
    public int getAddress(){
        return address;
    }

//-------------------------------------------------------------------------------------
// TODO:
//convert from String reading to bin reading!
//align data with the arhitecture width
    
    private void loadDataFromPath(Path _path) {
        // text:
        /*try{
            List<String> _list = Files.readAllLines(_path);
            for(int i = 0; i < _list.size(); i++){
                String _line = _list.get(i);
                add(_line);
            }
            */
        // binary:
        try {
            Input _input = new Input(new FileInputStream(_path.toString()));
            long _filesize = new File(_path.toString()).length();

            while (_input.canReadLong()) {
                dataLines.add(new DataLine(_input.readLong()));
                _filesize -= Long.BYTES;
            }

            if (_filesize > 0) {
                long remdata = 0;
                while (_filesize-- > 0) {
                    remdata = (remdata << 8) + _input.readByte();
                }
                dataLines.add(new DataLine(remdata));
            }
        } catch(IOException _e){
            log.error("Cannot read from: " + _path);            
        }
    }

//-------------------------------------------------------------------------------------
    private void addTextline(String _line) {
        _line = _line.replaceAll("_", "");
        _line = _line.trim();
        if(!_line.isEmpty()){
            dataLines.add(new DataLine(_line));
        }
    }

//-------------------------------------------------------------------------------------
    public List<DataLine> getAll() {
        return dataLines;
    }
    
//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
