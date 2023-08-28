//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.components.hierarchy;

//-------------------------------------------------------------------------------------
import java.io.*;
import java.net.*;
import java.util.*;
import java.util.stream.*;
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

    private HierarchyNode root;

    private List<HierarchyNode> projects;
    private HierarchyNode selectedProject;
    private HierarchyNode selectedFile;
    private List<TreeModelListener> listeners;


//-------------------------------------------------------------------------------------
    public HierarchyTreeModel(Gui _gui, Context _context) {
        gui = _gui;
        context= _context;
        log = _context.getLog();
        root = new HierarchyNode(_gui, _context);
        projects = new ArrayList<HierarchyNode>();
        listeners = new ArrayList<TreeModelListener>();
    }

//-------------------------------------------------------------------------------------
    public List<Project> getProjects(){
        return projects.stream()
            .map(_project -> _project.getProject())
            .collect(Collectors.toList());
    }

//-------------------------------------------------------------------------------------
    public void addProject(Project _project){
        for(int i = 0; i < projects.size(); i++){
            HierarchyNode _p = projects.get(i);
            if(_p.getProject().getPathToConfigFile().equals(_project.getPathToConfigFile())){
                return;
            }
        }
//        log.debug("addProject...");
        projects.add(new HierarchyNode(gui, context, _project));
        fireChange();
    }
    
//-------------------------------------------------------------------------------------
    public void removeProject(Project _project){
        projects.remove(selectedProject);
        fireChange();
    }
    
//-------------------------------------------------------------------------------------
    public Project getSelectedProject(){
        if(selectedProject != null){
            return selectedProject.getProject();
        }
        return null;
    }
/*
//-------------------------------------------------------------------------------------    
    public void setSelectedProject(Project _selectedProject){
       selectedProject = _selectedProject;
    }
*/
//-------------------------------------------------------------------------------------    
    public void setSelectedObject(HierarchyNode _projectNode, HierarchyNode _fileNode){
/*        selectedProject = projects.get(_index);
        Object _node = jTree.getLastSelectedPathComponent();
        int _index = hierarchyTreeModel.getIndexOfChild(hierarchyTreeModel.getRoot(), _node1);
        log.debug("select project... _index=" + _index);
        if(_index != -1){
            hierarchyTreeModel.setSelectedObject(_node);
        }*/
//        log.debug("select project...=" + _node);
        selectedProject = _projectNode;
        selectedFile = _fileNode;
    }

//-------------------------------------------------------------------------------------
    public File getSelectedFile(){
        if(selectedFile != null){
            return selectedFile.getFile();
        }
        return null;
    }

//-------------------------------------------------------------------------------------
    public Object getRoot() {
//        log.debug("getRoot..." + root);
        return root;
    }

//-------------------------------------------------------------------------------------
    public Object getChild(Object _parent, int _index) {
//        log.debug("getChild..."+_parent+", _index="+_index);
        HierarchyNode _parentNode = (HierarchyNode)_parent;
        if(_parentNode.isRoot()){
            return projects.get(_index);
        } else if(_parentNode.isProject()){
            Project _project = _parentNode.getProject();
            File _rootFile = _project.getRootFile();
            File[] _files = getChilds(_rootFile);
            return new HierarchyNode(gui, context, _files[_index]);
        } else if(_parentNode.isFile()){
            File _parentFile = _parentNode.getFile();
            if (_parentFile.isDirectory()) {
//                File[] _files = _parentFile.listFiles();
                File[] _files = getChilds(_parentFile);
                if(_files != null){
                    Arrays.sort(_files, (a,b) -> Boolean.compare(b.isDirectory(), a.isDirectory()));
                    return new HierarchyNode(gui, context, _files[_index]);
                } 
            } 
        }
        return null;
    }
 
//-------------------------------------------------------------------------------------
    public int getChildCount(Object _parent) {
        File _parentFile;
        HierarchyNode _parentNode = (HierarchyNode)_parent;
        if(_parentNode.isRoot()){
//            log.debug("getChildCount..." + projects.size());
            return projects.size();
        } else if(_parentNode.isProject()){
            Project _parentProject = _parentNode.getProject();
            _parentFile = _parentProject.getRootFile();
        } else {
            _parentFile = _parentNode.getFile();
        }
 
        if (_parentFile.isDirectory()){
//            File[] _files = _parentFile.listFiles();
            File[] _files = getChilds(_parentFile);
            if(_files != null){
                return _files.length;
            }
        }
        return 0;
    }
 
//-------------------------------------------------------------------------------------
    public boolean isLeaf(Object _objectNode) {
        HierarchyNode _node = (HierarchyNode)_objectNode;
        if(_node.isRoot()){
            return (projects.size() == 0);
        } else if(_node.isFile()){
            return !((File)_node.getFile()).isDirectory();
        } else if(_node.isProject()){
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
        HierarchyNode _parentNode = (HierarchyNode)_parent;
        File _parentFile = null;
        if(_parentNode.isProject()){
            _parentFile = (_parentNode.getProject()).getRootFile();
        } else if(_parentNode.isFile()){
            _parentFile = _parentNode.getFile();
        }
//        File par = ((Project)parent).getRootFile();
        HierarchyNode _childNode = (HierarchyNode)_child;
        if(_childNode.isProject()){
            return projects.indexOf(_childNode.getProject());
        } else if(_childNode.isFile()){
            File _childFile = _childNode.getFile();
//            File[] _files = _parentFile.listFiles();
            File[] _files = getChilds(_parentFile);            
            Arrays.sort(_files, (a,b) -> Boolean.compare(b.isDirectory(), a.isDirectory()));
            if(_files != null){
                return Arrays.asList(_files).indexOf(_childFile);
            }
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
    public File[] getChilds(File _projectRootFile) {
//        log.debug("getChild..."+_parent+", _index="+_index);
        File[] _files = _projectRootFile.listFiles(new FilenameFilter() {
                public boolean accept(File _dirFiles, String _filename) {
                    _filename = _filename.toLowerCase();
                    return _filename.endsWith(".asm")
                            || _filename.endsWith(".cpp")
                            || _filename.endsWith(".onnx")
                            || _filename.endsWith(".hex")
                            || _filename.endsWith(".obj")
                            || _filename.endsWith(".json");
            }
        });
        Arrays.sort(_files, (a,b) -> Boolean.compare(b.isDirectory(), a.isDirectory()));
        return _files;
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
