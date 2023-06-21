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
    private Map<String, Primitive> primitives;
    private Map<String, Macro> macros;
    private List<Data> datas;
    private List<Long> features;
    private int highestAddress;

//-------------------------------------------------------------------------------------
    public Application(Logger _log, String _path) {
        log = _log;
        path = _path;
        primitives = new HashMap<String, Primitive>();
        macros = new HashMap<String, Macro>();
        datas = new ArrayList<Data>();
        features = new ArrayList<>();
        highestAddress = 0;
    }

//-------------------------------------------------------------------------------------
    public Primitive getPrimitive(String _primitiveName) {
        return primitives.get(_primitiveName);
    }

//-------------------------------------------------------------------------------------
    public Macro getMacro(String _macroName) {
        return macros.get(_macroName);
    }

//-------------------------------------------------------------------------------------
    public void add(Primitive _primitive) {
//        log.debug("App add primitive: " + _primitive);
        primitives.put(_primitive.getName(), _primitive);
    }

//-------------------------------------------------------------------------------------
    public void add(Macro _macro) {
//        log.debug("App add primitive: " + _primitive);
        macros.put(_macro.getName(), _macro);
    }

//-------------------------------------------------------------------------------------
    public void add(Data _data) {
        datas.add(_data);
    }

//-------------------------------------------------------------------------------------
    public void add(Long _feature) { 
        features.add(_feature); 
    }

//-------------------------------------------------------------------------------------
    public String getLineAt(int _pc){
        Primitive[] _primitives = (Primitive[])primitives.values().toArray(new Primitive[]{});
        for(int i = 0; i < _primitives.length; i++) {
            Primitive _primitive = _primitives[i];
            String _line = _primitive.getLineAt(_pc);
            if(_line != null){
                return _line;
            }
        }
        return null;
    }
    

//-------------------------------------------------------------------------------------
    public boolean link() {
        return primitives.values().stream()
                .map(Primitive::link)
                .reduce(Boolean.TRUE, Boolean::logicalAnd);
    }

//-------------------------------------------------------------------------------------
    public boolean resolve() {
        return primitives.values().stream()
                .map(Primitive::resolve)
                .reduce(Boolean.TRUE, Boolean::logicalAnd);
    }

//-------------------------------------------------------------------------------------
    public boolean pack() {
        log.debug("App packing " + primitives.size() + " primitives...");
        boolean _success = primitives.values().stream()
                .map(Primitive::pack)
                .reduce(Boolean.TRUE, Boolean::logicalAnd);

        List<Primitive> _primitives = new ArrayList<Primitive>(primitives.values());
        for(int i = 0; i < _primitives.size(); i++){
            Primitive _primitive = _primitives.get(i);
            log.debug("[" + _primitive.getName() + "] : " + _primitive.getAll().size() + " lines");
        }

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
