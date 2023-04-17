//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.components.debugger.stack;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.net.*;
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
import xpu.sw.tools.sdk.common.io.targetmanager.*;

//-------------------------------------------------------------------------------------
public class NetworkLayer extends Thread {
    protected Context context;
    protected Logger log;

    protected DataInputStream   inStream;
    protected DataOutputStream  outStream;
    protected int status;

    protected TargetConnection targetConnection;

    public static final int STATUS_INIT 	        = 0;
    public static final int STATUS_RUNNING          = 1;
    public static final int STATUS_DISCONNECTED     = 2;
    public static final int STATUS_STOPPED 	        = 3;

//-------------------------------------------------------------------------------------
    public NetworkLayer(Context _context) {
        context = _context;
        log = _context.getLog();
        status = STATUS_INIT;
    }

//-------------------------------------------------------------------------------------
    protected void sendByte(byte _b) {
        try{
            outStream.writeByte(_b);
        }catch(IOException _e){
            status = STATUS_DISCONNECTED;
        }
    }

//-------------------------------------------------------------------------------------
    protected void sendInt(int _i) {
        try{
            outStream.writeInt(_i);
        }catch(IOException _e){
            status = STATUS_DISCONNECTED;
        }
    }

//-------------------------------------------------------------------------------------
    protected void sendIntArray(int[] _data) {
        for(int i = 0; i < _data.length; i++){
            try{
                outStream.writeInt(_data[i]);
            }catch(IOException _e){
                status = STATUS_DISCONNECTED;
            }
        }
    }

//-------------------------------------------------------------------------------------
    protected void sendLongArray(long[] _data) {
        for(int i = 0; i < _data.length; i++){
            try{
                outStream.writeLong(_data[i]);
            }catch(IOException _e){
                status = STATUS_DISCONNECTED;
            }
        }
    }

//-------------------------------------------------------------------------------------
    protected byte receiveByte() {
        byte _b = -1;
        try{
            _b = inStream.readByte();
        }catch(IOException _e){
            status = STATUS_DISCONNECTED;
        }
        return _b;
    }

//-------------------------------------------------------------------------------------
    public void targetStatusChanged(TargetConnection _targetConnection){
        targetConnection = _targetConnection;
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
