//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.components.hierarchy;

//-------------------------------------------------------------------------------------
import java.io.*;
import java.net.*;
import java.util.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.tree.*;

import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.project.*;
import xpu.sw.tools.sdk.common.xbasics.*;

import xpu.sw.tools.sdk.gui.*;

//-------------------------------------------------------------------------------------
public class HierarchyNode extends XBasic {
    private Gui gui;

    private Project project;
    private File file;
//    private List<HierarchyNode> childs;

//-------------------------------------------------------------------------------------
    public HierarchyNode(Gui _gui, Context _context) {
        this(_gui, _context, null, null);
    }

//-------------------------------------------------------------------------------------
    public HierarchyNode(Gui _gui, Context _context, Project _project) {
        this(_gui, _context, _project, null);
    }

//-------------------------------------------------------------------------------------
    public HierarchyNode(Gui _gui, Context _context, File _file) {
        this(_gui, _context, null, _file);
    }

//-------------------------------------------------------------------------------------
    public HierarchyNode(Gui _gui, Context _context, Project _project, File _file) {
        super(_context);
        gui = _gui;
        project = _project;
        file = _file;
//        childs = new ArrayList<HierarchyNode>();
    }

//-------------------------------------------------------------------------------------
    public boolean isRoot(){
        return (project == null) && (file == null);
    }
    
//-------------------------------------------------------------------------------------
    public boolean isProject(){
        return (project != null);
    }
    
//-------------------------------------------------------------------------------------
    public boolean isFile(){
        return (file != null);
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
    public String toString(){
        if(isProject()){
            return project.toString();
        }
        if(isFile()){
            return file.getName();
        }
        return null;
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
