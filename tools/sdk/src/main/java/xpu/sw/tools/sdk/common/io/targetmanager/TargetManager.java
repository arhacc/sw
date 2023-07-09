//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.io.targetmanager;
//-------------------------------------------------------------------------------------
import java.net.*;
import java.io.*;
import java.nio.channels.*;
import java.util.*;
import java.lang.reflect.*;

import org.apache.commons.configuration2.*;
import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.*;
import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.xbasics.*;
import xpu.sw.tools.sdk.common.context.version.*;

//-------------------------------------------------------------------------------------
public class TargetManager extends XStatus {
    private Context context;
    private TargetConnection currentTargetConnection;
    private List<TargetStatusListener> statusListeners;
    private List<TargetConnection> targetConnections;

//-------------------------------------------------------------------------------------
    public TargetManager(Context _context) {
        super(_context);
        context = _context;
        statusListeners = new ArrayList<TargetStatusListener>();
        targetConnections = new ArrayList<TargetConnection>();
        int _selectedId = reloadTargetConnections();
//        setTargetStatus(TargetConnection.STATUS_CONNECTING);
        changeCurrentTargetConnection(targetConnections.get(_selectedId));
    }

//-------------------------------------------------------------------------------------
    public int reloadTargetConnections(){
        int _selectedId = 0;
        List<String> _connections = context.getSdkConfig().getList(String.class, "preferences.target");
        targetConnections.clear();
        for(int i = 0; i < _connections.size(); i++){
            TargetConnection _targetConnection = new TargetConnection(context, this, _connections.get(i));
            targetConnections.add(_targetConnection);
            if(_targetConnection.isSelected()){
                currentTargetConnection = _targetConnection;
                _selectedId = i;
            }
        }
        return _selectedId;
    }

//-------------------------------------------------------------------------------------
    public synchronized void addStatusListener(TargetStatusListener _targetStatusListener){
        statusListeners.add(_targetStatusListener);
        _targetStatusListener.targetStatusChanged(currentTargetConnection);
    }

//-------------------------------------------------------------------------------------
    private synchronized void setTargetStatus(int _status){
        int _oldStatus = currentTargetConnection.getStatus();
        currentTargetConnection.setStatus(_status);
        triggerAllListeners();
    }

//-------------------------------------------------------------------------------------
    public synchronized void triggerAllListeners(){
        statusListeners.forEach(_statusListener -> {
            _statusListener.targetStatusChanged(currentTargetConnection);
        });
    }

//-------------------------------------------------------------------------------------
    private synchronized int getTargetStatus(){
        return currentTargetConnection.getStatus();
    }

//-------------------------------------------------------------------------------------
    public TargetConnection getTargetConnection(){
        return currentTargetConnection;
    }
//-------------------------------------------------------------------------------------
    public List<TargetConnection> getTargetConnections(){
        return targetConnections;
    }

//-------------------------------------------------------------------------------------
    public synchronized void changeCurrentTargetConnection(TargetConnection _targetConnection){
        currentTargetConnection = _targetConnection;
        triggerAllListeners();
/*
        if(currentTargetConnection == null){
            currentTargetConnection = _targetConnection;
        } else if(!_targetConnection.equals(currentTargetConnection)){
            currentTargetConnection.setSelected(false);
            currentTargetConnection = _targetConnection;
            currentTargetConnection.setSelected(true);
//            setTargetStatus(TargetConnection.STATUS_CONNECTING);            
        }*/
    }


//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
