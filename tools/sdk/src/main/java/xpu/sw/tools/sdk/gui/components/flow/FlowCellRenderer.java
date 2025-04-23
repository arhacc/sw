//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.components.flow;

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
public class FlowCellRenderer extends DefaultTreeCellRenderer {
    private Gui gui;
    private Context context;
    private Logger log;

    private ImageIcon unknownIcon;
    private ImageIcon waitingIcon;
    private ImageIcon doneIcon;
    private ImageIcon errorIcon;

    private ImageIcon[] icons;
//-------------------------------------------------------------------------------------
    public FlowCellRenderer(Gui _gui, Context _context) {
        gui = _gui;
        context = _context;
        log = _context.getLog();

        try {
//https://www.google.com/imgres?imgurl=https%3A%2F%2Fe7.pngegg.com%2Fpngimages%2F257%2F361%2Fpng-clipart-computer-icons-circle-tinnitus-lullaby-heaven-knows-circle-white-grey.png&imgrefurl=https%3A%2F%2Fwww.pngegg.com%2Fen%2Fpng-nqemd&tbnid=o5J2bKYTNLNGXM&vet=12ahUKEwjfu7GPpJ_zAhVYzSoKHccmAqMQMygLegQIARBf..i&docid=W-a11QmSur9cHM&w=900&h=900&itg=1&q=icons%20circle%20gray%20pngegg&ved=2ahUKEwjfu7GPpJ_zAhVYzSoKHccmAqMQMygLegQIARBf            
            URL _unknownUrl = this.getClass().getClassLoader().getResource("flow/circles/gray.png");
            URL _waitingUrl = this.getClass().getClassLoader().getResource("flow/circles/yellow.png");
            URL _doneUrl = this.getClass().getClassLoader().getResource("flow/circles/green.png");
            URL _errorUrl = this.getClass().getClassLoader().getResource("flow/circles/red.png");
            unknownIcon = new ImageIcon(_unknownUrl);
            waitingIcon = new ImageIcon(_waitingUrl);
            doneIcon = new ImageIcon(_doneUrl);
            errorIcon = new ImageIcon(_errorUrl);
//        } catch(MalformedURLException _e){
        } catch(Exception _e){
            log.debug("Cannot load flow icons!");
        }
        icons = new ImageIcon[4];
        icons[Flow.STATUS_UNKNOWN] = unknownIcon;
        icons[Flow.STATUS_WAITING] = waitingIcon;
        icons[Flow.STATUS_DONE] = doneIcon;
        icons[Flow.STATUS_ERROR] = errorIcon;
    }

//-------------------------------------------------------------------------------------
    public Component getTreeCellRendererComponent(JTree tree, Object value,
            boolean sel, boolean expanded, boolean leaf, int row,
            boolean hasFocus) {

        super.getTreeCellRendererComponent(tree, value, sel, expanded, leaf,
                row, hasFocus);
//        log.debug("value="+value + ", valueClass="+value.getClass().getName());
        if(value instanceof String){
            return this;
        }

        Object nodeObj = ((DefaultMutableTreeNode) value).getUserObject();
        if(nodeObj instanceof Action){
            Action _action = (Action) nodeObj;
            setIcon(icons[_action.getStatus()]);            
        }
        return this;
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
