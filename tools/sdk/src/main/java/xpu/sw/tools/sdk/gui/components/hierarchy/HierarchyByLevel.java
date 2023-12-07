//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.components.hierarchy;

//-------------------------------------------------------------------------------------
import java.io.*;
import java.awt.*;
import java.awt.event.*;
import java.net.*;
import java.util.*;
import java.nio.file.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.tree.*;

import org.apache.commons.configuration2.*;
import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.io.*;
import xpu.sw.tools.sdk.common.project.*;
import xpu.sw.tools.sdk.common.utils.*;
import xpu.sw.tools.sdk.gui.*;
import xpu.sw.tools.sdk.gui.components.common.*;


//-------------------------------------------------------------------------------------
public class HierarchyByLevel extends GuiPanel {
    private String level;
    private String basePath;

    private JTree jTree;
    private HierarchyCellRenderer hierarchyCellRenderer;
    private HierarchyTreeModel hierarchyTreeModel;

//-------------------------------------------------------------------------------------
    public HierarchyByLevel(Context _context, Gui _gui, String _level, String _relativePath) {
        super(_context, _gui);
        level = _level;
        basePath = PathResolver.importPath(sdkConfig.getString("librariesPath", "~/")) + _relativePath;
        init();
    }

//-------------------------------------------------------------------------------------
    private void init(){
        jTree = new JTree();        

        JScrollPane _scrollPane = new JScrollPane(jTree);
        _scrollPane.setViewportView(jTree);

        jTree.setRootVisible(false);
        jTree.setShowsRootHandles(true);        
        hierarchyCellRenderer = new HierarchyCellRenderer(gui, context);
        jTree.setCellRenderer(hierarchyCellRenderer);

        hierarchyTreeModel = new HierarchyTreeModel(gui, context, basePath);
        jTree.setModel(hierarchyTreeModel);
        jTree.addMouseListener(new HierarchyMouseListener(context, gui, this));
 
        setLayout(new BorderLayout());
        add(_scrollPane);
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
//        hierarchyTreeModel.addProject(_project);
        refresh();
        if(_addToConfig){
            sdkConfig.addProperty("open_projects", _project.getPathToConfigFile());
        }
    }
    
//-------------------------------------------------------------------------------------
    public void removeProject(Project _project){
       // ConfigDeleteProject(_project);
//        hierarchyTreeModel.removeProject(_project);

//        sdkConfig.removeProperty("open_projects", _project.getPathToConfigFile());        
    }

//-------------------------------------------------------------------------------------
    public java.util.List<Project> getProjects(){
       // ConfigDeleteProject(_project);
        return hierarchyTreeModel.getProjects();

//        sdkConfig.removeProperty("open_projects", _project.getPathToConfigFile());        
    }

//-------------------------------------------------------------------------------------
    public String getUniqueFileInPath(Path _path, String _fileExtension) throws IOException {

/*        if (!Files.isDirectory(path)) {
            throw new IllegalArgumentException("Path must be a directory!");
        }*/
        
//        System.out.println(">>>" + path.toString());
/*        java.util.List<String> _files;
        if(_path.toString().endsWith(".xpuprj")){
            _files = findFilesInProject(_path, _fileExtension);
        } else if (!Files.isDirectory(_path)) {
            throw new IllegalArgumentException("Path must be a directory:" + _path);
        } else {
            _files = findFilesInDirectory(_path, _fileExtension);
        }
        if((_files != null) && (_files.size() > 0)){
            return _files.get(0);
        }
        return null;*/
        return ".";
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
