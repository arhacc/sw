//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.components.menu.file.preferences.selector;

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
public class SelectorTreeModel implements TreeModel {
    private Gui gui;
    private Context context;
    private Logger log;

    private DefaultMutableTreeNode root;

    private DefaultMutableTreeNode[] level0;

//-------------------------------------------------------------------------------------
    public SelectorTreeModel(Gui _gui, Context _context) {
        gui = _gui;
        context = _context;
        log = _context.getLog();
        root = new DefaultMutableTreeNode(new String(">"));
        level0 = new DefaultMutableTreeNode[6];
        level0[0] = new DefaultMutableTreeNode(new String("General"));
        level0[1] = new DefaultMutableTreeNode(new String("Editor"));
        level0[2] = new DefaultMutableTreeNode(new String("Libraries"));
        level0[3] = new DefaultMutableTreeNode(new String("Compiler"));
        level0[4] = new DefaultMutableTreeNode(new String("Synthesis"));
        level0[5] = new DefaultMutableTreeNode(new String("Targets"));
    }

//-------------------------------------------------------------------------------------
    public Object getRoot() {
        return root;
    }

//-------------------------------------------------------------------------------------
    public boolean isLeaf(Object _node) {
        return getChildCount(_node) == 0;
    }

//-------------------------------------------------------------------------------------
    public Object getChild(Object _parent, int _index) {
        if(_parent.equals(root)){
            return level0[_index];
        } else {
            return null;            
        }
    }

//-------------------------------------------------------------------------------------
    public int getChildCount(Object _parent) {
        if(_parent.equals(root)){
            return level0.length;
        } else {
            return 0;
        }
    }

//-------------------------------------------------------------------------------------
    public int getIndexOfChild(Object _parent, Object _child) {
        return 0;
    }

//-------------------------------------------------------------------------------------
    public void addTreeModelListener(TreeModelListener _l) {

    }

//-------------------------------------------------------------------------------------
    public void removeTreeModelListener(TreeModelListener _l) {

    }

//-------------------------------------------------------------------------------------
    public void valueForPathChanged(TreePath _path, Object _newValue) {

    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
