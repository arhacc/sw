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
public class Editor extends GuiPanel implements CloseTabListener {
    private String activeLevel;
    private Map<String, EditorByLevel> editorByLevels;
    private CardLayout cardLayout;

    private Configuration sdkConfig;
    private String themeName;

    /**
     * Creates new form Editor
     */
    public Editor(Gui _gui, Context _context) {
        super(_context, _gui);
        initComponents();
        _gui.getEditor().add(this);
        editorByLevels = new HashMap<String, EditorByLevel>();
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
        String _level = sdkConfig.getString("profile");
        if((_level != null) && !_level.isEmpty()){
            setActiveLevel(_level);
        }
    }

//-------------------------------------------------------------------------------------
    public EditorByLevel getActiveEditor(){
        EditorByLevel _editorByLevel = editorByLevels.get(activeLevel);
        return _editorByLevel;
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
    public void selectTab(String _filePath){
        getActiveEditor().selectTab(_filePath);
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
        editorByLevels.values().forEach(_editorByLevel -> {
            _editorByLevel.setTheme(_themeName);
        });
    }

//-------------------------------------------------------------------------------------
    public void saveCurentTab(){
        getActiveEditor().saveCurentTab();
    }

//-------------------------------------------------------------------------------------
    public void setActiveLevel(String _activeLevel){
        activeLevel = _activeLevel;
        EditorByLevel _editorByLevel = getActiveEditor();
        if(_editorByLevel == null){
            _editorByLevel = new EditorByLevel(gui, context, _activeLevel, this);
            editorByLevels.put(_activeLevel, _editorByLevel);
            add(_activeLevel, _editorByLevel);
        }
//        log.debug("cardLayout.show:" + _editorByProject.toString());
        cardLayout.show(this, _activeLevel);        
    }

//-------------------------------------------------------------------------------------
  
}
//-------------------------------------------------------------------------------------
