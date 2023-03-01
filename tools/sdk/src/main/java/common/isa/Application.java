//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.isa;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.nio.file.*;
import java.util.*;

import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.isa.*;
import xpu.sw.tools.sdk.common.fileformats.hex.*;
import xpu.sw.tools.sdk.common.fileformats.obj.*;
import xpu.sw.tools.sdk.common.fileformats.json.*;

//-------------------------------------------------------------------------------------
public class Application {
    private transient Logger log;

    private String path;
    private List<Primitive> primitives;
    private List<Data> datas;
    private List<Long> features;
    private int highestAddress;

//-------------------------------------------------------------------------------------
    public Application(Logger _log, String _path) {
        log = _log;
        path = _path;
        primitives = new ArrayList<Primitive>();
        datas = new ArrayList<Data>();
        features = new ArrayList<>();
        highestAddress = 0;
    }

//-------------------------------------------------------------------------------------
    public void add(Primitive _primitive) {
        primitives.add(_primitive);
    }

//-------------------------------------------------------------------------------------
    public void add(Data _data) {
        datas.add(_data);
    }

//-------------------------------------------------------------------------------------
    public void add(Long _feature) { features.add(_feature); }

//-------------------------------------------------------------------------------------
    public boolean resolve() {
        return primitives.stream()
                .map(Primitive::resolve)
                .reduce(Boolean.TRUE, Boolean::logicalAnd);
    }

//-------------------------------------------------------------------------------------
    public boolean pack() {
        boolean _success = primitives.stream()
                .map(Primitive::pack)
                .reduce(Boolean.TRUE, Boolean::logicalAnd);

        //TODO:
        // check for overlaping memory segments
        // check for missing references
        return _success;
    }


//-------------------------------------------------------------------------------------
    public void addData(int _address, Path _path){
        Data _data = new Data(log, _address, _path);
        add(_data);
    }

//-------------------------------------------------------------------------------------
    public void addFeature(long value){
        features.add(value);
    }

//-------------------------------------------------------------------------------------
    public HexFile exportHexFile() {
        return new HexFile(log, path, primitives, datas, features);
    }

//-------------------------------------------------------------------------------------
    public ObjFile exportObjFile() {
        return new ObjFile(log, path, primitives, datas, features);
    }

//-------------------------------------------------------------------------------------
    public JsonFile exportJsonFile() {
        return new JsonFile(log, path, primitives, datas, features);
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
