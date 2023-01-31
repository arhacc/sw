//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.components.flow;

//-------------------------------------------------------------------------------------
import java.net.*;
import java.util.*;
import javax.swing.*;

import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.context.*;

import xpu.sw.tools.sdk.gui.*;

//-------------------------------------------------------------------------------------
public class Flow {
    private Gui gui;
    private Context context;
    private Logger log;

    private JTree jTree;
    private FlowCellRenderer flowCellRenderer;
    private FlowTreeModel flowTreeModel;

//-------------------------------------------------------------------------------------
    public Flow(Gui _gui, Context _context) {
        gui = _gui;
        context = _context;
        log = _context.getLog();
        jTree = _gui.getFlow();
        init();
    }

//-------------------------------------------------------------------------------------
    private void init(){
        jTree.setRootVisible(false);
        jTree.setShowsRootHandles(true);
        flowCellRenderer = new FlowCellRenderer(gui, context);
        jTree.setCellRenderer(flowCellRenderer);
        flowTreeModel = new FlowTreeModel(gui, context);
        jTree.setModel(flowTreeModel);
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
