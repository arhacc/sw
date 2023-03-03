//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.components.hierarchy;

//-------------------------------------------------------------------------------------
import java.io.*;
import java.net.*;
import java.util.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.tree.*;

import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.project.*;

import xpu.sw.tools.sdk.gui.*;

//-------------------------------------------------------------------------------------
public class HierarchyTreeModel implements TreeModel {
    private Gui gui;
    private Context context;
    private Logger log;

    private Project root;

    private List<Project> projects;
    private Project selectedProject;
    private File selectedFile;
    private List<TreeModelListener> listeners;


//-------------------------------------------------------------------------------------
    public HierarchyTreeModel(Gui _gui, Context _context) {
        gui = _gui;
        context= _context;
        log = _context.getLog();
        root = new Project(_context);
        projects = new ArrayList<Project>();
        listeners = new ArrayList<TreeModelListener>();
    }

//-------------------------------------------------------------------------------------
    public void addProject(Project _project){
        log.debug("addProject...");
        for(int i = 0; i < projects.size(); i++){
            Project _p = projects.get(i);
            if(_p.getPathToConfigFile().equals(_project.getPathToConfigFile())){
                return;
            }
        }
        projects.add(_project);       
        fireChange();
    }
    
//-------------------------------------------------------------------------------------
    public void removeProject(Project _project){
        projects.remove(selectedProject);
        fireChange();
    }
    
//-------------------------------------------------------------------------------------
    public Project getSelectedProject(){
        return selectedProject;
    }

//-------------------------------------------------------------------------------------    
    public void setSelectedProject(Project _selectedProject){
       selectedProject = _selectedProject;
    }

//-------------------------------------------------------------------------------------    
    public void setSelectedObject(Object _node){
/*        selectedProject = projects.get(_index);
        Object _node = jTree.getLastSelectedPathComponent();
        int _index = hierarchyTreeModel.getIndexOfChild(hierarchyTreeModel.getRoot(), _node1);
        log.debug("select project... _index=" + _index);
        if(_index != -1){
            hierarchyTreeModel.setSelectedObject(_node);
        }*/
//        log.debug("select project...=" + _node);
        if(_node instanceof File){
//            File _file = 
        }

    }

//-------------------------------------------------------------------------------------
    public File getSelectedFile(){
        return selectedFile;
    }

//-------------------------------------------------------------------------------------    
    public void setSelectedFile(File _selectedFile){
        selectedFile = _selectedFile;
    }
//-------------------------------------------------------------------------------------
    public Object getRoot() {
        return root;
    }

//-------------------------------------------------------------------------------------
    public Object getChild(Object _parent, int _index) {
//        log.debug("getChild..."+_parent+", _index="+_index);
        if(_parent instanceof Project){
            Project _project = (Project)_parent;
            if(_project.equals(root)){
                return projects.get(_index);
            }
        }
        File f = ((Project)_parent).getRootFile();
        return f.listFiles()[_index];
    }
 
//-------------------------------------------------------------------------------------
    public int getChildCount(Object _parent) {
        File _parentFile;
        if(_parent instanceof Project){
            Project _parentProject = (Project)_parent;
            if(_parentProject.equals(root)){
                return projects.size();
            }
            _parentFile = ((Project)_parent).getRootFile();
        } else {
            _parentFile = (File)_parent;
        }
 
        try {
            if (!_parentFile.isDirectory() && _parentFile.list() != null) {
                return 0;
            } else {
                return _parentFile.list().length;
            }
        } catch (NullPointerException ex) {
            return 0;
        }
    }
 
//-------------------------------------------------------------------------------------
    public boolean isLeaf(Object _node) {
        if(_node instanceof File){
            return !((File)_node).isDirectory();
        } else if(_node instanceof Project){
            return false;
        } else {
            return true;
        }
/*        Project _nodeProject = ((Project)_node);
        if(_nodeProject.isRoot()){
            return false;
        }
        File f = _nodeProject.getRootFile();
        return !f.isDirectory();*/
    }
 
//-------------------------------------------------------------------------------------
    public int getIndexOfChild(Object _parent, Object _child) {
        File _parentFile = null;
        if(_parent instanceof Project){
            _parentFile = ((Project)_parent).getRootFile();
        } else {
            _parentFile = (File)_parent;
        }
//        File par = ((Project)parent).getRootFile();
        if(_child instanceof Project){
            return projects.indexOf((Project)_child);
        }
        File ch = (File) _child;
        File[] _files = _parentFile.listFiles();
        if(_files != null){
            return Arrays.asList(_files).indexOf(ch);
        }
        return -1;
    }

//-------------------------------------------------------------------------------------
    public void addTreeModelListener(TreeModelListener _l) {
        listeners.add(_l);
    }

//-------------------------------------------------------------------------------------
    public void removeTreeModelListener(TreeModelListener _l) {
        listeners.remove(_l);
    }

//-------------------------------------------------------------------------------------
    public void valueForPathChanged(TreePath _path, Object _newValue) {

    }

//-------------------------------------------------------------------------------------
    public void fireChange() {
        TreeModelEvent _e = new TreeModelEvent(root, new TreePath(root));
//        log.debug("listeners.size=" + listeners.size());
        listeners.forEach(_l -> {
            _l.treeStructureChanged(_e);
        });
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
