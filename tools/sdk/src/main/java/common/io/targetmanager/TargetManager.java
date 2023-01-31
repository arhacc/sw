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

import codex.common.apps.rxbasics.*;
import codex.common.wrappers.version.*;

import xpu.sw.tools.sdk.*;
import xpu.sw.tools.sdk.common.context.*;

//-------------------------------------------------------------------------------------
public class TargetManager extends RxStatus {
    private Context context;
    private TargetConnection currentTargetConnection;
    private List<TargetStatusListener> statusListeners;
    private List<TargetConnection> targetConnections;

//-------------------------------------------------------------------------------------
    public TargetManager(Context _context) {
        super(_context.getLog());
        context = _context;
        statusListeners = new ArrayList<TargetStatusListener>();
        targetConnections = new ArrayList<TargetConnection>();
        reloadTargetConnections();
//        setTargetStatus(TargetConnection.STATUS_CONNECTING);
        changeCurrentTargetConnection(targetConnections.get(0));
    }

//-------------------------------------------------------------------------------------
    public void reloadTargetConnections(){
        List<String> _connections = context.getSdkConfig().getList(String.class, "preferences.target");
        targetConnections.clear();
        for(int i = 0; i < _connections.size(); i++){
            TargetConnection _targetConnection = new TargetConnection(context, this, _connections.get(i));
            targetConnections.add(_targetConnection);
            if(_targetConnection.isSelected()){
                currentTargetConnection = _targetConnection;
            }
        }
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
    public void changeCurrentTargetConnection(TargetConnection _targetConnection){
        currentTargetConnection.setSelected(false);
        currentTargetConnection = _targetConnection;
        currentTargetConnection.setSelected(true);
        setTargetStatus(TargetConnection.STATUS_CONNECTING);
    }


//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
