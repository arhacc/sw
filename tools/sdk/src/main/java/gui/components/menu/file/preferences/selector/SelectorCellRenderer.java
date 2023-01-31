//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.components.menu.file.preferences.selector;

//-------------------------------------------------------------------------------------
import java.awt.*;
import java.net.*;
import java.util.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.tree.*;

import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.context.*;

import xpu.sw.tools.sdk.gui.*;

//-------------------------------------------------------------------------------------
public class SelectorCellRenderer extends DefaultTreeCellRenderer {
    private Gui gui;
    private Context context;
    private Logger log;

    private ImageIcon doneIcon;
    private ImageIcon unknownIcon;
    private ImageIcon waitingIcon;

//-------------------------------------------------------------------------------------
    public SelectorCellRenderer(Gui _gui, Context _context) {
        gui = _gui;
        context = _context;
        log = _context.getLog();

        try {
            URL _doneUrl = this.getClass().getClassLoader().getResource("flow/circles/green.png");
            URL _unknownUrl = this.getClass().getClassLoader().getResource("flow/circles/red.png");
            URL _waitingUrl = this.getClass().getClassLoader().getResource("flow/circles/red.png");
            doneIcon = new ImageIcon(_doneUrl);
            unknownIcon = new ImageIcon(_unknownUrl);
            waitingIcon = new ImageIcon(_waitingUrl);
//        } catch(MalformedURLException _e){
        } catch(Exception _e){
            log.debug("Cannot load flow icons!");
        }
    }

//-------------------------------------------------------------------------------------
    public Component getTreeCellRendererComponent(JTree tree, Object value,
            boolean sel, boolean expanded, boolean leaf, int row,
            boolean hasFocus) {

        super.getTreeCellRendererComponent(tree, value, sel, expanded, leaf,
                row, hasFocus);

//        Object nodeObj = ((DefaultMutableTreeNode) value).getUserObject();
//        Object nodeObj = ((DefaultMutableTreeNode) value).getUserObject();
        setIcon(null);
        return this;
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
