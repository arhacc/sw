package xpu.sw.tools.sdk.simulator.stack;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

public class NetworkUtil {
    public DataInputStream inStream;
    public DataOutputStream outStream;
    public int status;

    public static final int ERROR_VALUE = -1;

    public static final int STATUS_INIT 	        = 0;
    public static final int STATUS_RUNNING          = 1;
    public static final int STATUS_DISCONNECTED     = 2;
    public static final int STATUS_STOPPED 	        = 3;

    //-------------------------------------------------------------------------------------
    public void sendByte(byte _b) {
        try{
            outStream.writeByte(_b);
        }catch(IOException _e){
            status = STATUS_DISCONNECTED;
        }
    }

    //-------------------------------------------------------------------------------------
    public byte receiveByte() {
        byte _b = -1;
        try{
            _b = inStream.readByte();
        }catch(IOException _e){
            status = STATUS_DISCONNECTED;
        }
        return _b;
    }

    //-------------------------------------------------------------------------------------
    public int receiveInt() {
        try{
            return inStream.readInt();
        }catch(IOException _e){
            status = STATUS_DISCONNECTED;
            return ERROR_VALUE;
        }
    }

    //-------------------------------------------------------------------------------------
    public void sendInt(int _i) {
        try{
            outStream.writeInt(_i);
        }catch(IOException _e){
            status = STATUS_DISCONNECTED;
        }
    }

    //-------------------------------------------------------------------------------------
    public int[] receiveIntArray(int _dataLength) {
        int data[] = new int[_dataLength];
        for(int i = 0; i < _dataLength; i++){
            try{
                data[i] = inStream.readInt();
            }catch(IOException _e){
                status = STATUS_DISCONNECTED;
                return null;
            }
        }
        return data;
    }

    //-------------------------------------------------------------------------------------
    public void sendIntArray(int[] _data) {
        for(int i = 0; i < _data.length; i++){
            try{
                outStream.writeInt(_data[i]);
            }catch(IOException _e){
                status = STATUS_DISCONNECTED;
            }
        }
    }

    //-------------------------------------------------------------------------------------
    public void sendLongArray(long[] _data) {
        for(int i = 0; i < _data.length; i++){
            try{
                outStream.writeLong(_data[i]);
            }catch(IOException _e){
                status = STATUS_DISCONNECTED;
            }
        }
    }

    //-------------------------------------------------------------------------------------
    public long[] receiveLongArray(int _dataLength) {
        long data[] = new long[_dataLength];
        for(int i = 0; i < _dataLength; i++){
            try{
                data[i] = inStream.readLong();
            }catch(IOException _e){
                status = STATUS_DISCONNECTED;
                return null;
            }
        }
        return data;
    }
}
