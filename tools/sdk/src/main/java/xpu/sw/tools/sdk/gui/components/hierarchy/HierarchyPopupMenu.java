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

//-------------------------------------------------------------------------------------
public class HierarchyPopupMenu extends JPopupMenu {
    private Gui gui;
    private JTree jTree;
    private int id;
    private Object node;

//-------------------------------------------------------------------------------------
    public HierarchyPopupMenu(Gui _gui, JTree _jTree, int _id,Object _node) {
        gui = _gui;
        jTree = _jTree;
        id = _id;
        node = _node;
        switch(_id) {
             case 0:{
                //JMenuItems
                JMenuItem _closeProjectMenuItem = new JMenuItem("Close");
                //Add Items
                add(_closeProjectMenuItem);
                //Actions
                _closeProjectMenuItem.addActionListener(new ActionListener() {
                 public void actionPerformed(ActionEvent e) {
                     gui.getMyComponents().getMenu().getMenuHandlers().closeProject();
                 }
               });
                break;
             }
             case 1:{
                 //JMenuItems
                JMenuItem _openFileMenuItem = new JMenuItem("Open File");
                //Add Items
                add(_openFileMenuItem);
                //Actions
                _openFileMenuItem.addActionListener(new ActionListener() {
                 public void actionPerformed(ActionEvent e) {
                     String _filePath = ((File)_node).getAbsolutePath();
                     gui.getMyComponents().getEditor().addTab(_filePath);
                 }
               });
                 break;
             }

             default:{
                 //JMenuItems
                JMenuItem _newProjectMenuItem = new JMenuItem("New");
                JMenuItem _openProjectMenuItem = new JMenuItem("Open");
                JMenuItem _settingsProjectMenuItem = new JMenuItem("Settings");
                //Add Items
                add(_newProjectMenuItem);
                add(_openProjectMenuItem);
                add(new JSeparator());
                add(_settingsProjectMenuItem);
                //Actions
                _newProjectMenuItem.addActionListener(new ActionListener() {
                 public void actionPerformed(ActionEvent e) {
                     gui.getMyComponents().getMenu().getMenuHandlers().newProject();
                 }
               });
                _openProjectMenuItem.addActionListener(new ActionListener() {
                 public void actionPerformed(ActionEvent e) {
                     gui.getMyComponents().getMenu().getMenuHandlers().openProject();
                 }
               });
                _settingsProjectMenuItem.addActionListener(new ActionListener() {
                 public void actionPerformed(ActionEvent e) {
                     gui.getMyComponents().getMenu().getMenuHandlers().settingsProject();
                 }
               });
                 break;
             }
         }
         TreeSelectionModel m = jTree.getSelectionModel();
         if (m != null) {
         m.clearSelection();
         }
         
//-------------------------------------------------------------------------------------
    }      
//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
