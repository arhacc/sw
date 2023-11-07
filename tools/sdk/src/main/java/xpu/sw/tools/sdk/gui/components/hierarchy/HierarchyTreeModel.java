//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.components.hierarchy;

//-------------------------------------------------------------------------------------
import java.io.*;
import java.net.*;
import java.nio.file.*;
import com.sun.nio.file.*;
import static java.nio.file.StandardWatchEventKinds.*;
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
public class HierarchyTreeModel implements TreeModel, Runnable {
    private Gui gui;
    private Context context;
    private Logger log;

    private String basePath;
    private HierarchyNode root;

//    private List<HierarchyNode> projects;
    private HierarchyNode selectedProject;
    private HierarchyNode selectedFile;
    private List<TreeModelListener> listeners;
    private WatchService watchService;

//-------------------------------------------------------------------------------------
    public HierarchyTreeModel(Gui _gui, Context _context, String _basePath) {
        gui = _gui;
        context= _context;
        log = _context.getLog();
        basePath = _basePath;

        root = new HierarchyNode(_gui, _context, _basePath);
        listeners = new ArrayList<TreeModelListener>();

        try {
            watchService = FileSystems.getDefault().newWatchService();
            Path path = Paths.get(basePath);
            path.register(watchService, new WatchEvent.Kind[]{ENTRY_CREATE, ENTRY_MODIFY, ENTRY_DELETE}, SensitivityWatchEventModifier.HIGH);
//            watchEvent(watchService, path);
//            log.info("Watch Service has ben created!");
        } catch (IOException _e) {
            log.error("Exception has ben throw when the service have tried to createWatchService()", _e);
        }
        new Thread(this).start();        
    }

//-------------------------------------------------------------------------------------
    public void run(){
        root.refresh();
        fireChange();
        WatchKey key;
        while (true) {
             try {
                 if ((key = watchService.take()) == null) break;

                 for (WatchEvent<?> event : key.pollEvents()) {
                     log.info("Event kind:" + event.kind()
                             + ". File affected: " + event.context() + ".");

//                     String fileName = event.context().toString();
//                     File directory = path.toFile();
                    root.refresh();
                    fireChange();
                 }
                 key.reset();
            } catch (InterruptedException e) {
                 log.error("InterruptedException when try watchEvent()" + e);
            }
        }
}

//-------------------------------------------------------------------------------------
    public List<Project> getProjects(){
        return root.getProjects();
    }
/*
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
    public void addProject(Project _project){
        root.addProject(_project);
        fireChange();
    }
    
//-------------------------------------------------------------------------------------
    public void removeProject(Project _project){
        root.removeProject(selectedProject);
        fireChange();
    }
 */   
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
        return root.getChild(_parent, _index);
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
    public int getChildCount(Object _parent) {
        return root.getChildCount(_parent);
    }
 
//-------------------------------------------------------------------------------------
    public boolean isLeaf(Object _objectNode) {
        return root.isLeaf(_objectNode);
    }
 
//-------------------------------------------------------------------------------------
    public int getIndexOfChild(Object _parent, Object _child) {
        return root.getIndexOfChild( _parent, _child);
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
