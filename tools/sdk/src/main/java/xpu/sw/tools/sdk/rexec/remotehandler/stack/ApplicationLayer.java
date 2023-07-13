//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.rexec.remotehandler.stack;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.nio.file.*;
import java.util.*;
import java.util.stream.*;

import org.apache.commons.configuration2.*;
import org.apache.logging.log4j.*;

import org.apache.commons.cli.*;
import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;

import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.tree.*;

import xpu.sw.tools.sdk.common.utils.*;

import xpu.sw.tools.sdk.*;
import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.project.*;
import xpu.sw.tools.sdk.common.fileformats.hex.*;
import xpu.sw.tools.sdk.common.fileformats.obj.*;
import xpu.sw.tools.sdk.common.fileformats.json.*;
import xpu.sw.tools.sdk.common.fileformats.onnx.*;
import xpu.sw.tools.sdk.common.io.*;
import xpu.sw.tools.sdk.common.io.targetmanager.*;
import xpu.sw.tools.sdk.asm.parser.*;

//-------------------------------------------------------------------------------------
public class ApplicationLayer extends CommandLayer {

//-------------------------------------------------------------------------------------
    public ApplicationLayer(Context _context, TargetManager _targetManager) {
        super(_context, _targetManager);
    }

//-------------------------------------------------------------------------------------
    protected void send(HexFile _hexFile) {
        sendInt(Command.COMMAND_LOAD_FILE_HEX);
        sendByte(Parameters.LEVEL_LOW);
        sendFile(_hexFile);
    }

//-------------------------------------------------------------------------------------
    protected void send(JsonFile _jsonFile) {
/*        while(status != STATUS_RUNNING){
            try{
                sleep(1000);
            }catch(InterruptedException _e){

            }
        }*/
        sendInt(Command.COMMAND_LOAD_FILE_JSON);
        sendByte(Parameters.LEVEL_LOW);
        sendFile(_jsonFile);
    }

//-------------------------------------------------------------------------------------
    protected void send(ObjFile _objFile) {
/*        while(status != STATUS_RUNNING){
            try{
                sleep(1000);
            }catch(InterruptedException _e){

            }
        }*/
        sendInt(Command.COMMAND_LOAD_FILE_OBJ);
        sendByte(Parameters.LEVEL_LOW);
        sendFeatures(_objFile.getFeatureSegment());
        sendCode(_objFile.getCodeSegment());
        sendData(_objFile.getDataSegment());
    }

//-------------------------------------------------------------------------------------
    protected void send(OnnxFile _onnxFile) {
        if(!isConnected()){
            log.debug(currentTargetConnection + " is not connected!");
            return;
        }
        sendInt(Command.COMMAND_LOAD_FILE_ONNX);
        sendFile(_onnxFile);
    }

//-------------------------------------------------------------------------------------
    protected int run(String _mainFunctionName) {
        sendInt(Command.COMMAND_RUN_FUNCTION);
        sendString(_mainFunctionName);
        int _responseCode = receiveInt();
        if(_responseCode == Command.COMMAND_ERROR){
            String _message = receiveString();
            log.error("Error runnig function: "  + _message);
        } else {
            log.error("Unknown response code after run function: " + _responseCode);
        }
        return _responseCode;
    }

//-------------------------------------------------------------------------------------
    public void debugRetreiveArrayRegistry(int[][] _data, int _indexXStart, int _indexXStop) {
        sendInt(Command.COMMAND_DEBUG_RETREIVE_ARRAY_REGISTRY);
        sendInt(_indexXStart);
        sendInt(_indexXStop);
//        int _lengthX = _indexXStop - _indexXStart;
        for (int i = _indexXStart; i <= _indexXStop; i++) {
            for(int j = 0; j <= 5; j++){
                _data[i][j] = receiveInt();
            }
        }
    }

//-------------------------------------------------------------------------------------
    public void debugRetreiveArrayMemoryData(int[][] _data, int _indexXStart, int _indexXStop, int _indexYStart, int _indexYStop) {
        sendInt(Command.COMMAND_DEBUG_RETREIVE_ARRAY_MEMORY_DATA);
        sendInt(_indexXStart);
        sendInt(_indexXStop);
        sendInt(_indexYStart);//0
        sendInt(_indexYStop);//1023
//        int _lengthX = _indexXStop - _indexXStart;
//        int _lengthY = _indexYStop - _indexYStart;
//        int[][] _data = new int[_lengthX][];
        for (int i = _indexXStart ; i <= _indexXStop; i++) {
            for (int j = _indexYStart ; j <= _indexYStop; j++) {
                _data[i][j] = receiveInt();
            }
        }
    }

//-------------------------------------------------------------------------------------
    protected void disconnect() {
        closeConnection();
    }


//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
