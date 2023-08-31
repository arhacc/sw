//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.components.menu;

//-------------------------------------------------------------------------------------
import java.io.*;
import java.net.*;
import java.util.*;
import java.nio.file.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.filechooser.*;

import org.apache.commons.cli.*;
import org.apache.commons.configuration2.*;
import org.apache.logging.log4j.*;

import org.antlr.v4.runtime.*;

import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.project.*;
import xpu.sw.tools.sdk.common.io.*;
import xpu.sw.tools.sdk.common.io.targetmanager.*;
//import xpu.sw.tools.sdk.common.io.updater.*;

import xpu.sw.tools.sdk.gui.*;
import xpu.sw.tools.sdk.gui.components.menu.file.newfile.*;
import xpu.sw.tools.sdk.gui.components.menu.file.preferences.*;
import xpu.sw.tools.sdk.gui.components.menu.project.newproject.*;
import xpu.sw.tools.sdk.gui.components.menu.project.settingsproject.*;
//import xpu.sw.tools.sdk.gui.components.menu.help.checkforupdates.*;
import xpu.sw.tools.sdk.gui.components.debugger.*;
import xpu.sw.tools.sdk.gui.components.editor.*;

import xpu.sw.tools.sdk.*;
import xpu.sw.tools.sdk.asm.*;
import xpu.sw.tools.sdk.rexec.*;

//-------------------------------------------------------------------------------------

public class MenuHandlers {
    private Gui gui;
    private Context context;
    private Logger log;

    private Rexec rexec;
    private Configuration sdkConfig;

//-------------------------------------------------------------------------------------
    public MenuHandlers(Gui _gui, Context _context) {
        gui = _gui;
        context = _context;
        log = _context.getLog();
        sdkConfig = _context.getSdkConfig();
        rexec = _gui.getServices().getRexec();
    }

//-------------------------------------------------------------------------------------
    public String getPathOfCurrentDirectory() {
        return gui.getActiveProject().getRootPath();
    }

//-------------------------------------------------------------------------------------
    public void switchToProfile(String _newProfile) {
//        log.debug("switchToProfile=" + _newProfile);
//        new Throwable().printStackTrace();
        String _profile = context.getProfile();
        if(!_profile.equals(_newProfile)){
            context.getSdkConfig().setProperty("profile", _newProfile);
            gui.getMyComponents().getHierarchy().setProfileLevel(_newProfile);
            gui.refresh();
        }
    }

//-------------------------------------------------------------------------------------
    public void newFile() {
//        String _lastDirectory = sdkConfig.getString("last_directory", FileSystemView.getFileSystemView().getHomeDirectory().getAbsolutePath());
//        _lastDirectory = PathResolver.importPath(_lastDirectory);

//        AsmFile(log, getPathOfCurrentDirectory(), noname
        new NewFile(gui, context);

        String _path = PathResolver.add(getPathOfCurrentDirectory(), "noname00");
        gui.getMyComponents().getEditor().addTab(_path);
    }

//-------------------------------------------------------------------------------------
    public void openFile() {
//        String _lastDirectory = sdkConfig.getString("last_directory", FileSystemView.getFileSystemView().getHomeDirectory().getAbsolutePath());
//        _lastDirectory = PathResolver.importPath(_lastDirectory);
/*                if((_lastDirectory == null) || (_lastDirectory.isEmpty())){
            _lastDirectory = FileSystemView.getFileSystemView().getHomeDirectory().getAbsolutePath();
            _config.addProperty("last_directory", _lastDirectory);
        }*/
        String _path = getPathOfCurrentDirectory();
        JFileChooser jfc = new JFileChooser(_path);

        int returnValue = jfc.showOpenDialog(null);
        // int returnValue = jfc.showSaveDialog(null);

        if (returnValue == JFileChooser.APPROVE_OPTION) {
            File selectedFile = jfc.getSelectedFile();
            sdkConfig.setProperty("last_directory", PathResolver.exportPath(selectedFile.getAbsolutePath()));
            gui.getMyComponents().getEditor().addTab(selectedFile);
        }
    }

//-------------------------------------------------------------------------------------
    public void closeFile() {
        log.debug("Not implemented yet!");
    }

//-------------------------------------------------------------------------------------
    public void saveFile() {
//        log.debug("Saving....");
        gui.getMyComponents().getEditor().saveCurentTab();
    }

//-------------------------------------------------------------------------------------
    public void preferences(String _section) {
        new Preferences(gui, context, _section);
    }

//-------------------------------------------------------------------------------------
    public void quit() {
        gui.dispose();
//        System.out.println("Quit....");
        System.exit(0);
    }

//-------------------------------------------------------------------------------------
    public void newProject() {
        NewProject _newProject = new NewProject(gui, context);
        Project _project = _newProject.getCreatedProject();
        if(_project != null){
//            openProject(_project);
            gui.getMyComponents().getHierarchy().addProject(_project);        
        }
    }

//-------------------------------------------------------------------------------------
    public void openProject() {
//        String _defaultPath = context.getPathToSdkHome() + "/projects/";
        String _defaultPath = PathResolver.XPU_LIBRARIES_PATH + "/app_level/";
//        String _lastDirectory = sdkConfig.getString("last.project.location", _defaultPath);
//        _lastDirectory = PathResolver.importPath(_lastDirectory);
        log.debug("setupOpenProject: _lastDirectory=" + _defaultPath);
        JFileChooser _jfc = new JFileChooser(_defaultPath);
        FileNameExtensionFilter _filter = new FileNameExtensionFilter("*.xpuprj", "xpuprj");
        _jfc.setFileFilter(_filter);

        int returnValue = _jfc.showOpenDialog(null);
        if (returnValue == JFileChooser.APPROVE_OPTION) {
            File _selectedFile = _jfc.getSelectedFile();
            String _path = _selectedFile.getAbsolutePath();
            sdkConfig.setProperty("last.project.directory", PathResolver.exportPath(_path));
            Project _project = new Project(context, _path);
            gui.getMyComponents().getHierarchy().addProject(_project);        
//            openProject(_project);
        }
    }
/*
//-------------------------------------------------------------------------------------
    public void openProject(Project _project) {
        sdkConfig.addProperty("open_projects", _project.getPathToConfigFile());
    }
*/
//-------------------------------------------------------------------------------------
    public void saveProject() {
        log.debug("Not implemented yet!");
    }

//-------------------------------------------------------------------------------------
    public void closeProject() {
//        log.debug("Not implemented yet!");
        Project _project = gui.getMyComponents()
            .getHierarchy()
            .getSelectedProject();

        gui.getMyComponents()
            .getHierarchy()
            .removeProject(_project);
    }

//-------------------------------------------------------------------------------------
    public void addFileToProject() {
//        log.debug("Not implemented yet!");
//        String _lastDirectory = sdkConfig.getString("last_directory", FileSystemView.getFileSystemView().getHomeDirectory().getAbsolutePath());
        String _path = getPathOfCurrentDirectory();
        JFileChooser jfc = new JFileChooser(_path);
        FileNameExtensionFilter filter = new FileNameExtensionFilter(
            "Asm, C, C++, ONNX files", "asm", "c", "cpp", "onnx");
        jfc.setFileFilter(filter);
        int returnValue = jfc.showOpenDialog(null);

        if (returnValue == JFileChooser.APPROVE_OPTION) {
            SwingUtilities.invokeLater( new Runnable() {
                public void run() {
                    try {
                        File selectedFile = jfc.getSelectedFile();
                        

                        gui.getMyComponents()
                            .getHierarchy()
                            .getSelectedProject()
                            .addFile(selectedFile);

                        gui.getMyComponents()
                            .getHierarchy()
                            .refresh();

                        gui.getMyComponents()
                            .getEditor()
                            .addTab(selectedFile);

                    } catch (Exception useDefault) {}
                }
            });
        }        
    }

//-------------------------------------------------------------------------------------
    public void removeFileFromProject() {
        log.debug("Not implemented yet!");
    }

//-------------------------------------------------------------------------------------
    public void settingsProject() {
        new SettingsProject(gui, context);
    }
/*
//-------------------------------------------------------------------------------------
    public void compile() {
        log.debug("Not implemented yet!");
    }
*/

//-------------------------------------------------------------------------------------
    public void asm() {
        String _path = gui.getMyComponents().getHierarchy().getSelectedProjectPath();
        if(_path == null){
            _path = gui.getMyComponents().getEditor().getSelectedFilename();
        }
        String[] _args = new String[]{"-cmd", "asm", _path};
        Sdk _sdk = context.getSdk();
        CommandLine _commandLine = _sdk.getCommandLine(_args);
        ANTLRErrorListener _errorListener = gui.getMyComponents().getTerminal().getConsoleAppender();
        Context _context = new Context(_sdk, log, _commandLine); 
        new Asm(_context, _errorListener);
    }

//-------------------------------------------------------------------------------------
    public void remoteRun() {
//        log.debug("Not implemented yet!");
//        String _path = gui.getMyComponents().getEditor().getSelectedFilename();
//        if((_path == null) || (!rexec.isRunnable(_path))){
        Project _project = gui.getMyComponents().getHierarchy().getSelectedProject();
        File _file = gui.getMyComponents().getHierarchy().getSelectedFile();
        if(_project == null){
            if(_file == null){
                log.debug("Please select a project or file!");
                return;
            }
        }
        int _responseCode = rexec.remoteRun(_project, _file);
        if(_responseCode == Command.COMMAND_DONE){
            gui.getMyComponents().getDebugger().refresh();
        }
    }

/*
//-------------------------------------------------------------------------------------
    public void upload() {
        String _path = gui.getMyComponents().getHierarchy().getSelectedProjectPath();
        if(_path == null){
            _path = gui.getMyComponents().getEditor().getSelectedFilename();
            if(_path != null){
                _path = Paths.get(_path).getParent().toString();
            }
        }
        String[] _args = new String[]{"-cmd", "upload", _path};
        Sdk _sdk = context.getSdk();
        CommandLine _commandLine = _sdk.getCommandLine(_args);
//        ANTLRErrorListener _errorListener = gui.getMyComponents().getTerminal().getConsoleAppender();
        Context _context = new Context(_sdk, log, _commandLine); 
        new Rexec(_context);
    }
*/
//-------------------------------------------------------------------------------------
    public void debugEnter() {
        gui.getMyComponents().getMenu().setDebugStatus(true);
        gui.getMyComponents().getToolbar().setDebugStatus(true);
        gui.getMyComponents().getDebugger().debugEnter();
    }

//-------------------------------------------------------------------------------------
    public void debugExit() {
        gui.getMyComponents().getMenu().setDebugStatus(false);
        gui.getMyComponents().getToolbar().setDebugStatus(false);
        gui.getMyComponents().getDebugger().debugExit();
    }

//-------------------------------------------------------------------------------------
    public void debugStep() {
    }

//-------------------------------------------------------------------------------------
    public void debugRun() {

    }

//-------------------------------------------------------------------------------------
    public void debugContinue() {

    }

//-------------------------------------------------------------------------------------
    public void viewWindowInFullScreen() {
//        log.debug("MenuHandlers.viewWindowInFullScreen");
        gui.getServices().getUtils().viewWindowInFullScreen();
    }

//-------------------------------------------------------------------------------------
    public void enterFullScreen() {
        log.debug("--->");
        log.debug("Set Visible...");
//            setPreferredSize(screenSize);
        gui.pack();
        gui.setResizable(false);
//            setExtendedState(JFrame.MAXIMIZED_BOTH);//blocks setFullScreenWindow!!!
        gui.setVisible(true);
        GraphicsDevice gd = GraphicsEnvironment.getLocalGraphicsEnvironment().getDefaultScreenDevice();
        gd.setFullScreenWindow(gui);
    }

//-------------------------------------------------------------------------------------
    public void checkForUpdates() {
        log.debug("checkForUpdates...");
//        new CheckForUpdates(gui, context);
        gui.getServices().getUpdater().triggerCheckForUpdates();
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
