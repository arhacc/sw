//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.components.debugger;

//-------------------------------------------------------------------------------------
//import java.awt.*;
import java.io.*;
import java.nio.file.*;
import java.net.*;
import java.util.*;
import javax.swing.*;
import javax.imageio.*;

import org.apache.commons.configuration2.*;
import org.apache.logging.log4j.*;
import org.apache.logging.log4j.core.*;
import org.apache.logging.log4j.core.appender.*;
import org.apache.logging.log4j.core.config.*;
import org.apache.logging.log4j.core.layout.*;
import org.apache.logging.log4j.core.appender.rolling.*;

import xpu.sw.tools.sdk.common.context.*;

import xpu.sw.tools.sdk.common.io.*;
import xpu.sw.tools.sdk.common.io.targetmanager.*;
import xpu.sw.tools.sdk.common.project.*;

import xpu.sw.tools.sdk.gui.*;
import xpu.sw.tools.sdk.gui.components.*;
import xpu.sw.tools.sdk.gui.components.common.panels.*;
import xpu.sw.tools.sdk.gui.components.debugger.stack.*;
import xpu.sw.tools.sdk.gui.components.debugger.magnifier.*;

//-------------------------------------------------------------------------------------
public class Debugger extends javax.swing.JPanel implements TargetStatusListener {
    private Gui gui;
    private Context context;
    private org.apache.logging.log4j.Logger log;
    private DebugLayer debugLayer;

    private EnhancedJTabbedPane jTabbedPane1;
    private org.apache.commons.configuration2.Configuration sdkConfig;
    private double debugDividerLocation;

//-------------------------------------------------------------------------------------
    public Debugger(Gui _gui, Context _context) {

        gui = _gui;
        context = _context;
        log = _context.getLog();
        initComponents();
        _gui.getDebugger().add(this);
        sdkConfig = context.getSdkConfig();
//        new Thread(this).start();
        init();
    }

//-------------------------------------------------------------------------------------
    private void initComponents() {
        jTabbedPane1 = new EnhancedJTabbedPane(gui, context, "debug.window", this);
        add(jTabbedPane1);
    }


//-------------------------------------------------------------------------------------
    private void init() {
        debugDividerLocation = sdkConfig.getDouble("gui.splitPane5", 0.7);
        if(context.getDebugStatus() == Context.DEBUG_STATUS_ON){
            debugEnter();
        } else {
            debugExit();
        }
        jTabbedPane1.addTab("Magnifier", new Magnifier(gui, context));
        gui.getServices().getTargetManager().addStatusListener(this);
    }
/*
//-------------------------------------------------------------------------------------
    public Component getMagnifier(){
        Component _component = jTabbedPane1.getComponentAt(0);
        jTabbedPane1.removeTabAt(0);
        return _component;
    }
*/
//-------------------------------------------------------------------------------------
    public void debugEnter(){
/*        String _path = gui.getMyComponents().getHierarchy().getSelectedProjectPath();
        if(_path == null){
            _path = gui.getMyComponents().getEditor().getSelectedFilename();
//            _path = Paths.get(_path).getParent().toString();
        }*/
        gui.getDebugSplitter().setDividerLocation(debugDividerLocation);
        context.setDebugStatus(Context.DEBUG_STATUS_ON);
        startDebug();
    }

//-------------------------------------------------------------------------------------
    public void debugExit(){
        debugDividerLocation = context.getSdkConfig().getDouble("gui.splitPane5", Double.NaN);
        gui.getDebugSplitter().setDividerLocation(1.0f);
        context.setDebugStatus(Context.DEBUG_STATUS_OFF);
    }

//-------------------------------------------------------------------------------------
    public void targetStatusChanged(TargetConnection _targetConnection){
        if(debugLayer != null){
            debugLayer.targetStatusChanged(_targetConnection);
        }
    }

//-------------------------------------------------------------------------------------
    public void update(int _command, int _cellIndex, int[] _data){
        switch (_command) {
/*            case Command.UPDATE_MEMORY_DATA : {
//                cells[_cellIndex].updateMemoryData(_data);
                break;
            }
            case Command.UPDATE_REGISTRY_DATA : {
//                cells[_cellIndex].updateRegistryData(_data);
                break;
            }*/
            default: {
                log.error("Unknown command: " + _command);
            }
        }
    }


//-------------------------------------------------------------------------------------
    private void startDebug() {
        Components _components = gui.getMyComponents();
        if(_components == null){
            return;
        }
        Project _currentProject = _components.getHierarchy().getSelectedProject();
        if(_currentProject == null){
            log.error("No project selected!");
            return;
        }
        if((debugLayer == null) || (!_currentProject.equals(debugLayer.getProject()))){
            debugLayer = new DebugLayer(gui, context, _currentProject);
        }
    }

//-------------------------------------------------------------------------------------
    public void afterInit(){
//        menu.afterInit();
//        toolbar.afterInit();
//        hierarchy.afterInit();
//        flow.afterInit();
//        editor.afterInit();
//        terminal.afterInit();
        startDebug();
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
