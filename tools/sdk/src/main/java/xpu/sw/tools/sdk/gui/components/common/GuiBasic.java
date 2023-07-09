//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.components.common;
//-------------------------------------------------------------------------------------
import java.awt.*;
import java.awt.event.*;
import java.net.*;
import java.io.*;
import java.nio.channels.*;
import java.util.*;
import javax.swing.*;
import java.lang.reflect.*;

import org.apache.commons.configuration2.*;
import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.*;
import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.xbasics.*;
import xpu.sw.tools.sdk.common.context.version.*;

import xpu.sw.tools.sdk.gui.*;

//-------------------------------------------------------------------------------------
public class GuiBasic extends XBasic {
    protected Gui gui;
    protected Configuration sdkConfig;

//-------------------------------------------------------------------------------------
    public GuiBasic(Context _context, Gui _gui) {
        super(_context);
        context = _context;
        gui = _gui;
        sdkConfig = _context.getSdkConfig();
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
