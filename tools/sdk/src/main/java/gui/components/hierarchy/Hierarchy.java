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
    public void mouseClicked(MouseEvent _e) {
        Object _node1 = jTree.getLastSelectedPathComponent();
        int _index = hierarchyTreeModel.getIndexOfChild(hierarchyTreeModel.getRoot(), _node1);
        log.debug("select project... _index=" + _index);
        if(_index != -1){
            hierarchyTreeModel.setSelectedProjectIndex(_index);
        }
        if (_e.getClickCount() == 2) {
            Object _node = jTree.getLastSelectedPathComponent();
            if (_node == null) {                        
                return;
            }
//                    Object _nodeInfo = _node.getUserObject();
            if(_node instanceof File){
                String _filePath = ((File)_node).getAbsolutePath();
                gui.getMyComponents().getEditor().addTab(_filePath);
            }
            // Cast nodeInfo to your object and do whatever you want
        } else {
            Object _node = jTree.getLastSelectedPathComponent();
            if(SwingUtilities.isRightMouseButton(_e)){
                int _id;
                    if(_node instanceof Project)_id = 0;
                        else
                            if(_node instanceof File)_id = 1;
                                 else
                                     _id = -1;
                    MyPopupMenu jPopupMenu = new MyPopupMenu(_id,_node);
                    jPopupMenu.show(_e.getComponent(), _e.getX(), _e.getY());
            } else {
                boolean _isSelected;
                _isSelected = (jTree.getRowForLocation(_e.getX(), _e.getY()) != -1);
                if(_isSelected){
                    TreeSelectionModel m = jTree.getSelectionModel();
                    if (m != null) {
                        m.clearSelection();
                    }
                }
            }
        }
    }

//-------------------------------------------------------------------------------------
    class MyPopupMenu extends JPopupMenu {
      private int id;
      Object node;
      public MyPopupMenu(int _id,Object _node) {
         id = _id;
        node=_node;
         switch(id)
         {
             case 0:{
                //JMenuItems
                JMenuItem _closeProjectMenuItem = new JMenuItem("Close");
                //Add Items
                add(_closeProjectMenuItem);
                //Actions
                _closeProjectMenuItem.addActionListener(new ActionListener() {
                 public void actionPerformed(ActionEvent e) {
                     gui.getMyComponents().getMenu().getMenuHandlers().closeProject();
                 }
               });
                break;
             }
             case 1:{
                 //JMenuItems
                JMenuItem _openFileMenuItem = new JMenuItem("Open File");
                //Add Items
                add(_openFileMenuItem);
                //Actions
                _openFileMenuItem.addActionListener(new ActionListener() {
                 public void actionPerformed(ActionEvent e) {
                     String _filePath = ((File)_node).getAbsolutePath();
                     gui.getMyComponents().getEditor().addTab(_filePath);
                 }
               });
                 break;
             }

             default:{
                 //JMenuItems
                JMenuItem _newProjectMenuItem = new JMenuItem("New");
                JMenuItem _openProjectMenuItem = new JMenuItem("Open");
                //Add Items
                add(_newProjectMenuItem);
                add(new JSeparator());
                add(_openProjectMenuItem);
                //Actions
                _newProjectMenuItem.addActionListener(new ActionListener() {
                 public void actionPerformed(ActionEvent e) {
                     gui.getMyComponents().getMenu().getMenuHandlers().newProject();
                 }
               });
                _openProjectMenuItem.addActionListener(new ActionListener() {
                 public void actionPerformed(ActionEvent e) {
                     gui.getMyComponents().getMenu().getMenuHandlers().openProject();
                 }
               });
                 break;
             }
         }
         TreeSelectionModel m = jTree.getSelectionModel();
         if (m != null) {
         m.clearSelection();
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
        jTree.expandPath(new TreePath(_project));
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
    public void refresh(){
        hierarchyTreeModel.fireChange();
//        sdkConfig.removeProperty("open_projects", _project.getPathToConfigFile());        
    }
//-------------------------------------------------------------------------------------

}

//-------------------------------------------------------------------------------------
