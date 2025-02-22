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
import xpu.sw.tools.sdk.gui.components.debugger.*;
import xpu.sw.tools.sdk.rexec.remotehandler.*;
//import xpu.sw.tools.sdk.debug.debugger.core.*;

//-------------------------------------------------------------------------------------
public class MemoryDataTableModel extends CommonTableModel {

//-------------------------------------------------------------------------------------
    public MemoryDataTableModel(Gui _gui, Context _context, Debugger _debugger, int _rows, int _columns) {
        super(_gui, _context, _debugger, _rows, _columns);
    }

//-------------------------------------------------------------------------------------
    public String getColumnName(int _column){
        if(_column == 0){
            return "Address";
        } else if(_column == 1){
            return "ctrl";
        } else {
            return "c" + (startIndex + _column - 2);
        }
    }

//-------------------------------------------------------------------------------------
    public Object getValueAt(int _row, int _column){
        String _value;
        if (_column == 0) {
            _value = HexFormat.of().toHexDigits((short)_row)  + ":";   
        } else {
            int _valueRaw = data[_column - 1][_row];
            _value = (debugger.getViewDataMode() == Debugger.VIEW_DATA_MODE_HEX) ? HexFormat.of().toHexDigits(_valueRaw) : String.valueOf(_valueRaw);
//            _value = HexFormat.of().toHexDigits(data[_column - 1][_row]);
        }
        return _value.toUpperCase();
    }

//-------------------------------------------------------------------------------------
    public void download(){
        remoteHandler.debugReadMemoryData(data, startIndex, stopIndex, 0, 1023);
        fireTableDataChanged();
    }
    
//-------------------------------------------------------------------------------------
    public void setValueAt(Object _value, int _row, int _column){
        super.setValueAt(_value, _row, _column);
        _column--;
        remoteHandler.debugWriteArrayMemoryData(data, _column, _column, _row, _row);
        remoteHandler.debugReadMemoryData(data, _column, _column, _row, _row);
        fireTableDataChanged();        
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
