//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.rexec.remotehandler.stack;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.nio.file.*;
import java.nio.channels.*;
import java.util.*;
import java.util.stream.*;
import java.security.*;

import org.apache.commons.configuration2.*;
import org.apache.logging.log4j.*;

import org.apache.commons.cli.*;
import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;

import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.tree.*;

import xpu.sw.tools.sdk.*;
import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.project.*;
import xpu.sw.tools.sdk.common.fileformats.core.*;
import xpu.sw.tools.sdk.common.fileformats.hex.*;
import xpu.sw.tools.sdk.common.fileformats.abstractexecutable.*;
import xpu.sw.tools.sdk.common.io.*;
import xpu.sw.tools.sdk.common.io.targetmanager.*;
import xpu.sw.tools.sdk.asm.parser.*;

//-------------------------------------------------------------------------------------
public class CommandLayer extends NetworkLayer {
    private MessageDigest md5Digest;

//-------------------------------------------------------------------------------------
    public CommandLayer(Context _context, TargetManager _targetManager) {
        super(_context, _targetManager);
        try{
            md5Digest = MessageDigest.getInstance("MD5");            
        } catch(NoSuchAlgorithmException _e){
            log.error("Error: NoSuchAlgorithmException in ApplicationLayer");
            System.exit(1);
        }
    }

//-------------------------------------------------------------------------------------
    public void targetStatusChanged(TargetConnection _connection) {
        super.targetStatusChanged(_connection);
        if(currentTargetConnection.isConnected()){
            sendInt(Command.COMMAND_GET_ARCHITECTURE_ID);
            String _architectureId = "xpu_" + HexFormat.of().toHexDigits(receiveLong()).toUpperCase() + HexFormat.of().toHexDigits(receiveLong()).toUpperCase();
            currentTargetConnection.setArchitectureId(_architectureId);
            log.debug("Remote architectureId: " + _architectureId);
        }
    }

//-------------------------------------------------------------------------------------
    protected void sendFile(XpuFile _xpuFile) {
        sendFile(_xpuFile.getPath());
    }

//-------------------------------------------------------------------------------------
    protected void sendFile(String _path) {
        try{
//            sendFilename(_path);
            FileInputStream _fileInputStream = new FileInputStream(_path);
            FileChannel _fileChannel = _fileInputStream.getChannel();
            byte[] _md5 = getMD5(_path);
            String _md5Hex = xpu.sw.tools.sdk.common.utils.StringUtils.bytesToHex(_md5).toLowerCase();
            log.debug("Send file: [" + _path + "]/[" + _md5Hex + "]...");
/*
//            sendByteArray(_md5);
            int _response = receiveInt();
            if(_response == Command.COMMAND_DONE){
                log.debug("File already exists!");
                return;
            } else if(_response == Command.COMMAND_ERROR){
                log.debug("Send file error!");
                return;
            } else if(_response == Command.COMMAND_RETRY){*/
                long _length = _fileChannel.size();
                log.debug("Send file: length = " + _length + "...");
                sendLong(_length);
                for(int i = 0; i < _length; i++){
                    sendByte((byte)_fileInputStream.read());
                }            
//            }
        } catch(IOException _e){
            log.error("Cannot send file to remote: " + _path);
        }
/*        Path _path = Paths.get(_onnxFile.getPath());
        BufferedReader _reader = Files.newBufferedReader(_path);
        String line = reader.readLine();*/

    }

//-------------------------------------------------------------------------------------
    protected void sendFilename(String _path) {
        String _filename = Paths.get(_path).getFileName().toString();
        sendString(_filename);
    }

//-------------------------------------------------------------------------------------
    protected void sendFeatures(AbstractSegment _featureSegment) {
        log.debug("CODE address=" + _featureSegment.getAddress() + ", length=" + _featureSegment.getLength());
        sendInt(Command.COMMAND_LOAD_FEATURE_MEMORY);
        sendInt(_featureSegment.getAddress());
        sendInt(_featureSegment.getLength());
        sendLongArray(_featureSegment.getData());
        byte _response = receiveByte();
        if(_response != Command.COMMAND_DONE){
            log.error("Error sending feature segment to remote: " + _response);
        }
    }
    
//-------------------------------------------------------------------------------------
    protected void sendCode(AbstractSegment _codeSegment) {
        log.debug("CODE address=" + _codeSegment.getAddress() + ", length=" + _codeSegment.getLength());
        sendInt(Command.COMMAND_LOAD_CODE_MEMORY);
        sendInt(_codeSegment.getAddress());
        sendInt(_codeSegment.getLength());
        sendLongArray(_codeSegment.getData());
        byte _response = receiveByte();
        if(_response != Command.COMMAND_DONE){
            log.error("Error sending code segment to remote: " + _response);
        }
    }

//-------------------------------------------------------------------------------------
    protected void sendData(AbstractSegment _dataSegment) {
        log.debug("DATA address=" + _dataSegment.getAddress() + ", length=" + _dataSegment.getLength());
        sendInt(Command.COMMAND_LOAD_DATA_MEMORY);
        sendInt(_dataSegment.getAddress());
        sendInt(_dataSegment.getLength());
        sendLongArray(_dataSegment.getData());
        byte _response = receiveByte();
        if(_response != Command.COMMAND_DONE){
            log.error("Error sending data segment to remote: " + _response);
        } else {
            log.debug("Sending data segment to remote: ok");
            log.debug("Readback data segment from remote...");

            sendInt(Command.COMMAND_UNLOAD_DATA_MEMORY);
            sendInt(_dataSegment.getAddress());
            sendInt(_dataSegment.getLength());
            byte _readbackResponse = receiveByte();
            Integer incomingDataSize = receiveInt();
            long[] incomingData = receiveLongArray(incomingDataSize);
            if(_readbackResponse != Command.COMMAND_DONE)
                log.error("Error reading back of data segment from remote: " + _response);

            if (incomingDataSize != _dataSegment.getLength())
                log.error("Error reading back of data segment from remote (different size) ");
            else {
                boolean result = true;
                for (int i = 0; i < incomingDataSize; i++) {
                    if (incomingData[i] != _dataSegment.getData()[i]) {
                        result = false;
                        log.error("Error readingback data segment from remote (different content) ");
                        break;
                    }
                }
                if (result == true)
                    log.debug("data readback ok");
            }
        }
    }

//-------------------------------------------------------------------------------------
    protected void closeConnection() {
        sendInt(Command.COMMAND_CLOSE_CONNECTION);
        byte _response = receiveByte();
        if(_response != Command.COMMAND_DONE){
            log.error("Error closing connection: " + _response);
        }
    }

//-------------------------------------------------------------------------------------
    protected byte[] getMD5(String _path) throws IOException {
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
