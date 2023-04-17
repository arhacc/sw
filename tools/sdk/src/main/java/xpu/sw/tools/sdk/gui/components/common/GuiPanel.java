//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.components.common;
//-------------------------------------------------------------------------------------
import java.awt.*;
import java.io.*;
import java.net.*;
import java.util.*;
import javax.swing.*;

import org.apache.commons.cli.*;
import org.apache.commons.configuration2.*;
import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.context.*;

import xpu.sw.tools.sdk.gui.*;

//-------------------------------------------------------------------------------------
public class GuiPanel extends JPanel {
    protected Context context;
    protected Gui gui;

    protected Logger log;
    protected Configuration sdkConfig;

//-------------------------------------------------------------------------------------
    public GuiPanel(Context _context, Gui _gui) {
        context = _context;
        gui = _gui;

        log = _context.getLog();
        sdkConfig = _context.getSdkConfig();
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
