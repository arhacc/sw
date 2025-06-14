//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.services.utils;
//-------------------------------------------------------------------------------------

import xpu.sw.tools.sdk.common.utils.StringUtils;
import java.awt.*;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;
import javax.imageio.ImageIO;
import javax.swing.ImageIcon;
import javax.swing.SwingUtilities;
import javax.swing.UIManager;
import org.apache.commons.io.FilenameUtils;
import org.apache.logging.log4j.Logger;

import com.formdev.flatlaf.*;

import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.io.*;
import xpu.sw.tools.sdk.gui.Gui;

//-------------------------------------------------------------------------------------
public class Utils {

    private Gui gui;
    private Context context;
    private Logger log;
    private String currentLFClass;
    private static final java.util.Map<String, String> lafMap = createLAFMap();

//-------------------------------------------------------------------------------------
    public Utils(Gui _gui, Context _context) {
        gui = _gui;
        context = _context;
        log = _context.getLog();
        currentLFClass = "";
    }

//-------------------------------------------------------------------------------------
    public void loadLF(Component _gui) {
        String _lf = context.getSdkConfig().getString("lf");
        try {
            if (_lf != null) {
                selectLF(_lf, _gui);
            }
        } catch (Exception _e) {
            log.error("Cannot load look and feel: " + _lf + " : " + _e.getMessage());
        }
    }

//-------------------------------------------------------------------------------------
    private static java.util.Map<String, String> createLAFMap() {
        UIManager.LookAndFeelInfo[] _lfs = UIManager.getInstalledLookAndFeels();
        java.util.Map<String, String> _themes = new java.util.HashMap<String, String>();
        for (int i = 0; i < _lfs.length; i++) {
            _themes.put(_lfs[i].getName(), _lfs[i].getClassName());
        }
        _themes.put("MaterialLookAndFeel", "mdlaf.MaterialLookAndFeel");
        FlatLightLaf.setup();
        _themes.put("Light",            "com.formdev.flatlaf.FlatLightLaf");
        _themes.put("Dark",             "com.formdev.flatlaf.FlatDarkLaf");
        _themes.put("IntelliJ",         "com.formdev.flatlaf.FlatIntelliJLaf");
        _themes.put("Darcula",          "com.formdev.flatlaf.FlatDarculaLaf");
        _themes.put("macOS Light v3",   "com.formdev.flatlaf.themes.FlatMacLightLaf");
        _themes.put("macOS Dark v3",    "com.formdev.flatlaf.themes.FlatMacDarkLaf");       
        return _themes;
    }

//-------------------------------------------------------------------------------------
    public java.util.List<String> getAvailableLFs() {
        return new ArrayList<String>(lafMap.keySet());
    }

//-------------------------------------------------------------------------------------
    public void selectLF(String _lf, Component _gui) {
        String _lfClassName = getLFClass(_lf);
        setLF(_lfClassName, _gui);
    }

//-------------------------------------------------------------------------------------
    public String getLFClass(String _themeName) {
        return lafMap.get(_themeName);
    }

//-------------------------------------------------------------------------------------
    private synchronized void setLF(String _lfClass, Component _gui) {
        try {
            if (!currentLFClass.equals(_lfClass)) {
//                log.info("Selecting theme[" + _themeClass + "]...");
                UIManager.setLookAndFeel(_lfClass);
                SwingUtilities.updateComponentTreeUI(_gui);
//                _gui.pack();
                currentLFClass = _lfClass;
            }
        } catch (Exception _e) {
            log.error("Cannot setLookAndFeel class: " + _lfClass + ": " + _e.getMessage());
        }
    }

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
    public java.util.List<String> getAvailableThemes() {
        java.util.List<String> _themes = new ArrayList<String>();

//        File _file = new File("editor/themes/");
        String _path = "editor/themes";
//        String _path = "./";
/*        try{
            ClassLoader _loader = Thread.currentThread().getContextClassLoader();
            InputStream _inputStream = _loader.getResourceAsStream(_path);
            InputStreamReader _inputStreamReader = new InputStreamReader(_inputStream, StandardCharsets.UTF_8);
            BufferedReader _br = new BufferedReader(_inputStreamReader);
            return _br.lines()
                .map(_l -> {
                    log.debug("_l=" + _l);
                    return _path + "/" + _l;
                    })
                .map(_r -> {
                    log.debug("_r=" + _r);
                    return _loader.getResource(_r);
                })
                .map(_u -> _u.toString())
                .map(_f -> FilenameUtils.removeExtension(_f))
                .collect(Collectors.toList());
        } catch(Exception _e){
            log.error("No themes found " + _path + "!!!" + _e.getMessage());
            _e.printStackTrace();
        }*/

        JarFile _jf = null;
        try {
            String _path1 = context.getSdk().getClass().getResource("").getPath();
//            log.debug("_path1 =" + _path1);
            String _jarPath = new File(_path1).getParent().replaceAll(
                    "(!|file:)", "").replaceAll(PathResolver.separator + "xpu"+PathResolver.separator + "sw" + PathResolver.separator + "tools", "");
//            log.debug("_jarPath =" + _jarPath);
            _jf = new JarFile(_jarPath);

            Enumeration<JarEntry> _entries = _jf.entries();
            while (_entries.hasMoreElements()) {
                JarEntry _je = _entries.nextElement();
//                log.debug("_je1=" + _je.getName());
                if (_je.getName().startsWith(_path)) {
//                    log.debug("_je2>>>>>>>=" + _je.getName());
                    String _theme = FilenameUtils.removeExtension(_je.getName());
                    String[] _tmp = _theme.split("/");
                    if(_tmp.length > 1){
                        _theme = _tmp[_tmp.length - 1];
                        _theme = StringUtils.captializeFirstLetter(_theme);
                        _themes.add(_theme);
                    }
                }
            }
        } catch (IOException ex) {
            ex.printStackTrace();
        } finally {
            try {
                _jf.close();
            } catch (Exception e) {
            }
        }

        return _themes;
    }

//-------------------------------------------------------------------------------------
    public void viewWindowInFullScreen() {
        /*        setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        getRootPane().getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(
            KeyStroke.getKeyStroke(KeyEvent.VK_ESCAPE, 0), "Cancel"); //$NON-NLS-1$
        getRootPane().getActionMap().put("Cancel", new AbstractAction() {*/
//            log.debug("Create frame...");

//            GraphicsDevice gd = GraphicsEnvironment.getLocalGraphicsEnvironment().getDefaultScreenDevice();
/*            JFrame _jFrame = new JFrame("Magnifier");
            Component _magnifier = gui.getMyComponents().getDebugger().getMagnifier();
//            _jFrame.setDefaultCloseOperation(JFrame.HIDE_ON_CLOSE);
            _jFrame.setDefaultCloseOperation(JFrame.HIDE_ON_CLOSE);
            _jFrame.getContentPane().add(_magnifier, BorderLayout.CENTER);
            _jFrame.pack();
            _jFrame.setVisible(true);*/
//            gd.setFullScreenWindow(_jFrame);
/*
            public void actionPerformed(ActionEvent e) {
                log.debug("Escape...");
                save();
                //framename.setVisible(false);
//                System.exit(0);
            }
        });*/
    }

//-------------------------------------------------------------------------------------
    public ImageIcon getIconFromResources(String _path) {
        try {
            InputStream _inputStream = null;
            if (_inputStream == null) {
                _inputStream = getClass().getResourceAsStream("/" + _path);
            }
            if (_inputStream == null) {
                _inputStream = getClass().getResourceAsStream("src/main/resources/" + _path);
            }

            if (_inputStream == null) {
                if (new File("src/main/resources/" + _path).exists()) {
                    _inputStream = new FileInputStream("src/main/resources/" + _path);
                }
            }

            if (_inputStream == null) {
                if (new File(_path).exists()) {
                    _inputStream = new FileInputStream(_path);
                }
            }

            return new ImageIcon(ImageIO.read(_inputStream));
        } catch (IOException _e) {
            log.error("Cannot load: " + _path);
            return null;
        }
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
