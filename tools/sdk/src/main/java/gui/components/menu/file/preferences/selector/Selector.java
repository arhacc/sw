//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.components.menu.file.preferences.selector;

//-------------------------------------------------------------------------------------
import java.net.*;
import java.util.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.tree.*;
import javax.swing.text.*;

import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.context.*;

import xpu.sw.tools.sdk.gui.*;
import xpu.sw.tools.sdk.gui.components.menu.file.preferences.*;
import xpu.sw.tools.sdk.gui.components.menu.file.preferences.sections.*;

//-------------------------------------------------------------------------------------
public class Selector {
    private Gui gui;
    private Context context;
    private Logger log;

    private JTree jTree;
    private Sections sections;
    private Preferences preferences;

    private SelectorCellRenderer selectorCellRenderer;
    private SelectorTreeModel selectorTreeModel;

//-------------------------------------------------------------------------------------
    public Selector(Gui _gui, Context _context, JTree _jTree, Sections _sections, Preferences _preferences) {
        gui = _gui;
        context = _context;
        log = _context.getLog();
        jTree = _jTree;
        sections = _sections;
        preferences = _preferences;
        init();
    }

//-------------------------------------------------------------------------------------
    private void init(){
        jTree.setRootVisible(false);
        jTree.setShowsRootHandles(true);
        selectorCellRenderer = new SelectorCellRenderer(gui, context);
        jTree.setCellRenderer(selectorCellRenderer);
        selectorTreeModel = new SelectorTreeModel(gui, context);
        jTree.setModel(selectorTreeModel);

        for (int i = 0; i < jTree.getRowCount(); i++) {
            jTree.expandRow(i);
        }
//        DefaultMutableTreeNode _firstLeaf = ((DefaultMutableTreeNode)jTree.getModel().getRoot()).getFirstLeaf();
//        jTree.setSelectionPath(new TreePath(_firstLeaf.getPath()));        
        jTree.getSelectionModel().setSelectionMode(TreeSelectionModel.SINGLE_TREE_SELECTION);
        jTree.addTreeSelectionListener(new TreeSelectionListener() {
            public void valueChanged(TreeSelectionEvent e) {
                DefaultMutableTreeNode _node = (DefaultMutableTreeNode)
                                   jTree.getLastSelectedPathComponent();
            /* if nothing is selected */ 
                if (_node == null) return;
            /* retrieve the node that was selected */ 
                String _selectedItem = (String)_node.getUserObject();
//                log.debug("---> " + _selectedItem);
                sections.select(_selectedItem);
            }
        });
        jTree.setSelectionRow(0);
    }
//-------------------------------------------------------------------------------------
    public void select(String _section){
        TreePath _treePath = jTree.getNextMatch(_section, 0, Position.Bias.Forward);
        jTree.setSelectionPath(_treePath);
//        sections.select(_section);
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
