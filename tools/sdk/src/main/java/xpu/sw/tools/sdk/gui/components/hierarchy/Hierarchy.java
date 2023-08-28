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
public class Hierarchy extends GuiBasic {
    private JPanel jPanel;
    private String selectedProfileLevel;
    private Map<String, HierarchyByLevel> hierarchyByLevels;
    private CardLayout cardLayout;

//-------------------------------------------------------------------------------------
    public Hierarchy(Context _context, Gui _gui) {
        super(_context, _gui);
        jPanel = _gui.getHierarchy();
        init();
    }

//-------------------------------------------------------------------------------------
    private void init(){
        cardLayout = new CardLayout();
        jPanel.setLayout(cardLayout);
        hierarchyByLevels = new HashMap<String, HierarchyByLevel>();

        addLevel(Context.PROFILE_LEVEL_LOW);
        addLevel(Context.PROFILE_LEVEL_MID);
        addLevel(Context.PROFILE_LEVEL_APP);

        String _selectedProfileLevel = sdkConfig.getString("selectedProfileLevel", Context.PROFILE_LEVEL_APP);
        setProfileLevel(_selectedProfileLevel);
/*        JTree jTree = new JTree();
        jPanel.add("t", jTree);
        cardLayout.show(jPanel, "t");*/
    }

//-------------------------------------------------------------------------------------
    private void addLevel(String _level){
        HierarchyByLevel _hierarchyLevel = new HierarchyByLevel(context, gui, _level);
        hierarchyByLevels.put(_level, _hierarchyLevel);
        jPanel.add(_level, _hierarchyLevel);
    }

//-------------------------------------------------------------------------------------
    public String getSelectedProfileLevel(){
        return selectedProfileLevel;
    }

//-------------------------------------------------------------------------------------
    public void setProfileLevel(String _profileLevel){
        selectedProfileLevel = _profileLevel;
        sdkConfig.setProperty("selectedProfileLevel", _profileLevel);
        HierarchyByLevel _selectedHierarchyLevel = hierarchyByLevels.get(_profileLevel);
        setVisibleSelectedHierarchyLevel(_profileLevel);
    }

//-------------------------------------------------------------------------------------
    public HierarchyByLevel getSelectedHierarchyLevel(){
        return hierarchyByLevels.get(selectedProfileLevel);
    }

//-------------------------------------------------------------------------------------
    private void setVisibleSelectedHierarchyLevel(String _profileLevel){
        cardLayout.show(jPanel, _profileLevel);
    }

//-------------------------------------------------------------------------------------    
    public void addProject(Project _project){
        addProject(_project, false);
    }
    
//-------------------------------------------------------------------------------------
    public void addProject(Project _project, boolean _addToConfig){
        getSelectedHierarchyLevel().addProject(_project, _addToConfig);
    }
    
//-------------------------------------------------------------------------------------
    public void removeProject(Project _project){
        getSelectedHierarchyLevel().removeProject(_project);
    }

//-------------------------------------------------------------------------------------
    public String getSelectedProjectPath(){
        return getSelectedHierarchyLevel().getSelectedProjectPath();
    }

//-------------------------------------------------------------------------------------
    public Project getSelectedProject(){
        return getSelectedHierarchyLevel().getSelectedProject();
    }

//-------------------------------------------------------------------------------------
    public java.util.List<Project> getSelectedProjects(){
        return getSelectedHierarchyLevel().getProjects();
    }

//-------------------------------------------------------------------------------------
    public File getSelectedFile(){
        return getSelectedHierarchyLevel().getSelectedFile();
    }

//-------------------------------------------------------------------------------------
    public void refresh(){
        getSelectedHierarchyLevel().refresh();
    }

//-------------------------------------------------------------------------------------

}
//-------------------------------------------------------------------------------------
