//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.components;

//-------------------------------------------------------------------------------------
import java.net.*;
import java.util.*;
import javax.swing.*;

import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.gui.*;
import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.gui.components.menu.*;
import xpu.sw.tools.sdk.gui.components.messager.*;
import xpu.sw.tools.sdk.gui.components.toolbar.*;
import xpu.sw.tools.sdk.gui.components.hierarchy.*;
import xpu.sw.tools.sdk.gui.components.flow.*;
import xpu.sw.tools.sdk.gui.components.editor.*;
import xpu.sw.tools.sdk.gui.components.terminal.*;
import xpu.sw.tools.sdk.gui.components.debugger.*;

//-------------------------------------------------------------------------------------
public class Components extends Thread {
    private Gui gui;
    private Context context;
    private Logger log;

    private Menu menu;
    private Messager messager;
    private Toolbar toolbar;
    private Hierarchy hierarchy;
    private Flow flow;
    private Editor editor;
    private Terminal terminal;
    private Debugger debugger;
//    private Statusbar statusbar;

//-------------------------------------------------------------------------------------
    public Components(Gui _gui, Context _context) {
        gui = _gui;
        context = _context;
        log = _context.getLog();
        run();
    }

//-------------------------------------------------------------------------------------
    public void run(){
        addComponents();
    }

//-------------------------------------------------------------------------------------
    private void addComponents(){
        menu = new Menu(gui, context);
        messager = new Messager(gui, context);
        toolbar = new Toolbar(gui, context);
        hierarchy = new Hierarchy(context, gui);
        flow = new Flow(gui, context);
        editor = new Editor(gui, context);
        terminal = new Terminal(gui, context);
        debugger = new Debugger(gui, context);
    }

//-------------------------------------------------------------------------------------
    public Menu getMenu(){
        return menu;
    }

//-------------------------------------------------------------------------------------
    public Messager getMessager(){
        return messager;
    }

//-------------------------------------------------------------------------------------
    public Toolbar getToolbar(){
        return toolbar;
    }

//-------------------------------------------------------------------------------------
    public Hierarchy getHierarchy(){
        return hierarchy;
    }

//-------------------------------------------------------------------------------------
    public Editor getEditor(){
        return editor;
    }

//-------------------------------------------------------------------------------------
    public Terminal getTerminal(){
        return terminal;
    }

//-------------------------------------------------------------------------------------
    public Debugger getDebugger(){
        return debugger;
    }


//-------------------------------------------------------------------------------------
    public void afterInit(){
//        menu.afterInit();
        messager.afterInit();
        toolbar.afterInit();
//        hierarchy.afterInit();
//        flow.afterInit();
//        editor.afterInit();
//        terminal.afterInit();
        debugger.afterInit();
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
