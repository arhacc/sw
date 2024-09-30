//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.components.debugger;
//-------------------------------------------------------------------------------------
import java.awt.*;
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
import xpu.sw.tools.sdk.common.context.arch.*;
import xpu.sw.tools.sdk.common.project.*;

import xpu.sw.tools.sdk.common.io.*;
import xpu.sw.tools.sdk.common.io.targetmanager.*;
import xpu.sw.tools.sdk.common.project.*;

import xpu.sw.tools.sdk.gui.*;
import xpu.sw.tools.sdk.gui.components.*;
import xpu.sw.tools.sdk.gui.components.common.*;
import xpu.sw.tools.sdk.gui.components.common.panels.*;
//import xpu.sw.tools.sdk.gui.components.debugger.stack.*;
import xpu.sw.tools.sdk.gui.components.debugger.magnifier.*;

//-------------------------------------------------------------------------------------
public class Debugger extends GuiPanel implements TargetStatusListener {
    private Project activeProject;
    private Map<Project, DebuggerByProject> debuggerByProjects;
    private CardLayout cardLayout;

    private org.apache.commons.configuration2.Configuration sdkConfig;
    private double debugDividerLocation;
    private int viewDataMode;

    public final static int VIEW_DATA_MODE_HEX = 0;
    public final static int VIEW_DATA_MODE_DEC = 1;

//-------------------------------------------------------------------------------------
    public Debugger(Gui _gui, Context _context) {
        super(_context, _gui);
        initComponents();
        _gui.getDebugger().add(this);
        sdkConfig = context.getSdkConfig();
//        new Thread(this).start();
        init();
    }

//-------------------------------------------------------------------------------------
    private void initComponents() {
//        jTabbedPane1 = new EnhancedJTabbedPane(gui, context, "debug.window", this);
        cardLayout = new CardLayout();
        setLayout(cardLayout);
    }

//-------------------------------------------------------------------------------------
    private void init() {
        String _viewInspectorValue = context.getSdkConfig().getString("view.inspector", "hex");
        viewDataMode = (_viewInspectorValue.equals("hex")) ? VIEW_DATA_MODE_HEX : VIEW_DATA_MODE_DEC;

        debuggerByProjects = new HashMap<Project, DebuggerByProject>();
        debugDividerLocation = sdkConfig.getDouble("gui.splitPane5", 0.7);
        if(context.getDebugMode() == Context.DEBUG_MODE_ON){
            if(debugDividerLocation == 1.0){
                debugDividerLocation = 0.9;
            }
            debugEnter();
        } else {
            debugExit();
        }
//        magnifier = new Magnifier(gui, context, architectureImplementation);
//        jTabbedPane1.addTab("Magnifier", magnifier);
        gui.getServices().getTargetManager().addStatusListener(this);
    }

//-------------------------------------------------------------------------------------
    public void setViewDataMode(int _viewDataMode){
//        log.debug("_viewDataMode=" + _viewDataMode);
        viewDataMode = _viewDataMode;
        String _viewDataModeAsString = (_viewDataMode == VIEW_DATA_MODE_HEX) ? "hex" : "dec";
        context.getSdkConfig().setProperty("view.inspector", _viewDataModeAsString);        
        refresh();
    }

//-------------------------------------------------------------------------------------
    public int getViewDataMode(){
        return viewDataMode;
    }

//-------------------------------------------------------------------------------------
    public void debugEnter(){
/*        String _path = gui.getMyComponents().getHierarchy().getSelectedProjectPath();
        if(_path == null){
            _path = gui.getMyComponents().getEditor().getSelectedFilename();
//            _path = Paths.get(_path).getParent().toString();
        }*/
        gui.getDebugSplitter().setDividerLocation(debugDividerLocation);
        context.setDebugMode(Context.DEBUG_MODE_ON);
        startDebug();
    }

//-------------------------------------------------------------------------------------
    public void debugExit(){
        debugDividerLocation = context.getSdkConfig().getDouble("gui.splitPane5", Double.NaN);
        gui.getDebugSplitter().setDividerLocation(1.0f);
        context.setDebugMode(Context.DEBUG_MODE_OFF);
    }

//-------------------------------------------------------------------------------------
    public void targetStatusChanged(TargetConnection _targetConnection){

    }

//-------------------------------------------------------------------------------------
    private void startDebug() {

    }

//-------------------------------------------------------------------------------------
    public void setActiveProject(Project _activeProject){
        activeProject = _activeProject;
//        log.debug("Debugger: set active project: " + _activeProject);
        DebuggerByProject _debuggerByProject = debuggerByProjects.get(activeProject);
//        log.debug("_debuggerByProject: " + _debuggerByProject);
        if(_debuggerByProject == null){
            _debuggerByProject = new DebuggerByProject(gui, context, _activeProject, this);
            debuggerByProjects.put(_activeProject, _debuggerByProject);
            add(_debuggerByProject.toString(), _debuggerByProject);
        }
        cardLayout.show(this, _debuggerByProject.toString());
    }

//-------------------------------------------------------------------------------------
    public void refresh(){
        if(activeProject == null){
            log.debug("Debugger: no active project set[0]!");
            return;
        }
        DebuggerByProject _debuggerByProject = debuggerByProjects.get(activeProject);
        if(_debuggerByProject == null){
            log.debug("Debugger: no active project set[1]!( activeProject=" + activeProject + ")");
            return;
        }
        _debuggerByProject.refresh();
    }

//-------------------------------------------------------------------------------------
    public void afterInit(){
        startDebug();
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
