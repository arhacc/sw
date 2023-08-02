//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.components.debugger.magnifier;

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
public class CommonTableModel extends DefaultTableModel {
    protected Gui gui;
    protected Context context;
    protected int rows;
    protected int columns;

    protected int startIndex;
    protected int stopIndex;

    protected org.apache.logging.log4j.Logger log;

    protected org.apache.commons.configuration2.Configuration sdkConfig;

    protected int[][] data;

//-------------------------------------------------------------------------------------
    public CommonTableModel(Gui _gui, Context _context, int _rows, int _columns) {
        gui = _gui;
        context = _context;
        rows = _rows;
        columns = _columns;

        log = _context.getLog();
        sdkConfig = context.getSdkConfig();
        data = new int[_columns][_rows];
//        setBounds(0, _columns + 1);
    }

//-------------------------------------------------------------------------------------
    public int[][] getData() {
        return data;
    }

//-------------------------------------------------------------------------------------
    public int getRowCount() {
        return rows;
    }

//-------------------------------------------------------------------------------------
    public int getColumnCount() {
        int _columnCount = stopIndex - startIndex + 2;
//        log.debug("CommonTableModel: _columnCount=" + _columnCount);
        return _columnCount;
    }

//-------------------------------------------------------------------------------------
    public boolean isCellEditable(int _row, int _column) {
        if(_column == 0){
            return false;
        }
        return true;
    }

/*
//-------------------------------------------------------------------------------------
    public void update(int[][] _data){
        for(int i = 0; i < _data.length; i++){
            for(int j = 0; j < _data[i].length; j++){
                data[i][j] = _data[i][j];
            }
        }
    }
*/

//-------------------------------------------------------------------------------------
    public void setBounds(int _startIndex, int _stopIndex) {
        startIndex = _startIndex;
        stopIndex = _stopIndex;
//        log.debug("Magnifier: setBounds: _startIndex=" + _startIndex + ", _stopIndex=" + _stopIndex);
        fireTableStructureChanged();
        fireTableDataChanged();
    }

//-------------------------------------------------------------------------------------
    
//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
