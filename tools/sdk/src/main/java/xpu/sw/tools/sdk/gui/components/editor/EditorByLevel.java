//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.components.editor;
//-------------------------------------------------------------------------------------
import java.awt.*;
import java.io.*;
import java.nio.*;
import java.nio.file.*;
import java.net.*;
import java.util.*;
import javax.swing.*;

import org.apache.commons.configuration2.*;
import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.project.*;

import xpu.sw.tools.sdk.gui.*;
import xpu.sw.tools.sdk.gui.components.common.*;
import xpu.sw.tools.sdk.gui.components.common.panels.*;
import xpu.sw.tools.sdk.gui.components.common.buttons.*;

//-------------------------------------------------------------------------------------

/**
 *
 * @author marius
 */
public class EditorByLevel extends GuiPanel implements CloseTabListener {
    private String level;
    private Project activeProject;
    private Editor editor;
    private Map<Project, EditorByProject> editorByProjects;
    private CardLayout cardLayout;

    private Configuration sdkConfig;
    private String themeName;

    /**
     * Creates new form Editor
     */
    public EditorByLevel(Gui _gui, Context _context, String _level, Editor _editor) {
        super(_context, _gui);
        level = _level;
//        activeProject = _activeProject;
        editor = _editor;
        initComponents();
        _gui.getEditor().add(this);
        editorByProjects = new HashMap<Project, EditorByProject>();
        sdkConfig = context.getSdkConfig();
        
        setVisible(true);
        init();
    }

//-------------------------------------------------------------------------------------
    private void initComponents() {
        cardLayout = new CardLayout();
        setLayout(cardLayout);
    }

//-------------------------------------------------------------------------------------
    private void init(){
        String _theme = sdkConfig.getString("editor_theme");
        if(_theme != null){
            setTheme(_theme);
        }        
        String _pathToActiveProject = sdkConfig.getString("pathToActiveProject");
        if((_pathToActiveProject != null) && !_pathToActiveProject.isEmpty()){
            setActiveProject(new Project(context, _pathToActiveProject));
        }
    }

//-------------------------------------------------------------------------------------
    public EditorByProject getActiveEditor(){
        EditorByProject _editorByProject = editorByProjects.get(activeProject);
        return _editorByProject;
    }

//-------------------------------------------------------------------------------------
    public int addTab(String _filePath){    
        return getActiveEditor().addTab(new File(_filePath));
//        jTabbedPane1.setSelectedIndex(jTabbedPane1.getTabCount() - 1);
    }

//-------------------------------------------------------------------------------------
    public int addTab(File _file){
        return getActiveEditor().addTab(_file);
    }

//-------------------------------------------------------------------------------------
    public void removeTab(int _index){
        getActiveEditor().removeTab(_index);        
    }

//-------------------------------------------------------------------------------------
    public void closeTab(int _index){
        getActiveEditor().closeTab(_index);
    }

//-------------------------------------------------------------------------------------
    public void removeAllTabs(){
        getActiveEditor().removeAllTabs();
    }

//-------------------------------------------------------------------------------------
    public String getSelectedFilename(){
        return getActiveEditor().getSelectedFilename();
    }
//-------------------------------------------------------------------------------------
    public String getThemeName(){
        return themeName;
    }

//-------------------------------------------------------------------------------------
    public void setTheme(String _themeName){
        themeName = _themeName;
        editorByProjects.values().forEach(_editorByProject -> {
            _editorByProject.setTheme(_themeName);
        });
    }

//-------------------------------------------------------------------------------------
    public void saveCurentTab(){
        getActiveEditor().saveCurentTab();
    }

//-------------------------------------------------------------------------------------
    public void setActiveProject(Project _activeProject){
        activeProject = _activeProject;
        EditorByProject _editorByProject = getActiveEditor();
        if(_editorByProject == null){
            _editorByProject = new EditorByProject(gui, context, _activeProject, this);
            editorByProjects.put(_activeProject, _editorByProject);
            add(_activeProject.getPathToConfigFile(), _editorByProject);
        }
//        log.debug("cardLayout.show:" + _editorByProject.toString());
        cardLayout.show(this, _activeProject.getPathToConfigFile());        
    }

//-------------------------------------------------------------------------------------
  
}
//-------------------------------------------------------------------------------------
