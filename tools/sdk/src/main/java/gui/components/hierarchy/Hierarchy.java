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


//-------------------------------------------------------------------------------------
public class Hierarchy extends MouseAdapter {
    private Gui gui;
    private Context context;
    private Logger log;

    private JTree jTree;
    private HierarchyCellRenderer hierarchyCellRenderer;
    private HierarchyTreeModel hierarchyTreeModel;
    private Configuration sdkConfig;

//-------------------------------------------------------------------------------------
    public Hierarchy(Gui _gui, Context _context) {
        gui = _gui;
        context = _context;
        log = _context.getLog();
        jTree = _gui.getHierarchy();
        sdkConfig = context.getSdkConfig();
        init();
    }

//-------------------------------------------------------------------------------------
    private void init(){
        jTree.setRootVisible(false);
        jTree.setShowsRootHandles(true);
        hierarchyCellRenderer = new HierarchyCellRenderer(gui, context);
        jTree.setCellRenderer(hierarchyCellRenderer);
        hierarchyTreeModel = new HierarchyTreeModel(gui, context);
        jTree.setModel(hierarchyTreeModel);
        jTree.addMouseListener(this);
 
        java.util.List<String> _openProjectsPaths = sdkConfig.getList(String.class, "open_projects");
        if(_openProjectsPaths != null){
            log.debug("_openProjectsPaths.size=" + _openProjectsPaths.size());
            _openProjectsPaths.forEach( _openProjectsPath -> {
                Project _project = new Project(context, _openProjectsPath);
                if(_project.isValid()){
                    addProject(_project);
                } else {
                    log.debug("Cannot find project file: " + _openProjectsPath);
                }
            });
        }

    }

//-------------------------------------------------------------------------------------
    public void setProfile(String _profile){
        
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
    public void addProject(Project _project){
        addProject(_project, false);
    }
    
//-------------------------------------------------------------------------------------
    public void addProject(Project _project, boolean _addToConfig){
        hierarchyTreeModel.addProject(_project);
//        jTree.expandPath(new TreePath(_project));
        refresh();
        if(_addToConfig){
            sdkConfig.addProperty("open_projects", _project.getPathToConfigFile());
        }
    }
    
//-------------------------------------------------------------------------------------
    public void removeProject(Project _project){
       // ConfigDeleteProject(_project);
        hierarchyTreeModel.removeProject(_project);

//        sdkConfig.removeProperty("open_projects", _project.getPathToConfigFile());        
    }
    
//-------------------------------------------------------------------------------------
    public String getSelectedProjectPath(){
        Project _p = getSelectedProject();
        if(_p != null){
            return _p.getPathToConfigFile();
        }
        return null;
    }

//-------------------------------------------------------------------------------------
    public Project getSelectedProject(){
        return hierarchyTreeModel.getSelectedProject();
//        sdkConfig.removeProperty("open_projects", _project.getPathToConfigFile());        
    }

//-------------------------------------------------------------------------------------
    public File getSelectedFile(){
        return hierarchyTreeModel.getSelectedFile();
//        sdkConfig.removeProperty("open_projects", _project.getPathToConfigFile());        
    }

//-------------------------------------------------------------------------------------
    public void refresh(){
        hierarchyTreeModel.fireChange();
//        sdkConfig.removeProperty("open_projects", _project.getPathToConfigFile());        
    }
//-------------------------------------------------------------------------------------

}

//-------------------------------------------------------------------------------------
