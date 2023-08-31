//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.io.targetmanager;
//-------------------------------------------------------------------------------------

import xpu.sw.tools.sdk.common.xbasics.XStatus;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import xpu.sw.tools.sdk.common.context.Context;

//-------------------------------------------------------------------------------------
public class TargetConnection extends XStatus {

    private Context context;
    private TargetManager targetManager;
    private int id;
    private String name;
    private String type;
    private String host;
    private int port;
    private int status;
    private boolean selected;

    private DataInputStream inputStream;
    private DataOutputStream outputStream;
    private Object lock;

    public static final int STATUS_INIT = 0;
    public static final int STATUS_CONNECTING = 1;
    public static final int STATUS_CONNECTED = 2;
    public static final int STATUS_FAILED = 3;

//-------------------------------------------------------------------------------------
    public TargetConnection(Context _context, TargetManager _targetManager, String _data) {
        super(_context);
        context = _context;
        targetManager = _targetManager;
//        log.debug("_data="+_data);
        String[] _dataArray = _data.split(",");
        id = Integer.parseInt(_dataArray[0]);
        name = _dataArray[2];
        type = _dataArray[3];
        host = _dataArray[4];
        port = Integer.parseInt(_dataArray[5]);
        selected = (_dataArray.length <= 7) ? false : (_dataArray[7].trim().equals("selected"));//(id == 0);
        if (selected) {
            log.debug("Target connection " + this + " is selected!");
        }
        status = STATUS_INIT;
        lock = new Object();
        setRunning();
        start();
    }

//-------------------------------------------------------------------------------------
    @Override
    public void run() {
        while (isNotStopped()) {
            if (getStatus() == STATUS_CONNECTING) {
                try {
//                    log.debug("> Connecting to " + host + ":" + port + "...");
                    Socket _socket = new Socket(host, port);
                    inputStream = new DataInputStream(_socket.getInputStream());
                    outputStream = new DataOutputStream(_socket.getOutputStream());
                    setStatus(STATUS_CONNECTED);
                } catch (IOException _e) {
                    inputStream = null;
                    outputStream = null;
                    setStatus(STATUS_CONNECTING);
                    delay(2000);
                }
            } else {
                delay(1000);
            }
        }
    }

//-------------------------------------------------------------------------------------
    public int getId() {
        return id;
    }

//-------------------------------------------------------------------------------------
    public boolean equals(TargetConnection _targetConnection) {
        if (_targetConnection.id != id) {
            return false;
        }
        if (!_targetConnection.name.equals(name)) {
            return false;
        }
        if (!_targetConnection.type.equals(type)) {
            return false;
        }
        if (!_targetConnection.host.equals(host)) {
            return false;
        }
        return _targetConnection.port == port;
    }

//-------------------------------------------------------------------------------------
    @Override
    public synchronized int getStatus() {
        return status;
    }

//-------------------------------------------------------------------------------------
    @Override
    public synchronized void setStatus(int _status) {
        boolean _change = (status != _status);
        status = _status;
        if (_change) {
            targetManager.triggerAllListeners();
        }
    }

//-------------------------------------------------------------------------------------
    public String getName() {
        return name;
    }

//-------------------------------------------------------------------------------------
    public String getPath() {
        return host + ":" + port;
    }

//-------------------------------------------------------------------------------------
    public String getDescriptor() {
//        return id + " : " + getName() + " [" + getPath() + "]";
        return getName() + " [" + getPath() + "]";
    }

//-------------------------------------------------------------------------------------
    public synchronized boolean isConnected() {
        return (status == STATUS_CONNECTED);
    }

//-------------------------------------------------------------------------------------
    public synchronized boolean isSelected() {
        return selected;
    }

//-------------------------------------------------------------------------------------
    public synchronized void setSelected(boolean _selected) {
        boolean _change = (selected != _selected);
        selected = _selected;
        if (_change) {
            targetManager.triggerAllListeners();
        }
    }

//-------------------------------------------------------------------------------------
    public void sendByte(byte _b) {
        try {
            outputStream.writeByte(_b);
        } catch (Exception _e) {
            setStatus(STATUS_CONNECTING);
        }
    }

//-------------------------------------------------------------------------------------
    public void sendInt(int _i) {
        try {
            outputStream.writeInt(_i);
        } catch (Exception _e) {
            setStatus(STATUS_CONNECTING);
        }
    }

//-------------------------------------------------------------------------------------
    public void sendLong(long _l) {
        try {
            outputStream.writeLong(_l);
        } catch (Exception _e) {
            setStatus(STATUS_CONNECTING);
        }
    }

//-------------------------------------------------------------------------------------
    public void sendByteArray(byte[] _data) {
        try {
            for (int i = 0; i < _data.length; i++) {
                outputStream.writeByte(_data[i]);
            }
        } catch (Exception _e) {
            setStatus(STATUS_CONNECTING);
        }
    }

//-------------------------------------------------------------------------------------
    public void sendIntArray(int[] _data) {
        for (int i = 0; i < _data.length; i++) {
            try {
                outputStream.writeInt(_data[i]);
            } catch (Exception _e) {
                setStatus(STATUS_CONNECTING);
                break;
            }
        }
    }

//-------------------------------------------------------------------------------------
    public void sendLongArray(long[] _data) {
        for (int i = 0; i < _data.length; i++) {
            try {
                outputStream.writeLong(_data[i]);
            } catch (Exception _e) {
                setStatus(STATUS_CONNECTING);
                break;
            }
        }
    }

//-------------------------------------------------------------------------------------
    public byte[] receiveByteArray(int _dataLength) {
        byte[] _data = new byte[_dataLength];
        for (int i = 0; i < _dataLength; i++) {
            try {
                _data[i] = inputStream.readByte();
            } catch (Exception _e) {
                setStatus(STATUS_CONNECTING);
                return null;
            }
        }
        return _data;
    }

//-------------------------------------------------------------------------------------

    public long[] receiveLongArray(int _dataLength) {
        long[] _data = new long[_dataLength];
        for (int i = 0; i < _dataLength; i++) {
            try {
                _data[i] = inputStream.readLong();
            } catch (Exception _e) {
                setStatus(STATUS_CONNECTING);
                return null;
            }
        }
        return _data;
    }
//-------------------------------------------------------------------------------------

    public byte receiveByte() {
        byte _b = -1;
        try {
            _b = inputStream.readByte();
        } catch (Exception _e) {
            setStatus(STATUS_CONNECTING);
        }
        return _b;
    }

//-------------------------------------------------------------------------------------
    public int receiveInt() {
        int _b = -1;
        try {
            _b = inputStream.readInt();
        } catch (Exception _e) {
            setStatus(STATUS_CONNECTING);
        }
        return _b;
    }

//-------------------------------------------------------------------------------------
    @Override
    public String toString() {
        return name + "[" + host + ":" + port + "]";
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
