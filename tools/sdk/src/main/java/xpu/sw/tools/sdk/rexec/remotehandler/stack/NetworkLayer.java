//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.rexec.remotehandler.stack;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.net.*;

import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.io.*;
import xpu.sw.tools.sdk.common.io.targetmanager.*;

//-------------------------------------------------------------------------------------
public class NetworkLayer implements TargetStatusListener {
    protected Context context;
    protected Logger log;

    protected TargetManager targetManager;
    protected TargetConnection currentTargetConnection;

//-------------------------------------------------------------------------------------
    public NetworkLayer(Context _context, TargetManager _targetManager) {
        context = _context;
        log = _context.getLog();
        targetManager = _targetManager;
        if(targetManager == null){
            targetManager = new TargetManager(_context);
        }
        targetManager.addStatusListener(this);
//        currentTargetConnection
    }

//-------------------------------------------------------------------------------------
    public boolean isConnected() {
        return currentTargetConnection.isConnected();
    }

//-------------------------------------------------------------------------------------
    public void targetStatusChanged(TargetConnection _connection) {
        currentTargetConnection = _connection;
    }

//-------------------------------------------------------------------------------------
    protected void sendHello() throws IOException {
        log.debug("HELLO");
        sendInt(Command.COMMAND_OPEN_CONNECTION);
        byte _response = receiveByte();
        if(_response != Command.COMMAND_DONE){
            throw new IOException("Error sending code segment to remote: " + _response);
        }
    }

//-------------------------------------------------------------------------------------
    protected void sendByte(byte _b) {
        currentTargetConnection.sendByte(_b);
    }

//-------------------------------------------------------------------------------------
    protected void sendByte(int _i) {
        currentTargetConnection.sendByte((byte)_i);
    }

//-------------------------------------------------------------------------------------
    protected void sendInt(int _i) {
        currentTargetConnection.sendInt(_i);
    }

//-------------------------------------------------------------------------------------
    protected void sendLong(long _l) {
        currentTargetConnection.sendLong(_l);
    }

//-------------------------------------------------------------------------------------
    protected void sendByteArray(byte[] _data) {
        currentTargetConnection.sendByteArray(_data);
    }

//-------------------------------------------------------------------------------------
    protected void sendIntArray(int[] _data) {
        currentTargetConnection.sendIntArray(_data);
    }

//-------------------------------------------------------------------------------------
    protected void sendLongArray(long[] _data) {
        currentTargetConnection.sendLongArray(_data);
    }

//-------------------------------------------------------------------------------------
    public Long[] receiveLongArray(int _dataLength) {
        return currentTargetConnection.receiveLongArray(_dataLength);
    }
//-------------------------------------------------------------------------------------
    protected byte receiveByte() {
        return currentTargetConnection.receiveByte();
    }

//-------------------------------------------------------------------------------------
    protected int receiveInt() {
        return currentTargetConnection.receiveInt();
    }

//-------------------------------------------------------------------------------------

}
//-------------------------------------------------------------------------------------
