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
        debuggerByProjects = new HashMap<Project, DebuggerByProject>();
        debugDividerLocation = sdkConfig.getDouble("gui.splitPane5", 0.7);
        if(context.getDebugStatus() == Context.DEBUG_STATUS_ON){
            debugEnter();
        } else {
            debugExit();
        }
//        magnifier = new Magnifier(gui, context, architectureImplementation);
//        jTabbedPane1.addTab("Magnifier", magnifier);
        gui.getServices().getTargetManager().addStatusListener(this);
    }

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

    }

//-------------------------------------------------------------------------------------
    private void startDebug() {

    }

//-------------------------------------------------------------------------------------
    public void setActiveProject(Project _activeProject){
        activeProject = _activeProject;
        DebuggerByProject _debuggerByProject = debuggerByProjects.get(activeProject);
        if(_debuggerByProject == null){
            _debuggerByProject = new DebuggerByProject(gui, context, _activeProject);
            add(_debuggerByProject.toString(), _debuggerByProject);
        }
        cardLayout.show(this, _debuggerByProject.toString());
    }

//-------------------------------------------------------------------------------------
    public void refresh(){
        debuggerByProjects.get(activeProject).refresh();
    }

//-------------------------------------------------------------------------------------
    public void afterInit(){
        startDebug();
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
