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
            // top-level map with 2 keys
            packer.packMapHeader(2);
            log.debug("MsgPack save: top-level map with 2 keys");

            // agregăm toate liniile de instrucțiune din toate primitivele
            List<Callable> allLines = new ArrayList<>();
            for (Primitive primitive : primitives.values()) {
                allLines.addAll(primitive.getAll());
            }

            // debug_lines: index -> line number
            packer.packString("debug_lines");
            log.debug("MsgPack save: debug_lines size=" + allLines.size());
            packer.packMapHeader(allLines.size());
            for (int i = 0; i < allLines.size(); i++) {
                Callable line = allLines.get(i);
                int lineNo = line.getLocalization().getLineNoInFile();
                packer.packString(String.valueOf(i));
                packer.packInt(lineNo);
                log.debug("MsgPack save debug_lines[" + i + "]=" + lineNo);
            }

            // instructions: vector de 32-bit
            packer.packString("instructions");
            log.debug("MsgPack save: instructions size=" + allLines.size());
            packer.packArrayHeader(allLines.size());
            int idx = 0;
            for (Callable line : allLines) {
                if (line instanceof InstructionLine) {
                    List<Long> bins = ((InstructionLine) line).toBin();
                    int dataLo = bins.get(0).intValue();
                    packer.packInt(dataLo);
                    log.debug("MsgPack save instructions[" + idx + "]=" + Integer.toHexString(dataLo));
                } else {
                    packer.packInt(0);
                    log.debug("MsgPack save instructions[" + idx + "]=0");
                }
                idx++;
            }

            packer.close();
        } catch (IOException _e) {
            log.error("Cannot write MsgPack object! " + _e.getMessage());
        }
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
