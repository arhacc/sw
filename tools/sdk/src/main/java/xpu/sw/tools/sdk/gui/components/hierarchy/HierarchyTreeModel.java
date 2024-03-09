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
public class HierarchyTreeModel extends DefaultTreeModel implements Runnable {
    private Gui gui;
    private Context context;
    private Logger log;
    private JTree jTree;
    private String levelName;

    private TreeSelectionModel treeSelectionModel;
    private String basePath;
//    private HierarchyNode getRoot();

    private HierarchyNode selectedProject;
    private HierarchyNode selectedFile;
    private List<TreeModelListener> listeners;
    private WatchService watchService;

//-------------------------------------------------------------------------------------
    public HierarchyTreeModel(Gui _gui, Context _context, JTree _jTree, String _levelName, String _basePath) {
        super(new HierarchyNode(_gui, _context, null, _basePath));
        gui = _gui;
        context= _context;
        log = _context.getLog();
        jTree = _jTree;
        levelName = _levelName;
        basePath = _basePath;

        treeSelectionModel = jTree.getSelectionModel();
        treeSelectionModel.setSelectionMode(TreeSelectionModel.SINGLE_TREE_SELECTION);
        
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
        getRoot().refresh();
        String _appLevelSelectedProjectPath = context.getSdkConfig().getString("hierarchy." +levelName + ".selectedProject");
        if(_appLevelSelectedProjectPath == null){
            selectedProject = (HierarchyNode)getRoot().getChildAt(0);
        } else {
            selectedProject = getRoot().getNode(_appLevelSelectedProjectPath);            
        }
        String _appLevelSelectedFilePath = context.getSdkConfig().getString("hierarchy." +levelName + ".selectedFile");
        if(_appLevelSelectedFilePath == null) {
            if(selectedProject != null){
                selectedFile = (HierarchyNode)selectedProject.getChildAt(0);
            }
        } else {
            selectedFile = getRoot().getNode(_appLevelSelectedFilePath);            
        }
        log.debug("HierarchyTreeModel: selectedProject=" + selectedProject + ",selectedFile="+selectedFile);
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
                    getRoot().refresh();
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
        return getRoot().getProjects();
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
        log.debug("select project...=" + _projectNode + "/" + _fileNode);
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
    public void setSelectedFile(File _file){
        HierarchyNode _node = getRoot().getNode(_file.getPath());
        log.debug("HierarchyTreeModel.setSelectedFile... _file=" + _file + ", _node=" + _node);

        if(_node != null){
            setSelectedObject(selectedProject, _node);
            refreshSelection();
        }
    }

//-------------------------------------------------------------------------------------
    public HierarchyNode getRoot() {
//        log.debug("getRoot..." + getRoot());
        return (HierarchyNode)root;
    }

//-------------------------------------------------------------------------------------
    public Object getChild(Object _parent, int _index) {
//        log.debug("getChild..."+_parent+", _index="+_index);
        return getRoot().getChild(_parent, _index);
    }
 
//-------------------------------------------------------------------------------------
    public int getChildCount(Object _parent) {
        return getRoot().getChildCount(_parent);
    }
 
//-------------------------------------------------------------------------------------
    public boolean isLeaf(Object _objectNode) {
        return getRoot().isLeaf(_objectNode);
    }
 
//-------------------------------------------------------------------------------------
    public int getIndexOfChild(Object _parent, Object _child) {
        return getRoot().getIndexOfChild( _parent, _child);
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
        TreeModelEvent _e = new TreeModelEvent(getRoot(), new TreePath(getRoot()));
//        log.debug("listeners.size=" + listeners.size());
        listeners.forEach(_l -> {
            _l.treeStructureChanged(_e);
        });
        refreshSelection();
    }

//-------------------------------------------------------------------------------------
    public void refreshSelection() {
        if(selectedFile != null){
            treeSelectionModel.setSelectionPath(new TreePath(selectedFile));            
            context.getSdkConfig().setProperty("hierarchy." + levelName + ".selectedFile", selectedFile.getPath());
        } else if(selectedProject != null){
            treeSelectionModel.setSelectionPath(new TreePath(selectedProject));            
        }
        if(selectedProject != null){
            context.getSdkConfig().setProperty("hierarchy." + levelName + ".selectedProject", selectedProject.getPath());
        }
    }

//-------------------------------------------------------------------------------------
    public void reload() {
        super.reload();
        refreshSelection();
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
