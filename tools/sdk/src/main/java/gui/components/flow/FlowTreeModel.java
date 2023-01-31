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
public class FlowTreeModel implements TreeModel {
    private Gui gui;
    private Context context;
    private Logger log;

    private Object root;

    private Object[] level0;

//-------------------------------------------------------------------------------------
    public FlowTreeModel(Gui _gui, Context _context) {
        gui = _gui;
        context = _context;
        log = _context.getLog();
        root = new String(">");
        level0 = new String[5];
        level0[0] = new String("Compile");
        level0[1] = new String("Analyze");
        level0[2] = new String("Reconfigure");
        level0[3] = new String("Program");
        level0[4] = new String("Run");
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
