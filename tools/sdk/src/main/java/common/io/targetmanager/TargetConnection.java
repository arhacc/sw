//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.io.targetmanager;
//-------------------------------------------------------------------------------------
import java.awt.*;
import java.awt.event.*;
import java.net.*;
import java.io.*;
import java.nio.channels.*;
import java.util.*;
import javax.swing.*;
import java.lang.reflect.*;

import org.apache.commons.configuration2.*;
import org.apache.logging.log4j.*;

import codex.common.apps.rxbasics.*;
import codex.common.wrappers.version.*;

import xpu.sw.tools.sdk.*;
import xpu.sw.tools.sdk.common.context.*;

//-------------------------------------------------------------------------------------
public class TargetConnection extends RxStatus {
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

    public static final int STATUS_INIT             = 0;
    public static final int STATUS_CONNECTING       = 1;
    public static final int STATUS_CONNECTED        = 2;
    public static final int STATUS_FAILED           = 3;

//-------------------------------------------------------------------------------------
    public TargetConnection(Context _context, TargetManager _targetManager, String _data) {
        super(_context.getLog());
        context = _context;
        targetManager = _targetManager;
//        log.debug("_data="+_data);
        String[] _dataArray = _data.split(",");
        id = Integer.parseInt(_dataArray[0]);
        name  = _dataArray[1];
        type = _dataArray[2];
        host = _dataArray[3];
        port = Integer.parseInt(_dataArray[4]);
        status = STATUS_INIT;
        selected = (id == 0);
        lock = new Object();
        setRunning();
        start();
    }

//-------------------------------------------------------------------------------------
    public void run(){
        while(isNotStopped()){
//            if(isSelected()){
                if((inputStream == null) || (outputStream == null)){
                    try{
                        log.debug("> Connecting to " + host + ":" + port + "...");
                        Socket _socket = new Socket(host, port);
                        inputStream = new DataInputStream(_socket.getInputStream());
                        outputStream = new DataOutputStream(_socket.getOutputStream());
                        setStatus(STATUS_CONNECTED);
                    } catch(Exception _e){
                        inputStream = null;
                        outputStream = null;
//                        log.error("Cannot create connection to " + host + ":" + port + "...");
//                        setStatus(STATUS_FAILED);
                        setStatus(STATUS_CONNECTING);
                        delay(3000);
                    }
                } else {
//                    delay(1500);
//                    try {
//                        synchronized(lock){
//                            lock.wait(10000000);
                            delay(100);
//                        }
/*                    }catch(InterruptedException _e){

                    }*/
                }
        }
    }

//-------------------------------------------------------------------------------------
    public int getId(){
        return id;
    }

//-------------------------------------------------------------------------------------
    public boolean equals(TargetConnection _targetConnection){
        if(_targetConnection.id != id){
            return false;
        }
        if(!_targetConnection.name.equals(name)){
            return false;
        }
        if(!_targetConnection.type.equals(type)){
            return false;
        }
        if(!_targetConnection.host.equals(host)){
            return false;
        }
        if(_targetConnection.port != port){
            return false;
        }
        return true;
    }

//-------------------------------------------------------------------------------------
    public synchronized int getStatus(){
        return status;
    }

//-------------------------------------------------------------------------------------
    public synchronized void setStatus(int _status){
        boolean _change = (status != _status);
        status = _status;
        if(_change){
            targetManager.triggerAllListeners();            
        }
    }

//-------------------------------------------------------------------------------------
    public String getName(){
        return name;
    }

//-------------------------------------------------------------------------------------
    public String getPath(){
        return host + ":" + port;
    }

//-------------------------------------------------------------------------------------
    public String getDescriptor(){
//        return id + " : " + getName() + " [" + getPath() + "]";
        return getName() + " [" + getPath() + "]";
    }

//-------------------------------------------------------------------------------------
    public synchronized boolean isSelected(){
        return selected;
    }

//-------------------------------------------------------------------------------------
    public synchronized void setSelected(boolean _selected){
        boolean _change = (selected != _selected);
        selected = _selected;
        if(_change){
            targetManager.triggerAllListeners();            
        }

/*        if(_selected){ 
           setRunning(); 
           interrupt();
        } else {
            setPaused();
        }*/
    }

//-------------------------------------------------------------------------------------
    public void sendByte(byte _b) {
        try{
            outputStream.writeByte(_b);
        }catch(Throwable _e){
            status = STATUS_CONNECTING;
        }
    }

//-------------------------------------------------------------------------------------
    public void sendInt(int _i) {
        try{
            outputStream.writeInt(_i);
        }catch(Throwable _e){
            status = STATUS_CONNECTING;
        }
    }

//-------------------------------------------------------------------------------------
    public void sendLong(long _l) {
        try{
            outputStream.writeLong(_l);
        }catch(Throwable _e){
            status = STATUS_CONNECTING;
        }
    }

//-------------------------------------------------------------------------------------
    public void sendByteArray(byte[] _data) {
        try{
            for(int i = 0; i < _data.length; i++){
                outputStream.writeByte(_data[i]);
            }            
        }catch(Throwable _e){
            status = STATUS_CONNECTING;
        }
    }

//-------------------------------------------------------------------------------------
    public void sendIntArray(int[] _data) {
        for(int i = 0; i < _data.length; i++){
            try{
                outputStream.writeInt(_data[i]);
            }catch(Throwable _e){
                status = STATUS_CONNECTING;
            }
        }
    }

//-------------------------------------------------------------------------------------
    public void sendLongArray(long[] _data) {
        for(int i = 0; i < _data.length; i++){
            try{
                outputStream.writeLong(_data[i]);
            }catch(Throwable _e){
                status = STATUS_CONNECTING;
            }
        }
    }
//-------------------------------------------------------------------------------------
    public Long[] receiveLongArray(int _dataLength) {
        Long data[] = new Long[_dataLength];
        for(int i = 0; i < _dataLength; i++){
            try{
                data[i] = inputStream.readLong();
            }catch(Throwable _e){
                status = STATUS_CONNECTING;
                return null;
            }
        }
        return data;
    }
//-------------------------------------------------------------------------------------
    public byte receiveByte() {
        byte _b = -1;
        try{
            _b = inputStream.readByte();
        }catch(Throwable _e){
            status = STATUS_CONNECTING;
        }
        return _b;
    }

//-------------------------------------------------------------------------------------
    public int receiveInt() {
        int _b = -1;
        try{
            _b = inputStream.readInt();
        }catch(Throwable _e){
            status = STATUS_CONNECTING;
        }
        return _b;
    }
    
//-------------------------------------------------------------------------------------
    public String toString() {
        return name +"[" + host +":" + port;
    }
    
//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
