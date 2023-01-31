//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.components.hierarchy;

//-------------------------------------------------------------------------------------
import java.io.*;
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
public class HierarchyCellRenderer extends DefaultTreeCellRenderer {
    private Gui gui;
    private Context context;
    private Logger log;

    private ImageIcon doneIcon;
    private ImageIcon unknownIcon;
    private ImageIcon waitingIcon;

//-------------------------------------------------------------------------------------
    public HierarchyCellRenderer(Gui _gui, Context _context) {
        gui = _gui;
        context = _context;
        log = _context.getLog();

        try {
//https://www.google.com/imgres?imgurl=https%3A%2F%2Fe7.pngegg.com%2Fpngimages%2F257%2F361%2Fpng-clipart-computer-icons-circle-tinnitus-lullaby-heaven-knows-circle-white-grey.png&imgrefurl=https%3A%2F%2Fwww.pngegg.com%2Fen%2Fpng-nqemd&tbnid=o5J2bKYTNLNGXM&vet=12ahUKEwjfu7GPpJ_zAhVYzSoKHccmAqMQMygLegQIARBf..i&docid=W-a11QmSur9cHM&w=900&h=900&itg=1&q=icons%20circle%20gray%20pngegg&ved=2ahUKEwjfu7GPpJ_zAhVYzSoKHccmAqMQMygLegQIARBf            
            URL _doneUrl = this.getClass().getClassLoader().getResource("flow/circles/green.png");
            URL _unknownUrl = this.getClass().getClassLoader().getResource("flow/circles/red.png");
            URL _waitingUrl = this.getClass().getClassLoader().getResource("flow/circles/gray.png");
            doneIcon = new ImageIcon(_doneUrl);
            unknownIcon = new ImageIcon(_unknownUrl);
            waitingIcon = new ImageIcon(_waitingUrl);
//        } catch(MalformedURLException _e){
        } catch(Exception _e){
            log.debug("Cannot load flow icons!");
        }
    }

//-------------------------------------------------------------------------------------
    public Component getTreeCellRendererComponent(JTree tree, Object _value,
            boolean sel, boolean expanded, boolean leaf, int row,
            boolean hasFocus) {

        if(_value instanceof File){
            _value = ((File)_value).getName();
        }
        super.getTreeCellRendererComponent(tree, _value, sel, expanded, leaf, row, hasFocus);

//        Object nodeObj = ((DefaultMutableTreeNode) value).getUserObject();
//        Object nodeObj = ((DefaultMutableTreeNode) value).getUserObject();
//        setIcon(doneIcon);

        return this;
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
