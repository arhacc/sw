//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.components.common.panels;

//-------------------------------------------------------------------------------------
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.net.*;
import java.util.*;
import javax.swing.*;
import javax.imageio.*;

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
import xpu.sw.tools.sdk.gui.components.debugger.magnifier.*;
//import xpu.sw.tools.sdk.debug.debugger.core.*;
import xpu.sw.tools.sdk.common.io.targetmanager.*;

//-------------------------------------------------------------------------------------
public class EnhancedJTabbedPane extends javax.swing.JPanel {
    private Gui gui;
    private Context context;
    private String configWindow;
    private JComponent parent;

    private org.apache.logging.log4j.Logger log;
    private org.apache.commons.configuration2.Configuration sdkConfig;

    private JTabbedPane jTabbedPane1;
    private JButton minMaxButton;
    private ImageIcon minIcon;
    private ImageIcon maxIcon;
    private boolean windowState;
    private JFrame expandedJFrame;

    private static final boolean WINDOW_STATE_COLLAPSED     = false;
    private static final boolean WINDOW_STATE_EXPANDED      = true;
//-------------------------------------------------------------------------------------
    public EnhancedJTabbedPane(Gui _gui, Context _context, String _configWindow, JComponent _parent) {
        super();
        gui = _gui;
        context = _context;
        configWindow = _configWindow;
        parent = _parent;

        log = _context.getLog();
        sdkConfig = context.getSdkConfig();
        initComponents();
    }

//-------------------------------------------------------------------------------------
    private void initComponents() {
//        setAutoscrolls(true);
//        setMinimumSize(new java.awt.Dimension(0, 0));
//        setOpaque(false);
        windowState = sdkConfig.getBoolean(configWindow + ".state", false);
        setLayout(new javax.swing.OverlayLayout(this));
        try {
            minIcon = gui.getServices().getUtils().getIconFromResources("arrows/minimize_16x16.png");
            maxIcon = gui.getServices().getUtils().getIconFromResources("arrows/expand-arrows_16x16.png");
        }catch(Exception _e){
            log.error("Cannot load min/maxIcon!");
        }


        jTabbedPane1 = new JTabbedPane();
        jTabbedPane1.setTabLayoutPolicy(JTabbedPane.WRAP_TAB_LAYOUT);
//        jTabbedPane1.addTab("Magnifier", new Magnifier(gui, context));
        jTabbedPane1.setAlignmentX(1.0f);
        jTabbedPane1.setAlignmentY(0.0f);
//        gui.getServices().getTargetManager().addStatusListener(this);

        minMaxButton = new JButton("");
        minMaxButton.setOpaque( false );
        minMaxButton.setAlignmentX(1.0f);
        minMaxButton.setAlignmentY(0.0f);
        repaintminMaxButton();
//        minMaxButton.setIcon((windowState == WINDOW_STATE_COLLAPSED) ? maxIcon : minIcon);
        minMaxButton.addActionListener(new ActionListener() { 
            public void actionPerformed(ActionEvent e) { 
//                log.debug("minMaxButtonPressed...");
                minMaxButtonPressed();
            } 
        });
        add(minMaxButton);
        add(jTabbedPane1);
        if(windowState == WINDOW_STATE_EXPANDED){
            expandWindow();
        }
    }

//-------------------------------------------------------------------------------------
    public void addTab(String _name, Component _tab){
        jTabbedPane1.addTab(_name, _tab);
    }

//-------------------------------------------------------------------------------------
    public synchronized void minMaxButtonPressed() {
        if(windowState == WINDOW_STATE_COLLAPSED){
            expandWindow();
        } else {
            collapseWindow();
        }
        repaintminMaxButton();
        sdkConfig.setProperty(configWindow + ".state", windowState);
    }

//-------------------------------------------------------------------------------------
    public void expandWindow() {
/*        setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        getRootPane().getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(
            KeyStroke.getKeyStroke(KeyEvent.VK_ESCAPE, 0), "Cancel"); //$NON-NLS-1$
        getRootPane().getActionMap().put("Cancel", new AbstractAction() {*/
//            log.debug("Create frame...");

//            GraphicsDevice gd = GraphicsEnvironment.getLocalGraphicsEnvironment().getDefaultScreenDevice();
        if(expandedJFrame == null){
            expandedJFrame = new JFrame("");
            expandedJFrame.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
        }
        expandedJFrame.getContentPane().add(this, BorderLayout.CENTER);
//        expandedJFrame.setSize();
        int _x = sdkConfig.getInteger(configWindow + ".expanded.location.x", 0);
        int _y = sdkConfig.getInteger(configWindow + ".expanded.location.y", 0);
        expandedJFrame.setLocation(new Point(_x, _y));

        sdkConfig.getDouble(configWindow + ".expanded.dimension.x");
        sdkConfig.getDouble(configWindow + ".expanded.dimension.y");
        expandedJFrame.setSize(new Dimension(_x, _y));
        

        expandedJFrame.pack();
        parent.remove(this);
        parent.revalidate();
        parent.repaint();
        expandedJFrame.setVisible(true);
//        expandedJFrame.setLocation();
//        expandedJFrame.setSize();

//            gd.setFullScreenWindow(_jFrame);
/*
            public void actionPerformed(ActionEvent e) {
                log.debug("Escape...");
                save();
                //framename.setVisible(false);
//                System.exit(0);
            }
        });*/
        windowState = WINDOW_STATE_EXPANDED;
    }

//-------------------------------------------------------------------------------------
    public void collapseWindow() {
        if(expandedJFrame != null){
            sdkConfig.setProperty(configWindow + ".expanded.location.x", expandedJFrame.getLocation().x);
            sdkConfig.setProperty(configWindow + ".expanded.location.y", expandedJFrame.getLocation().y);
            sdkConfig.setProperty(configWindow + ".expanded.dimension.x", expandedJFrame.getContentPane().getSize().getWidth());
            sdkConfig.setProperty(configWindow + ".expanded.dimension.y", expandedJFrame.getContentPane().getSize().getHeight());

            expandedJFrame.getContentPane().remove(this);
            expandedJFrame.pack();
            expandedJFrame.setVisible(false);
            parent.add(this);
            parent.revalidate();
            parent.repaint();
        } else {
            log.error("Illegal state of expandedJFrame in EnhancedJTabbedPane!");
        }
        windowState = WINDOW_STATE_COLLAPSED;
    }

//-------------------------------------------------------------------------------------
    private void repaintminMaxButton() {
        minMaxButton.setIcon((windowState == WINDOW_STATE_COLLAPSED) ? maxIcon : minIcon);
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
