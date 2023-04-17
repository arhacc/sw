//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.components.menu.file.preferences.sections.general;
//-------------------------------------------------------------------------------------
import java.net.*;
import java.util.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.lang.reflect.*;

import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.gui.*;

//-------------------------------------------------------------------------------------
public class Theme {
    private javax.swing.UIManager.LookAndFeelInfo lookAndFeelInfo;

//-------------------------------------------------------------------------------------
    public Theme(javax.swing.UIManager.LookAndFeelInfo _lookAndFeelInfo){
        lookAndFeelInfo = _lookAndFeelInfo;
    }

//-------------------------------------------------------------------------------------
    public javax.swing.UIManager.LookAndFeelInfo getLookAndFeelInfo(){
        return lookAndFeelInfo;
    }

//-------------------------------------------------------------------------------------
    public String getClassName(){
        return lookAndFeelInfo.getClassName();
    }

//-------------------------------------------------------------------------------------
    public String toString(){
        return lookAndFeelInfo.getName();
    }
//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
