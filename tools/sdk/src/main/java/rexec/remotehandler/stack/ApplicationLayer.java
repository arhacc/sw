//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.rexec.remotehandler.stack;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.nio.file.*;
import java.nio.channels.*;
import java.security.*;
import java.util.*;
import java.util.stream.*;

import org.apache.commons.configuration2.*;
import org.apache.logging.log4j.*;

import org.apache.commons.cli.*;
import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;

import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.tree.*;

import codex.common.utils.*;

import xpu.sw.tools.sdk.*;
import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.project.*;
import xpu.sw.tools.sdk.common.fileformats.hex.*;
import xpu.sw.tools.sdk.common.fileformats.obj.*;
import xpu.sw.tools.sdk.common.fileformats.onnx.*;
import xpu.sw.tools.sdk.common.io.*;
import xpu.sw.tools.sdk.common.io.targetmanager.*;
import xpu.sw.tools.sdk.asm.parser.*;

//-------------------------------------------------------------------------------------
public class ApplicationLayer extends CommandLayer {
    private MessageDigest md5Digest;

//-------------------------------------------------------------------------------------
    public ApplicationLayer(Context _context, TargetManager _targetManager) {
        super(_context, _targetManager);
        try{
            md5Digest = MessageDigest.getInstance("MD5");            
        } catch(NoSuchAlgorithmException _e){
            log.error("Error: NoSuchAlgorithmException in ApplicationLayer");
            System.exit(0);
        }
    }

//-------------------------------------------------------------------------------------
    protected void send(ObjFile _objFile) {
/*        while(status != STATUS_RUNNING){
            try{
                sleep(1000);
            }catch(InterruptedException _e){

            }
        }*/
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
        sendInt(Command.COMMAND_RUN_FILE_ONNX);
        String _path = _onnxFile.getPath();
        try{
            FileInputStream _fileInputStream = new FileInputStream(_path);
            FileChannel _fileChannel = _fileInputStream.getChannel();
            byte[] _md5 = getMD5(_path);
            String _md5Hex = codex.common.utils.StringUtils.bytesToHex(_md5).toLowerCase();
            log.debug("Send onnx file MD5: [" + _md5Hex + "]...");
            sendByteArray(_md5);
            int _response = receiveInt();
            if(_response == Command.COMMAND_DONE){
                log.debug("File already exists!");
                return;
            } else if(_response == Command.COMMAND_ERROR){
                log.debug("Send onnx file error!");
                return;
            } else if(_response == Command.COMMAND_RETRY){
                long _length = _fileChannel.size();
                log.debug("Send onnx file: length = " + _length + "...");
                sendLong(_length);
                for(int i = 0; i < _length; i++){
                    sendByte((byte)_fileInputStream.read());
                }            
            }
        } catch(IOException _e){
            log.error("Cannot send file to remote: " + _onnxFile);
        }
/*        Path _path = Paths.get(_onnxFile.getPath());
        BufferedReader _reader = Files.newBufferedReader(_path);
        String line = reader.readLine();*/

    }

//-------------------------------------------------------------------------------------
    protected void disconnect() {
        closeConnection();
    }

//-------------------------------------------------------------------------------------
    private byte[] getMD5(String _path) throws IOException {
      //Get file input stream for reading the file content
        File _file = new File(_path);
        FileInputStream _fis = new FileInputStream(_file);
       
      //Create byte array to read data in chunks
      byte[] _byteArray = new byte[1024];
      int _bytesCount = 0; 
        
      //Read file data and update in message digest
      while ((_bytesCount = _fis.read(_byteArray)) != -1) {
        md5Digest.update(_byteArray, 0, _bytesCount);
      };
       
      //close the stream; We don't need it now.
      _fis.close();
       
      //Get the hash's bytes
      return md5Digest.digest();
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
