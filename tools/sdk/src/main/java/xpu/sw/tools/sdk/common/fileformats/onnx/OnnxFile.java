//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.fileformats.onnx;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;
import org.apache.lucene.util.*;

import com.esotericsoftware.kryo.kryo5.*;
import com.esotericsoftware.kryo.kryo5.io.*;

import ai.onnxruntime.*;

import xpu.sw.tools.sdk.common.isa.*;
import xpu.sw.tools.sdk.common.fileformats.core.*;
import xpu.sw.tools.sdk.common.fileformats.abstractexecutable.*;

//-------------------------------------------------------------------------------------
public class OnnxFile extends AbstractExecutableFile {
    private OrtSession session;

    public static final String EXTENSION = "onnx";

//-------------------------------------------------------------------------------------
    public OnnxFile(Logger _log, String _path) {
        super(_log, _path, EXTENSION);
        try{
            OrtEnvironment _env = OrtEnvironment.getEnvironment();
            session = _env.createSession(_path, new OrtSession.SessionOptions());
        } catch(OrtException _e){
            _e.printStackTrace();
        }
    }

//-------------------------------------------------------------------------------------
    public String getText(){
        try{
            return "ONNX file: \n" +
                "Inputs:" + session.getNumInputs() + "\n" +
                "Outputs:" + session.getNumOutputs() + "\n" +
                "Meta:" + session.getMetadata();
        } catch(OrtException _e){
            _e.printStackTrace();
        }
        return "";
    }


//-------------------------------------------------------------------------------------
    public void load() {

    }

//-------------------------------------------------------------------------------------
    public void save() {

    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
