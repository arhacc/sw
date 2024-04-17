//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.components.hierarchy;

//-------------------------------------------------------------------------------------
import java.io.*;
import java.net.*;
import java.util.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.tree.*;

import org.apache.commons.io.*;
import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.project.*;
import xpu.sw.tools.sdk.common.xbasics.*;
import xpu.sw.tools.sdk.common.io.*;

import xpu.sw.tools.sdk.gui.*;
import xpu.sw.tools.sdk.common.fileformats.xpuprj.*;

import xpu.sw.tools.sdk.gui.components.common.*;

//-------------------------------------------------------------------------------------
public class HierarchyNode extends GuiBasic implements TreeNode {
//    private Gui gui;

    private HierarchyNode parentNode;
    private String name;
    private String path;
    private Project project;
    private File file;
    private List<HierarchyNode> childs;
    private static FilenameFilter filenameFilter = new FilenameFilter() {
            @Override
            public boolean accept(File dir, String name) {
                return !name.endsWith(".gittemp") 
                    && !name.endsWith(".DS_Store")
                    && !name.endsWith(".obj") 
//                    && !name.endsWith(".xpuprj")
                ;
            }
        };
//-------------------------------------------------------------------------------------
    public HierarchyNode(Gui _gui, Context _context, HierarchyNode _parentNode) {
        this(_gui, _context, _parentNode, null, null);
    }

//-------------------------------------------------------------------------------------
    public HierarchyNode(Gui _gui, Context _context, HierarchyNode _parentNode, Project _project) {
        this(_gui, _context, _parentNode, _project, null);
    }

//-------------------------------------------------------------------------------------
    public HierarchyNode(Gui _gui, Context _context, HierarchyNode _parentNode, String _path) {
        this(_gui, _context, _parentNode, new File(_path));
    }

//-------------------------------------------------------------------------------------
    public HierarchyNode(Gui _gui, Context _context, HierarchyNode _parentNode, File _file) {
        this(_gui, _context, _parentNode, null, _file);
    }

//-------------------------------------------------------------------------------------
    public HierarchyNode(Gui _gui, Context _context, HierarchyNode _parentNode, Project _project, File _file) {
        super(_context,  _gui);
//        gui = _gui;
        parentNode = _parentNode;
        project = _project;
        file = _file;
        childs = new ArrayList<HierarchyNode>();
    }

//-------------------------------------------------------------------------------------
    public boolean isRoot(){
        return (project == null) && (file == null);
    }
    
//-------------------------------------------------------------------------------------
    public boolean isLeaf(){
        return childs.size() == 0;
    }
    
//-------------------------------------------------------------------------------------
    public void refresh(){
        childs.clear();
        if(file != null){
            if(file.isDirectory()){
                File[] _files = file.listFiles(filenameFilter);
                Arrays.sort(_files, (a,b) -> {
                    if (a.isDirectory() && b.isDirectory()) {
                        return a.getName().compareTo(b.getName());
                    } else if (a.isDirectory()) {
                        return -1;
                    } else if (b.isDirectory()) {
                        return 1;
                    } else {
                        return a.getName().compareTo(b.getName());
                    }
                });
                for (int i = 0; i < _files.length ; i++) {
                    childs.add(new HierarchyNode(gui, context, this, _files[i]));
                }
            } else {
                String _filePath = file.getAbsolutePath();
//                String _extension = Paths.get(_filePath).getExtension();
                log.debug("_filePath:" + _filePath +", parentNode:" + parentNode);
                if(FilenameUtils.isExtension(_filePath, XpuprjFile.EXTENSION)){
                    if(parentNode != null){
                        parentNode.project = new Project(context, _filePath);
                    }
//                    log.debug("isExtension:" + project);
                }
            }
        }

        for (int i = 0; i < childs.size(); i++) {
            childs.get(i).refresh();
        }

        if(isProject()){
            name = project.toString();
            path = project.getRootPath();
        } else if(file != null){
            name = file.getName();
            path = file.getPath();
        } else {
            log.warn("Warning: no name assigned to this node!");
            name = null;
        }
    }

//-------------------------------------------------------------------------------------
    public List<Project> getProjects(){
        List<Project> _projects = new ArrayList<Project>();
        if(isProject()){
            _projects.add(getProject());
        }
        for (int i = 0; i < childs.size(); i++) {
            List<Project> _childProjects = childs.get(i).getProjects();
            if(_childProjects.size() > 0){
                _projects.addAll(_childProjects);
            }
        }
        return _projects;
    }
  
//-------------------------------------------------------------------------------------
    public boolean isProject(){
        return (project != null);
    }
    
//-------------------------------------------------------------------------------------
    public boolean isFile(){
        return (file != null) && (!file.isDirectory());
    }

//-------------------------------------------------------------------------------------
    public Project getProject(){
        return project;
    }
    
//-------------------------------------------------------------------------------------
    public File getFile(){
        return file;
    }

//-------------------------------------------------------------------------------------
    public String getPath(){
        return path;
    }

//-------------------------------------------------------------------------------------
    public HierarchyNode getNode(String _path){
//        log.debug("HierarchyNode.getNode: path=" + path +", name="+ name +", path=" + _path);
        if(_path.equals(path)){
            return this;
        } else if(_path.startsWith(path)){
//            _path = _path.replace(path + PathResolver.separator, "");
//TODO: HashMap instead of List
            for(int i = 0; i < childs.size(); i++){
                HierarchyNode _fileNode = childs.get(i).getNode(_path);
                if(_fileNode != null){
                    return _fileNode;
                }
            }
        }
        return null;
    }

//-------------------------------------------------------------------------------------
    public TreeNode getChildAt(int _index) {
        return (TreeNode)getChild(this, _index);
    }

//-------------------------------------------------------------------------------------
    public Object getChild(Object _parent, int _index) {
//        log.debug("getChild..."+_parent+", _index="+_index);
        if(((HierarchyNode)_parent).equals(this)){
            return childs.get(_index);
        }
        for(int i = 0; i < childs.size(); i++){
            HierarchyNode _child = (HierarchyNode)childs.get(i).getChild(_parent, _index);
            if(_child != null){
                return _child;
            }
        }
        return null;
/*
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
*/        
    }
 
 //-------------------------------------------------------------------------------------
    public int getChildCount() {
        return getChildCount(this);
    }

 //-------------------------------------------------------------------------------------
    public int getChildCount(Object _parent) {
        if(((HierarchyNode)_parent).equals(this)){
            return childs.size();
        }
        for(int i = 0; i < childs.size(); i++){
            int _childCount = childs.get(i).getChildCount(_parent);
            if(_childCount != -1){
                return _childCount;
            }
        }
        return -1;
/*        File _parentFile;
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
        return 0;*/
    }
 
//-------------------------------------------------------------------------------------
    public boolean isLeaf(Object _objectNode) {
        return isLeafInt(_objectNode) == 1;
    }

//-------------------------------------------------------------------------------------
    public int isLeafInt(Object _objectNode) {
        if(((HierarchyNode)_objectNode).equals(this)){
            return ((HierarchyNode)_objectNode).isLeaf() ? 1 : 0;
        }
        for(int i = 0; i < childs.size(); i++){
            int _isLeafInt = childs.get(i).isLeafInt(_objectNode);
            if(_isLeafInt != -1){
                return _isLeafInt;
            }
        }
        return -1;
/*        HierarchyNode _node = (HierarchyNode)_objectNode;
        if(_node.isRoot()){
            return (projects.size() == 0);
        } else if(_node.isFile()){
            return !((File)_node.getFile()).isDirectory();
        } else if(_node.isProject()){
            return false;
        } else {
            return true;
        }


        Project _nodeProject = ((Project)_node);
        if(_nodeProject.isRoot()){
            return false;
        }
        File f = _nodeProject.getRootFile();
        return !f.isDirectory();*/
    }
//-------------------------------------------------------------------------------------
    public int getIndex(TreeNode _child) {
        return getIndexOfChild(this, _child);
    }
 
//-------------------------------------------------------------------------------------
    public TreeNode getParent() {
        return parentNode;
    }
 
//-------------------------------------------------------------------------------------
    public int getIndexOfChild(Object _parent, Object _child) {
        if(((HierarchyNode)_parent).equals(this)){
            for(int i = 0; i < childs.size(); i++){
                if(childs.get(i).equals((HierarchyNode)_child)){
                    return i;
                }
            }
        }
        for(int i = 0; i < childs.size(); i++){
            int _indexOfChild = childs.get(i).getIndexOfChild(_parent, _child);
            if(_indexOfChild != -1){
                return _indexOfChild;
            }
        }
        return -1;
        /*
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
        return -1;*/
    }

//-------------------------------------------------------------------------------------
    public Enumeration<HierarchyNode> children(){
        return null;
    }

//-------------------------------------------------------------------------------------
    public boolean getAllowsChildren(){
        return true;
    }

//-------------------------------------------------------------------------------------
    public String toString(){
        return name;
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
