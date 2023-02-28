//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.core;
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

import xpu.sw.tools.sdk.common.xbasics.*;
import xpu.sw.tools.sdk.common.wrappers.version.*;

import xpu.sw.tools.sdk.*;
import xpu.sw.tools.sdk.common.context.*;

import xpu.sw.tools.sdk.gui.*;

//-------------------------------------------------------------------------------------
public class Unit extends XStatus {
    protected Gui gui;
    protected Context context;
    protected Configuration sdkConfig;

//-------------------------------------------------------------------------------------
    public Unit(Gui _gui, Context _context) {
        super(_context);
        gui = _gui;
        context = _context;
        sdkConfig = _context.getSdkConfig();
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
