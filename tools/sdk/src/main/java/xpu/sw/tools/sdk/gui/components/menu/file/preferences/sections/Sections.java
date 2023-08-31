//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.components.menu.file.preferences.sections;

//-------------------------------------------------------------------------------------
import java.net.*;
import java.util.*;
import java.awt.*;
import javax.swing.*;
import javax.swing.tree.*;

import org.apache.logging.log4j.*;
import org.apache.commons.configuration2.*;

import xpu.sw.tools.sdk.common.context.*;

import xpu.sw.tools.sdk.gui.*;
import xpu.sw.tools.sdk.gui.components.menu.file.preferences.*;
import xpu.sw.tools.sdk.gui.components.menu.file.preferences.sections.general.*;
import xpu.sw.tools.sdk.gui.components.menu.file.preferences.sections.editor.*;
import xpu.sw.tools.sdk.gui.components.menu.file.preferences.sections.libraries.*;
import xpu.sw.tools.sdk.gui.components.menu.file.preferences.sections.synthesis.*;
import xpu.sw.tools.sdk.gui.components.menu.file.preferences.sections.targets.*;

//-------------------------------------------------------------------------------------
public class Sections {
    private Gui gui;
    private Context context;
    private Logger log;
    private JPanel panel;
    private Preferences preferences;

    private General general;
    private Editor editor;
    private Libraries libraries;
    private Synthesis synthesis;
    private Targets targets;


//-------------------------------------------------------------------------------------
    public Sections(Gui _gui, Context _context, JPanel _panel, Preferences _preferences) {
        gui = _gui;
        context = _context;
        log = _context.getLog();
        panel = _panel;
        preferences = _preferences;
        init();
    }

//-------------------------------------------------------------------------------------
    private void init(){
        general = new General(gui, context, preferences);
        editor = new Editor(gui, context, preferences);
        libraries = new Libraries(gui, context, preferences);
        synthesis = new Synthesis(gui, context);
        targets = new Targets(gui, context);
    }

//-------------------------------------------------------------------------------------
    public void select(String _selectedItem){
        switch(_selectedItem){
            case "General": {
                select(general);
                break;
            }
            case "Editor": {
                select(editor);
                break;
            }
            case "Libraries": {
                select(libraries);
                break;
            }
            case "Synthesis": {
                select(synthesis);
                break;
            }
            case "Targets": {
                select(targets);
                break;
            }
            default:{
                break;
            }
        }
    }

//-------------------------------------------------------------------------------------
    public void select(JPanel _selectedPanel){
        panel.removeAll();
        panel.add(_selectedPanel);
        panel.validate();
        panel.repaint();        
    }

//-------------------------------------------------------------------------------------
    public General getGeneral(){
        return general;
    }

//-------------------------------------------------------------------------------------
    public Editor getEditor(){
        return editor;
    }

//-------------------------------------------------------------------------------------
    public Libraries getLibraries(){
        return libraries;
    }

//-------------------------------------------------------------------------------------
    public Targets getTargets(){
        return targets;
    }

//-------------------------------------------------------------------------------------
    public void save(Component _component){
        general.save(_component);
        editor.save();
        libraries.save();
        targets.save();
    }

//-------------------------------------------------------------------------------------
    public void apply(Component _component){
        general.apply(_component);
        editor.apply();
        libraries.apply();
        targets.apply();
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
