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
public class HierarchyByLevel extends GuiPanel {
    private String level;

    private JTree jTree;
    private HierarchyCellRenderer hierarchyCellRenderer;
    private HierarchyTreeModel hierarchyTreeModel;

//-------------------------------------------------------------------------------------
    public HierarchyByLevel(Context _context, Gui _gui, String _level) {
        super(_context, _gui);
        level = _level;
//        jTree = _gui.getHierarchy();
        init();
    }

//-------------------------------------------------------------------------------------
    private void init(){
        jTree = new JTree();
        jTree.setRootVisible(false);
        jTree.setShowsRootHandles(true);
        hierarchyCellRenderer = new HierarchyCellRenderer(gui, context);
        jTree.setCellRenderer(hierarchyCellRenderer);
        hierarchyTreeModel = new HierarchyTreeModel(gui, context);
        jTree.setModel(hierarchyTreeModel);
        jTree.addMouseListener(new HierarchyMouseListener(context, gui, this));
 
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
    public JTree getTree(){
        return jTree;
    }

//-------------------------------------------------------------------------------------
    public HierarchyTreeModel getHierarchyTreeModel(){
        return hierarchyTreeModel;
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
