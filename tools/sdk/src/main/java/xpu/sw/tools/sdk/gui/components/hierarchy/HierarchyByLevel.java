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
    public HierarchyByLevel(Context _context, Gui _gui, String _level) {
        super(_context, _gui);
        level = _level;
//        basePath = _basePath;
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
 
        loadProjects();

        setLayout(new BorderLayout());
        add(jTree);
    }

//-------------------------------------------------------------------------------------
    private void loadProjects(){
        String _librariesPath = sdkConfig.getString("librariesPath", "~/");
        boolean _autoCreateProjectWhenOpenDirectory = sdkConfig.getBoolean("autoCreateProjectWhenOpenDirectory", true);
        switch (level) {
            case Context.PROFILE_LEVEL_LOW: {
                basePath = _librariesPath + "lowlevel";
                loadProjectsFromDirectory(basePath, _autoCreateProjectWhenOpenDirectory);
                break;
            }    
            case Context.PROFILE_LEVEL_MID: {
                basePath = _librariesPath + "midlevel";
                loadProjectsFromDirectory(basePath, _autoCreateProjectWhenOpenDirectory);
                break;
            }    
            case Context.PROFILE_LEVEL_APP: {
                basePath = sdkConfig.getString("appsPath", "~/.xpu/projects/");
                java.util.List<String> _projectsPaths = sdkConfig.getList(String.class, "open_projects");
                loadProjectsFromList(_projectsPaths);
                break;
            }    
            default: {
                log.error("Unknown level in Hierarchy: " + level);
                break;
            }
        }
    }

//-------------------------------------------------------------------------------------
    private void loadProjectsFromDirectory(String _basePath, boolean _autoCreateProjectWhenOpenDirectory){
/*        java.util.List<String> _openProjectsPaths = new ArrayList<String>();
        java.util.List<File> _listOfDirectories = Arrays.asList(new File(_basePath).listFiles(new FilenameFilter() {
            public boolean accept(File _dirFile, String _filename) {
                return new File(_dirFile, _filename).isDirectory();
            }
        }));
        for(File _dirFile : _listOfDirectories){

            if(_autoCreateProjectWhenOpenDirectory){
                _openProjectsPaths.add(Project.autoCreate(_dirFile));
            }
        }
        loadProjectsFromList(_openProjectsPaths);*/
    }

//-------------------------------------------------------------------------------------
    private void loadProjectsFromList(java.util.List<String> _openProjectsPaths){
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
