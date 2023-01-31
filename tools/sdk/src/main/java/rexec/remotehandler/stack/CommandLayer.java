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

import xpu.sw.tools.sdk.*;
import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.project.*;
import xpu.sw.tools.sdk.common.fileformats.hex.*;
import xpu.sw.tools.sdk.common.fileformats.obj.*;
import xpu.sw.tools.sdk.common.io.*;
import xpu.sw.tools.sdk.common.io.targetmanager.*;
import xpu.sw.tools.sdk.asm.parser.*;

//-------------------------------------------------------------------------------------
public class CommandLayer extends NetworkLayer {

//-------------------------------------------------------------------------------------
    public CommandLayer(Context _context, TargetManager _targetManager) {
        super(_context, _targetManager);
    }

//-------------------------------------------------------------------------------------
    protected void sendFeatures(ObjSegment _featureSegment) {
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
    protected void sendCode(ObjSegment _codeSegment) {
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
    protected void sendData(ObjSegment _dataSegment) {
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
            Long[] incomingData = receiveLongArray(incomingDataSize);
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
}
//-------------------------------------------------------------------------------------
