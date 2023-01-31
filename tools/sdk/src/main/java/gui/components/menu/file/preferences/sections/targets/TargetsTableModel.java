//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.components.menu.file.preferences.sections.targets;

//-------------------------------------------------------------------------------------
import java.awt.*;
import java.io.*;
import java.net.*;
import java.util.*;
import javax.swing.*;
import javax.swing.table.*;

import org.apache.commons.configuration2.*;
import org.apache.logging.log4j.*;
import org.apache.logging.log4j.core.*;
import org.apache.logging.log4j.core.appender.*;
import org.apache.logging.log4j.core.config.*;
import org.apache.logging.log4j.core.layout.*;
import org.apache.logging.log4j.core.appender.rolling.*;

import xpu.sw.tools.sdk.common.context.*;

import xpu.sw.tools.sdk.gui.*;
import xpu.sw.tools.sdk.gui.components.common.buttons.*;
//import xpu.sw.tools.sdk.debug.debugger.core.*;

//-------------------------------------------------------------------------------------
public class TargetsTableModel extends DefaultTableModel {
    protected Gui gui;
    protected Context context;


    protected org.apache.logging.log4j.Logger log;

    protected org.apache.commons.configuration2.Configuration sdkConfig;

    protected java.util.List<String[]> targets;

//-------------------------------------------------------------------------------------
    public TargetsTableModel(Gui _gui, Context _context) {
        gui = _gui;
        context = _context;

        log = _context.getLog();
        sdkConfig = context.getSdkConfig();
        targets = new ArrayList<String[]>();
//        setBounds(0, _columns + 1);
    }

//-------------------------------------------------------------------------------------
    public int getRowCount() {
        if(targets == null){
            return 0;
        }
        return targets.size();
    }

//-------------------------------------------------------------------------------------
    public int getColumnCount() {
        return 5;
    }

//-------------------------------------------------------------------------------------
    public boolean isCellEditable(int _row, int _column) {
        return false;
    }

//-------------------------------------------------------------------------------------
    public String getColumnName(int _column){
        switch(_column){
            case 0 : return "Id";
            case 1 : return "Name";
            case 2 : return "Type";
            case 3 : return "Hostname";
            case 4 : return "Port";
            default : return "ERROR";
        }
    }

//-------------------------------------------------------------------------------------
    public Object getValueAt(int _row, int _column){
        return targets.get(_row)[_column];
    }

//-------------------------------------------------------------------------------------
    public void setTargets(java.util.List<String> _targets){
        targets.clear();
        for(int i = 0; i < _targets.size(); i++){
            targets.add(_targets.get(i).split(","));
        }
/*        if(targets == null){
            return;
        }
        if(_targets != null){
//            TableModel _tableModel = jTable1.getModel();
            for(int i = 0; i < _targets.size(); i++){
                String[] _value = _targets.get(i).split(",");
                targetsTableModel.setValueAt(_value[0], i, 0);
                targetsTableModel.setValueAt(_value[1], i, 1);
                targetsTableModel.setValueAt(_value[2], i, 2);
                targetsTableModel.setValueAt(_value[3], i, 3);
                targetsTableModel.setValueAt(_value[4], i, 4);
            }
        }*/
    }
/*
//-------------------------------------------------------------------------------------
    public void setValueAt(Object _value, int _row, int _column){
        if(data == null){
            return;
        }
        String _data.get(_row);
    }
*/
//-------------------------------------------------------------------------------------
    
//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
