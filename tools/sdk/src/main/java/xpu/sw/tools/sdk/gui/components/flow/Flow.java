//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.components.flow;
//-------------------------------------------------------------------------------------
import java.awt.*;
import java.net.*;
import java.util.*;
import javax.swing.*;

import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.context.*;

import xpu.sw.tools.sdk.gui.*;
import xpu.sw.tools.sdk.gui.components.common.*;

//-------------------------------------------------------------------------------------
public class Flow extends GuiBasic {
    private JPanel jPanel;
    private JTree jTree;
    private FlowCellRenderer flowCellRenderer;
    private FlowTreeModel flowTreeModel;

//-------------------------------------------------------------------------------------
    public Flow(Context _context, Gui _gui) {
        super(_context, _gui);
        jPanel = _gui.getFlow();
        init();
    }

//-------------------------------------------------------------------------------------
    private void init(){
        jTree = new JTree();
        jTree.setRootVisible(false);
        jTree.setShowsRootHandles(true);
        flowCellRenderer = new FlowCellRenderer(gui, context);
        jTree.setCellRenderer(flowCellRenderer);
        flowTreeModel = new FlowTreeModel(gui, context);
        jTree.setModel(flowTreeModel);

//        JScrollPane scrollTree = new JScrollPane(jTree);
//        scrollTree.setViewportView(jTree);
//        jPanel.add(scrollTree);
        jPanel.setLayout(new BorderLayout());
        jPanel.add(jTree);
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
