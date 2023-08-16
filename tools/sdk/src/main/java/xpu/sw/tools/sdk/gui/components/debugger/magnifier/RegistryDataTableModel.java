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
import xpu.sw.tools.sdk.rexec.remotehandler.*;
//import xpu.sw.tools.sdk.debug.debugger.core.*;

//-------------------------------------------------------------------------------------
public class RegistryDataTableModel extends CommonTableModel {
    private String[] registers;

//-------------------------------------------------------------------------------------
    public RegistryDataTableModel(Gui _gui, Context _context, int _rows, int _columns) {
        super(_gui, _context, _rows, _columns);
        registers = new String[]{"ACC", "ADDRESS", "BOOL", "SHIFT", "IO", "COUNTER"};
    }

//-------------------------------------------------------------------------------------
    public String getColumnName(int _column){
        if(_column == 0){
            return "Register";
        } else {
            return "c" + (startIndex + _column - 1);
        }
    }

//-------------------------------------------------------------------------------------
    public Object getValueAt(int _row, int _column){
        String _value;
        if (_column == 0) {
            _value = registers[_row];
        } else {
            _value = HexFormat.of().toHexDigits(data[_column - 1][_row]);
        }
        return _value.toUpperCase();
    }

//-------------------------------------------------------------------------------------
    public void download(){
        remoteHandler.debugReadArrayRegistry(data, startIndex, stopIndex);
        fireTableDataChanged();
    }

//-------------------------------------------------------------------------------------
    public void setValueAt(Object _value, int _row, int _column){
        super.setValueAt(_value, _row, _column);
        _column--;
        remoteHandler.debugWriteArrayRegistry(data, _column, _column);
        remoteHandler.debugReadArrayRegistry(data, _column, _column);
        fireTableDataChanged();        
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
