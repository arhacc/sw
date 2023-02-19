//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.components.debugger.stack;
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
import xpu.sw.tools.sdk.common.io.*;
import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.project.*;
import xpu.sw.tools.sdk.common.fileformats.abstractexecutable.*;
import xpu.sw.tools.sdk.asm.parser.*;

//-------------------------------------------------------------------------------------
public class CommandLayer extends NetworkLayer {


//-------------------------------------------------------------------------------------
    public CommandLayer(Context _context) {
        super(_context);
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
