//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.isa.flow;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.nio.file.*;
import java.util.*;

import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;
import org.antlr.v4.runtime.tree.*;

//import xpu.sw.tools.sdk.common.isa.*;
import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.context.arch.*;
import xpu.sw.tools.sdk.common.debug.*;
import xpu.sw.tools.sdk.common.fileformats.hex.*;
import xpu.sw.tools.sdk.common.fileformats.obj.*;
import xpu.sw.tools.sdk.common.fileformats.json.*;
import xpu.sw.tools.sdk.common.xbasics.*;
import xpu.sw.tools.sdk.asm.parser.*;

//-------------------------------------------------------------------------------------
public class Application extends XBasic {
    protected String architectureId;
    protected ArchitectureImplementation architectureImplementation;

    private String path;
    private Map<String, Expression> defines;
    private Map<String, Primitive> primitives;
    private Map<String, Macro> macros;
    private List<Data> datas;
    private List<Long> features;
    private int highestAddress;
//    private DebugInformation debugInformation;

//-------------------------------------------------------------------------------------
    public Application(Context _context, String _path) {
        super(_context);
        path = _path;
        defines = new HashMap<String, Expression>();
        primitives = new HashMap<String, Primitive>();
        macros = new HashMap<String, Macro>();
        datas = new ArrayList<Data>();
        features = new ArrayList<>();
        highestAddress = 0;
//        debugInformation = new DebugInformation(_context, _path);
    }

//-------------------------------------------------------------------------------------
    public String getArchitectureId(){
        return architectureId;
    }

//-------------------------------------------------------------------------------------
    public void setArchitectureId(String _architectureId){
        architectureId = _architectureId;
        architectureImplementation = context.getArchitectureImplementations().getArchitecture(_architectureId);
    }

//-------------------------------------------------------------------------------------
    public ArchitectureImplementation getArchitectureImplementation(){
        return architectureImplementation;
    }

//-------------------------------------------------------------------------------------
    public Expression getDefine(String _defineName) {
        return defines.get(_defineName);
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
    public void addDefine(String _name, AsmParser.ExpressionContext _expressionContext) {
        defines.put(_name, new Expression(this, _expressionContext));
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
    public String getLineTextAt(int _pc){
        Primitive[] _primitives = (Primitive[])primitives.values().toArray(new Primitive[]{});
        for(int i = 0; i < _primitives.length; i++) {
            Primitive _primitive = _primitives[i];
            String _line = _primitive.getLineTextAt(_pc);
            if(_line != null){
                return _line;
            }
        }
        return null;
    }
    

//-------------------------------------------------------------------------------------
    public boolean link() {
//        log.debug("Linking application:" +this);
        return primitives.values().stream()
                .map(Primitive::link)
                .map(_int -> (_int > 0))
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
//            debugInformation.add(_primitive.getDebugInformation());
            log.debug("[" + _primitive.getName() + "] : " + _primitive.size() + " instructions");
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
