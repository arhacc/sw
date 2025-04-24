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

import xpu.sw.tools.sdk.common.debug.*;
import xpu.sw.tools.sdk.common.io.*;

import xpu.sw.tools.sdk.common.isa.flow.*;
import xpu.sw.tools.sdk.common.isa.instruction.*;
import xpu.sw.tools.sdk.common.fileformats.core.*;
import xpu.sw.tools.sdk.common.fileformats.abstractexecutable.*;

import org.msgpack.core.MessagePack;
import org.msgpack.core.MessagePacker;

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
//        debugInformation = _debugInformation;
    }
/*
//-------------------------------------------------------------------------------------
    public DebugInformation getDebugInformation() {
        return debugInformation;
    }
*/
//-------------------------------------------------------------------------------------
    public void load() {
        try {
//          Input _input = new Input(new GZIPInputStream(new FileInputStream(path)));
            ObjectInputStream _ois = new ObjectInputStream(new FileInputStream(PathResolver.importPath(path)));

            mainFunctionName = (String)_ois.readObject();
            featureSegments = (ArrayList<AbstractSegment>)_ois.readObject();
            codeSegments = (ArrayList<AbstractSegment>)_ois.readObject();
            dataSegments = (ArrayList<AbstractSegment>)_ois.readObject();
            primitives = (Map<String, Primitive>)_ois.readObject();
            _ois.close();
//          log.info("Loading [" + _index + ":" + _filePath + "]...OK[" + _data.size() + " entries]");
//            log.info("Loading [" + path + "]...OK");
            if (isValid()) {
//                log.info("Loading [" + path + "]...OK");
            } else {
//                log.error("Loading [" + path + "]...BAD CRC");
            }
            //saveTestSegment();
        } catch(Throwable _t){
//            log.info("Loading [" + path + "]...error: " + _t.getMessage());
            log.info("You might need to rerun asm for [" + path + "]");
//            _t.printStackTrace();
//            if(_t instanceof SerializableException){

//            }
        }
    }

//-------------------------------------------------------------------------------------
    private List<AbstractSegment> readSegments(Logger _log, ObjectInputStream _ois) throws IOException, ClassNotFoundException {
        List<AbstractSegment> ret = new ArrayList<>();
        int num = _ois.readInt(); crcValue ^= num;
        while (num-- > 0) {
            int length = _ois.readInt(); crcValue ^= length;
            int address = _ois.readInt(); crcValue ^= address;
            long[] data = (long[])_ois.readObject();
            for (long l : data) crcValue ^= l;
            ret.add(new AbstractSegment(_log, address, data));
        }
        return ret;
    }

//-------------------------------------------------------------------------------------
    private void saveSegments(List<AbstractSegment> _arr, ObjectOutputStream _oos) throws IOException {
        crcValue ^= _arr.size();
        _oos.writeInt(_arr.size());
        for (int i = 0; i < _arr.size(); i++) {
            _oos.writeInt(_arr.get(i).getLength()); crcValue ^= _arr.get(i).getLength();
            _oos.writeInt(_arr.get(i).getAddress()); crcValue ^= _arr.get(i).getAddress();
            _oos.writeObject(_arr.get(i).getData());
            for (long l : _arr.get(i).getData())
                crcValue ^= l;
        }
    }

//-------------------------------------------------------------------------------------
    private void saveTestSegment() throws IOException {
        String filename = path + ".jpeg";
        ObjectOutputStream _oos = null;
        try {
            _oos = new ObjectOutputStream(new FileOutputStream(filename));
            _oos.writeObject(dataSegments.get(0).getData());
            _oos.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
    }

//-------------------------------------------------------------------------------------
    public void save() {
        log.info("Save " + path + "... ");
        try {
            MessagePacker packer = MessagePack.newDefaultPacker(new FileOutputStream(path));
/*            // main function name
            packer.packString(mainFunctionName);
            // feature segments
            packer.packArrayHeader(featureSegments.size());
            for (AbstractSegment seg : featureSegments) {
                packer.packInt(seg.getLength());
                packer.packInt(seg.getAddress());
                long[] data = seg.getData();
                packer.packArrayHeader(data.length);
                for (long l : data) {
                    packer.packLong(l);
                }
            }
            // code segments
            packer.packArrayHeader(codeSegments.size());
            for (AbstractSegment seg : codeSegments) {
                packer.packInt(seg.getLength());
                packer.packInt(seg.getAddress());
                long[] data = seg.getData();
                packer.packArrayHeader(data.length);
                for (long l : data) {
                    packer.packLong(l);
                }
            }
            // data segments
            packer.packArrayHeader(dataSegments.size());
            for (AbstractSegment seg : dataSegments) {
                packer.packInt(seg.getLength());
                packer.packInt(seg.getAddress());
                long[] data = seg.getData();
                packer.packArrayHeader(data.length);
                for (long l : data) {
                    packer.packLong(l);
                }
            }*/
            // primitives map (serialize name only)
            packer.packMapHeader(primitives.size());
            for (Map.Entry<String, Primitive> entry : primitives.entrySet()) {
                String _name = entry.getKey();
                int _lineNo = entry.getValue().getLocalization().getLineNoInFile();
                log.debug("Primitive: " + _name + ", lineNo=" + _lineNo);
                packer.packString(_name);
                packer.packInt(_lineNo);
                // pack instruction index -> instruction line number map
                List<Callable> _lines = entry.getValue().getAll();
                packer.packMapHeader(_lines.size());
                for (int _i = 0; _i < _lines.size(); _i++) {
                    packer.packInt(_i);
                    packer.packInt(_lines.get(_i).getLocalization().getLineNoInFile());
                    log.debug("Primitive[" + _name + "]: instruction no: [" + _i + "] -> lineNo=" + _lines.get(_i).getLocalization().getLineNoInFile());
                }
            }
            packer.close();
        } catch (IOException _e) {
            log.error("Cannot write MsgPack object! " + _e.getMessage());
        }
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
