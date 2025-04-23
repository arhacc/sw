//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.components.flow;

//-------------------------------------------------------------------------------------
import java.net.*;
import java.util.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.tree.*;

import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.context.*;

import xpu.sw.tools.sdk.gui.*;

//-------------------------------------------------------------------------------------
public class Action {
    private String name;
    private int status;

//-------------------------------------------------------------------------------------
    public Action(String _s){
    	this(_s, Flow.STATUS_UNKNOWN);
    }

//-------------------------------------------------------------------------------------
    public Action(String _s, int _status){
    	name = _s;
    	status = _status;
    }

//-------------------------------------------------------------------------------------
    public void setStatus(int _status){
    	status = _status;
    }

//-------------------------------------------------------------------------------------
    public int getStatus(){
    	return status;
    }

//-------------------------------------------------------------------------------------
    public String toString() {
    	return name;
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
