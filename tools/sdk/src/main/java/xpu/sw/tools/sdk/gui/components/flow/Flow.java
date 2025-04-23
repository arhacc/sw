//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.components.flow;
//-------------------------------------------------------------------------------------
import java.awt.*;
import java.net.*;
import java.util.*;
import javax.swing.*;
import javax.swing.tree.*;

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


    public static final int STATUS_UNKNOWN  = 0;
    public static final int STATUS_WAITING  = 1;
    public static final int STATUS_DONE     = 2;
    public static final int STATUS_ERROR    = 3;

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
        flowTreeModel = new FlowTreeModel(gui, context, new DefaultMutableTreeNode(">"));
        jTree.setModel(flowTreeModel);

//        JScrollPane scrollTree = new JScrollPane(jTree);
//        scrollTree.setViewportView(jTree);
//        jPanel.add(scrollTree);
        jPanel.setLayout(new BorderLayout());
        jPanel.add(jTree);
    }

//-------------------------------------------------------------------------------------
    public void setStatus(String _name, int _status){
        flowTreeModel.setStatus(_name, _status);
        jTree.revalidate();  // re-layout the tree if needed
        jTree.repaint();     // repaint the tree component
    }
//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
