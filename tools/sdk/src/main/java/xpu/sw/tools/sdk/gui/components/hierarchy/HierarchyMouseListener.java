//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.components.hierarchy;

//-------------------------------------------------------------------------------------
import java.io.*;
import java.awt.*;
import java.awt.event.*;
import java.net.*;
import java.util.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.tree.*;

import org.apache.commons.configuration2.*;
import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.project.*;
import xpu.sw.tools.sdk.gui.*;
import xpu.sw.tools.sdk.gui.components.common.*;


//-------------------------------------------------------------------------------------
public class HierarchyMouseListener extends MouseAdapter {
    private Context context;
    private Gui gui;
    private HierarchyByLevel hierarchyByLevel;
    private JTree jTree;
    private HierarchyTreeModel hierarchyTreeModel;

//-------------------------------------------------------------------------------------
    public HierarchyMouseListener(Context _context, Gui _gui, HierarchyByLevel _hierarchyByLevel) {
        context = _context;
        gui = _gui;
        hierarchyByLevel = _hierarchyByLevel;
        jTree = _hierarchyByLevel.getTree();
        hierarchyTreeModel = _hierarchyByLevel.getHierarchyTreeModel();
    }


//-------------------------------------------------------------------------------------
    public void mouseClicked(MouseEvent _e) {
        Object _nodeSelected = jTree.getLastSelectedPathComponent();
        TreePath _treePath = jTree.getSelectionPath();
        if(_treePath == null){
            return;
        }
        HierarchyNode _projectNode = (HierarchyNode)_treePath.getPathComponent(0);
        HierarchyNode _fileNode = (HierarchyNode)_treePath.getLastPathComponent();
        hierarchyTreeModel.setSelectedObject(_projectNode, _fileNode);
        if (_e.getClickCount() == 2) {
            if (_treePath == null) {                        
                return;
            }
//                    Object _nodeInfo = _node.getUserObject();
            if(_fileNode.isFile()){
                String _filePath = _fileNode.getFile().getAbsolutePath();
                gui.getMyComponents().getEditor().addTab(_filePath);
            }
            // Cast nodeInfo to your object and do whatever you want
        } else {
            if(SwingUtilities.isRightMouseButton(_e)){
                int _id;
                    if(_nodeSelected instanceof Project)_id = 0;
                        else
                            if(_nodeSelected instanceof File)_id = 1;
                                 else
                                     _id = -1;
                    HierarchyPopupMenu jPopupMenu = new HierarchyPopupMenu(gui, jTree, _id,_nodeSelected);
                    jPopupMenu.show(_e.getComponent(), _e.getX(), _e.getY());
            } else {
/*                boolean _isSelected;
                _isSelected = (jTree.getRowForLocation(_e.getX(), _e.getY()) != -1);
                if(_isSelected){
                    TreeSelectionModel m = jTree.getSelectionModel();
                    if (m != null) {
                        m.clearSelection();
                    }
                }*/
            }
        }
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
