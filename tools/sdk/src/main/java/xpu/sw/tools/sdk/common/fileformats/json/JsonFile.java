//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.fileformats.json;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;
import org.apache.lucene.util.*;

//import com.esotericsoftware.kryo.kryo5.*;
//import com.esotericsoftware.kryo.kryo5.io.*;

import xpu.sw.tools.sdk.common.isa.flow.*;
import xpu.sw.tools.sdk.common.isa.instruction.*;
import xpu.sw.tools.sdk.common.fileformats.core.*;
import xpu.sw.tools.sdk.common.fileformats.abstractexecutable.*;

//import javax.json.*;
import jakarta.json.*;
import javax.json.stream.*;

//-------------------------------------------------------------------------------------
public class JsonFile extends AbstractExecutableFile {

    public static final String EXTENSION = "json";

//-------------------------------------------------------------------------------------
    public JsonFile(Logger _log, String _path) {
        super(_log, _path, EXTENSION);
    }

//-------------------------------------------------------------------------------------
    public JsonFile(Logger _log, String _path, Map<String, Primitive> _primitives, List<Data> _datas, List<Long> _features) {
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
            InputStream _inputstream = new FileInputStream(path);
            JsonReader _jsonReader = Json.createReader(_inputstream);
            JsonObject _jsonObject = _jsonReader.readObject();

            crcValue = 0;
            featureSegments = readSegment(_jsonObject, "features");
            codeSegments = readSegment(_jsonObject, "code");
            dataSegments = readSegment(_jsonObject, "data");

//            JsonReader jsonReader = Json.createReader(_inputstream);
//            JsonObject obj = _jsonReader.readObject();
            if (_jsonObject.containsKey("crc")) {
                crcValue ^= ((JsonNumber)_jsonObject.get("crc")).intValue();
            }

            _inputstream.close();
            log.info("Loading [" + path + "]...OK");
            if (isValid()) log.info("Loading [" + path + "]...OK");
            else log.error("Loading [" + path + "]...BAD CRC");
        } catch(JsonParsingException _e){
            log.info("Loading [" + path + "]...error:JsonParsingException" + _e.getMessage() + ": " + _e.getLocation());
            _e.printStackTrace();
        } catch(JsonException _e1){
            log.info("Loading [" + path + "]...error:JsonException" + _e1.getMessage());
            _e1.printStackTrace();
        } catch(Exception _e2){
            log.info("Loading [" + path + "]...error" + _e2.getMessage());
            _e2.printStackTrace();
        }
        mainFunctionName = codeSegments.get(0).getName();
    }

//-------------------------------------------------------------------------------------
    /**
     * Reads segments from object file
     * @param _log Logger where messages are printed
     * @param _input InputStream for the obj file
     * @return An ArrayList of ObjSegments
     */
    protected ArrayList<AbstractSegment> readSegment(JsonObject _jsonObject, String _name) {
        ArrayList<AbstractSegment> ret = new ArrayList<>();

//        JsonReader jsonReader = Json.createReader(_inputstream);
//        log.debug("JsonObject: " + obj);
        if (_jsonObject.containsKey(_name))
        {
            JsonArray js = _jsonObject.getJsonArray(_name);
            crcValue ^= js.size();

            for (int i = 0; i < js.size(); i++) {
                JsonObject jo = js.getJsonObject(i);
//                if (jo.containsKey("length") && jo.containsKey("address") && jo.containsKey("data")) {
                    AbstractSegment os = new AbstractSegment();

                    int length = ((JsonNumber)jo.get("length")).intValue();
                    crcValue ^= length;

                    String _primitiveName = (jo.get("name").toString());
                    os.setName(_primitiveName);

                    int address = ((JsonNumber)jo.get("address")).intValue();
                    crcValue ^= address;
                    os.setAddress(address);

                    JsonArray jarrdata = jo.getJsonArray("payload");
                    long []l = new long[length];

                    for (int j = 0; j < jarrdata.size(); j++) {
                        l[j] = ((JsonNumber) jarrdata.get(j)).longValue();
                        crcValue ^= l[j];
                    }
                    os.setData(l);
                    ret.add(os);
/*                } else {
                    _log.error("Cannot find a json part (length / address / data) for index " + i + " and name " + _name);
                }*/
            }
        } else {
            log.error("Cannot find json section with name " + _name);
        }
        return ret;
    }

//-------------------------------------------------------------------------------------
    /**
     * Writes segments to object file
     * @param _arr ArrayList of object segments to write
     */
    protected JsonArrayBuilder getSegments(List<AbstractSegment> _arr) {
        crcValue ^= _arr.size();
        JsonArrayBuilder ret = Json.createArrayBuilder();

        for (int i = 0; i < _arr.size(); i++) {
            JsonObjectBuilder objBuilder = Json.createObjectBuilder();

            objBuilder.add("length", _arr.get(i).getLength());
            _arr.get(i).getLength(); crcValue ^= _arr.get(i).getLength();

            objBuilder.add("name", _arr.get(i).getName());

            objBuilder.add("address", _arr.get(i).getAddress());
            _arr.get(i).getAddress(); crcValue ^= _arr.get(i).getAddress();

            JsonArrayBuilder arrBuilder = Json.createArrayBuilder();
            for (long l : _arr.get(i).getData()) {
                arrBuilder.add(l);
                crcValue ^= l;
            }
            objBuilder.add("payload", arrBuilder);
            ret.add(objBuilder);
        }
        return ret;
    }

//-------------------------------------------------------------------------------------
    public String alignSection(String _str, String _section) {
        String[] _parts = _str.split("\""+ _section + "\"");
        return _parts[0] + "\n\n" + "\""+ _section + "\"" + _parts[1];
    }

//-------------------------------------------------------------------------------------
    public void save() {
        log.info("Save " + path + "... ");
        try {
            JsonArrayBuilder fs = getSegments(featureSegments);
            JsonArrayBuilder cs = getSegments(codeSegments);
            JsonArrayBuilder ds = getSegments(dataSegments);

            JsonObjectBuilder jo = Json.createObjectBuilder();
            jo.add("features", fs);
            jo.add("code", cs);
            jo.add("data", ds);
            jo.add("crc", crcValue);

            OutputStream os = new ByteArrayOutputStream();
            JsonWriter jsonWriter = Json.createWriter(os);
            jsonWriter.writeObject(jo.build());
            String strToFile = os.toString();

            strToFile = alignSection(strToFile, "features");
            strToFile = alignSection(strToFile, "code");
            strToFile = alignSection(strToFile, "data");
            strToFile = alignSection(strToFile, "crc");

//            strToFile = strToFile.replaceAll("\\{", "\\{\n");
//            strToFile = strToFile.replaceAll("\\}", "\n\\}");
            if (strToFile.lastIndexOf("}") > 0) {
                strToFile = strToFile.substring(0, strToFile.lastIndexOf("}"));
                strToFile += "\n}";
            }

            FileOutputStream fos = new FileOutputStream(path);
            fos.write(strToFile.getBytes());
            fos.close();

        } catch (Exception _e) {
            log.info("error: Cannot write object!" + _e.getMessage());
            _e.printStackTrace();
        }
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
