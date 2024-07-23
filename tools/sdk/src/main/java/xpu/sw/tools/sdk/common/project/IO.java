//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.project;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.nio.file.*;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.*;

import ai.onnxruntime.*;

import org.apache.commons.configuration2.*;
import org.apache.commons.configuration2.builder.*;
import org.apache.commons.configuration2.builder.fluent.*;
import org.apache.commons.configuration2.ex.*;
import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.debug.*;
import xpu.sw.tools.sdk.common.io.PathResolver;
import xpu.sw.tools.sdk.common.fileformats.asm.*;
import xpu.sw.tools.sdk.common.fileformats.obj.*;
import xpu.sw.tools.sdk.common.fileformats.onnx.*;
import xpu.sw.tools.sdk.common.xbasics.*;
import xpu.sw.tools.sdk.common.isa.flow.*;
import xpu.sw.tools.sdk.common.isa.instruction.*;

//-------------------------------------------------------------------------------------
public class IO extends XBasic {
    private Project project;

    private int inputSize;
    private int outputSize;

    private List<String> inputNames;
    private List<String> inputResourceNames;
    
    private List<String> outputNames;
    private List<String> outputResourceNames;
    
//-------------------------------------------------------------------------------------
    public IO(Context _context, Project _project) {
        super(_context);

        project = _project;
        inputNames = new ArrayList<String>();
        inputResourceNames = new ArrayList<String>();

        outputNames = new ArrayList<String>();
        outputResourceNames = new ArrayList<String>();
    }

//-------------------------------------------------------------------------------------
    public void registerIO(String _filename){
        try{
            String _path = project.getRootPath() + PathResolver.separator + _filename;
            OrtEnvironment _env = OrtEnvironment.getEnvironment();
            OrtSession _session = _env.createSession(_path, new OrtSession.SessionOptions());
            inputSize = (int)_session.getNumInputs();
            outputSize = (int)_session.getNumOutputs();
            inputNames.addAll(_session.getInputNames());
            outputNames.addAll(_session.getInputNames());

//TBD!!!
//proper map of resources. For now name of the resource = name_of_port + ".data"
            for (int i = 0; i < inputNames.size(); i++) {
                inputResourceNames.add(inputNames.get(i) + ".mp");
            }

            for (int i = 0; i < outputNames.size(); i++) {
                outputResourceNames.add(outputNames.get(i) + ".mp");
            }

        } catch(Throwable _t){
            log.debug("Cannot load: " + _filename + "["+_t.getMessage()+"]");
//            _e.printStackTrace();
        }
    }

//-------------------------------------------------------------------------------------
    public int getNumberOfInputs(){
        return inputSize;
    }

//-------------------------------------------------------------------------------------
    public String getInputName(int _index){
        return inputNames.get(_index);
    }

//-------------------------------------------------------------------------------------
    public String getInputResourceName(int _index){
        return inputResourceNames.get(_index);
    }

//-------------------------------------------------------------------------------------
    public int getNumberOfOutputs(){
        return outputSize;
    }

//-------------------------------------------------------------------------------------
    public String getOutputName(int _index){
        return outputNames.get(_index);
    }

//-------------------------------------------------------------------------------------
    public String getOutputResourceName(int _index){
        return outputResourceNames.get(_index);
    }

//-------------------------------------------------------------------------------------
    public String toString(){
        return "IO: " + inputSize + " : " + outputSize;
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
