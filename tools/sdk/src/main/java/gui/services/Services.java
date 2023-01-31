//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.services;

//-------------------------------------------------------------------------------------
import java.net.*;
import java.util.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.lang.reflect.*;

import org.apache.commons.cli.*;
import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.gui.*;
import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.io.targetmanager.*;

import xpu.sw.tools.sdk.gui.services.utils.*;
import xpu.sw.tools.sdk.gui.services.updater.*;

//-------------------------------------------------------------------------------------
public class Services {
    private Gui gui;
    private Context context;
    private Logger log;


    private TargetManager targetManager;
    private Updater updater;
    private Utils utils;

//-------------------------------------------------------------------------------------
    public Services(Gui _gui, Context _context) {
        gui = _gui;
        context = _context;
        log = _context.getLog();
        init();
    }

//-------------------------------------------------------------------------------------
    public TargetManager getTargetManager(){
        return targetManager;
    }

//-------------------------------------------------------------------------------------
    public Updater getUpdater(){
        return updater;
    }

//-------------------------------------------------------------------------------------
    public Utils getUtils(){
        return utils;
    }

//-------------------------------------------------------------------------------------
    protected void init(){
        targetManager = new TargetManager(context);
        updater = new Updater(context);
        utils = new Utils(gui, context);
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
