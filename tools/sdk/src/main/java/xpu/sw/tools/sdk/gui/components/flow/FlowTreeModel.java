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
//public class FlowTreeModel implements TreeModel {
public class FlowTreeModel extends DefaultTreeModel {
    private Gui gui;
    private Context context;
    private Logger log;

    private DefaultMutableTreeNode root;

    private Map<String, DefaultMutableTreeNode> actions;
    private List<String> actionKeys;

//-------------------------------------------------------------------------------------
    public FlowTreeModel(Gui _gui, Context _context, DefaultMutableTreeNode _root) {
        super(_root);
        gui = _gui;
        context = _context;
        log = _context.getLog();
        root = _root;
        actions = new LinkedHashMap<String, DefaultMutableTreeNode>();
        add("Asm");
        add("Run");
        actionKeys = new ArrayList<>(actions.keySet());
    }

//-------------------------------------------------------------------------------------
    private void add(String _name){
        actions.put(_name, new DefaultMutableTreeNode(new Action(_name, Flow.STATUS_UNKNOWN)));
    }

//-------------------------------------------------------------------------------------
    public void setStatus(String _name, int _status){
        DefaultMutableTreeNode _d = actions.get(_name);
        Action _action = (Action)_d.getUserObject();
        _action.setStatus(_status);
        nodeStructureChanged(root);
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
            return actions.get(actionKeys.get(_index));
        } else {
            return null;            
        }
    }

//-------------------------------------------------------------------------------------
    public int getChildCount(Object _parent) {
        if(_parent.equals(root)){
            return actionKeys.size();
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
