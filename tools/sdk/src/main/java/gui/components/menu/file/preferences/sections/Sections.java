//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.components.menu.file.preferences.sections;

//-------------------------------------------------------------------------------------
import java.net.*;
import java.util.*;
import javax.swing.*;
import javax.swing.tree.*;

import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.context.*;

import xpu.sw.tools.sdk.gui.*;
import xpu.sw.tools.sdk.gui.components.menu.file.preferences.*;
import xpu.sw.tools.sdk.gui.components.menu.file.preferences.sections.general.*;
import xpu.sw.tools.sdk.gui.components.menu.file.preferences.sections.editor.*;
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
}
//-------------------------------------------------------------------------------------
