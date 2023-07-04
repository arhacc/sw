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

import xpu.sw.tools.sdk.common.isa.flow.*;
import xpu.sw.tools.sdk.common.isa.instruction.*;
import xpu.sw.tools.sdk.common.fileformats.core.*;
import xpu.sw.tools.sdk.common.fileformats.abstractexecutable.*;

//-------------------------------------------------------------------------------------
public class ObjFile extends AbstractExecutableFile {

    public static final String EXTENSION = "obj";

//-------------------------------------------------------------------------------------
    public ObjFile(Logger _log, String _path) {
        super(_log, _path, EXTENSION);
    }

//-------------------------------------------------------------------------------------
    public ObjFile(Logger _log, String _path, String _extension) {
        super(_log, _path, _extension);
    }

//-------------------------------------------------------------------------------------
    public ObjFile(Logger _log, String _path, Map<String, Primitive> _primitives, List<Data> _datas, List<Long> _features) {
        super(_log, _path, EXTENSION, _primitives, _datas, _features);
    }


//-------------------------------------------------------------------------------------
    @SuppressWarnings("unchecked")
    /**
     * Loads an object file from path, reading its segments and checking the CRC to match
     * The object file contains in this order:
     *  featuresSegments
     *  codeSegments
     *  dataSegments
     *  crc
     */
    public void load() {
        try {
//          Input _input = new Input(new GZIPInputStream(new FileInputStream(path)));
            Input _input = new Input(new FileInputStream(path));

            crcValue = 0;
            featureSegments = readSegments(log, _input);
            codeSegments = readSegments(log, _input);
            dataSegments = readSegments(log, _input);
            crcValue ^= _input.readInt();

            _input.close();
//          log.info("Loading [" + _index + ":" + _filePath + "]...OK[" + _data.size() + " entries]");
            log.info("Loading [" + path + "]...OK");
            if (isValid()) log.info("Loading [" + path + "]...OK");
            else log.error("Loading [" + path + "]...BAD CRC");
            //saveTestSegment();
        }
        catch(Exception _e){
            log.info("Loading [" + path + "]...error" + _e.getMessage());
        }
    }

    /**
     * Reads segments from object file
     * @param _log Logger where messages are printed
     * @param _input InputStream for the obj file
     * @return An ArrayList of ObjSegments
     */
    private List<AbstractSegment> readSegments(Logger _log, Input _input) {
        List<AbstractSegment> ret = new ArrayList<>();
        int num = _input.readInt(); crcValue ^= num;
        while (num-- > 0) {
            int length = _input.readInt(); crcValue ^= length;
            int address = _input.readInt(); crcValue ^= address;
            long[]data = _input.readLongs(length);
            for (long l : data) crcValue ^= l;
            ret.add(new AbstractSegment(_log, address, data));
        }
        return ret;
    }

    /**
     * Writes segments to object file
     * @param _arr ArrayList of object segments to write
     * @param _output OutputStream where segments are written
     */
    private void saveSegments(List<AbstractSegment> _arr, Output _output) {
        crcValue ^= _arr.size();
        _output.writeInt(_arr.size());
        for (int i = 0; i < _arr.size(); i++) {
            _output.writeInt(_arr.get(i).getLength()); crcValue ^= _arr.get(i).getLength();
            _output.writeInt(_arr.get(i).getAddress()); crcValue ^= _arr.get(i).getAddress();
            _output.writeLongs(_arr.get(i).getData(), 0, _arr.get(i).getData().length);
            for (long l : _arr.get(i).getData())
                crcValue ^= l;
        }
    }

    private void saveTestSegment() {
        String filename = path + ".jpeg";
        Output _output = null;
        try {
            _output = new Output(new FileOutputStream(filename));
            _output.writeLongs(dataSegments.get(0).getData(), 0, dataSegments.get(0).getLength());
            _output.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
    }

//-------------------------------------------------------------------------------------
    public void save() {
        log.info("Save " + path + "... ");
        try {
            Output _output = new Output(new FileOutputStream(path));
            saveSegments(featureSegments, _output);
            saveSegments(codeSegments, _output);
            saveSegments(dataSegments, _output);
            _output.writeInt(crcValue);
            _output.close();
        } catch (Exception _e) {
            log.info("error: Cannot write object!" + _e.getMessage());
        }
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
