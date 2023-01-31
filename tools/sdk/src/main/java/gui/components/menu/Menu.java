//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.components.menu;

//-------------------------------------------------------------------------------------
import java.io.*;
import java.net.*;
import java.util.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.filechooser.*;

import org.apache.commons.configuration2.*;
import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.project.*;

import xpu.sw.tools.sdk.gui.*;
import xpu.sw.tools.sdk.gui.components.menu.file.preferences.*;
import xpu.sw.tools.sdk.gui.components.menu.project.newproject.*;
import xpu.sw.tools.sdk.gui.components.menu.project.settingsproject.*;
import xpu.sw.tools.sdk.gui.components.menu.help.checkforupdates.*;

//-------------------------------------------------------------------------------------

public class Menu {
    private Gui gui;
    private Context context;
    private Logger log;

    private JMenuBar menuBar;

    private JMenu profile;
    private JMenuItem appLevel;
    private JMenuItem highLibraryLevel;
    private JMenuItem lowLibraryLevel;

    private JMenu file;
    private JMenuItem newFile;
    private JMenuItem openFile;
    private JMenuItem saveFile;
    private JMenuItem closeFile;
    private JMenuItem prefences;
    private JMenuItem quit;

    private JMenu project;
    private JMenuItem newProject;
    private JMenuItem openProject;
    private JMenuItem saveProject;
    private JMenuItem closeProject;
    private JMenuItem addFileToProject;
    private JMenuItem removeFileFromProject;
    private JMenuItem settingsProject;

    private JMenu tools;
    private JMenuItem asm;
    private JMenuItem run;
    private JCheckBoxMenuItem debug;

    private JMenu debugMenu;
    private JMenuItem debugStep;
    private JMenuItem debugRun;
    private JMenuItem debugContinue;
    private JMenuItem debugExit;

    private JMenu view;
    private JMenuItem viewWindowInFullScreen;


    private JMenu help;
    private JMenuItem userManual;
    private JMenuItem reportABug;
    private JMenuItem checkForUpdates;

    private Configuration sdkConfig;
    private MenuHandlers menuHandlers;

//-------------------------------------------------------------------------------------
    public Menu(Gui _gui, Context _context) {
        gui = _gui;
        context = _context;
        log = _context.getLog();
        sdkConfig = _context.getSdkConfig();
        addMenu();
        debug.setSelected(context.getDebugStatus() == Context.DEBUG_STATUS_ON);
        menuHandlers = new MenuHandlers(_gui, _context);
        addMenuHandlers();
    }

//-------------------------------------------------------------------------------------
    private void addMenu() {
        menuBar = new JMenuBar();

        profile = new JMenu("Profile");

        appLevel = new JCheckBoxMenuItem("App Level");
        profile.add(appLevel);

        highLibraryLevel = new JCheckBoxMenuItem("High Library Level");
        profile.add(highLibraryLevel);

        lowLibraryLevel = new JCheckBoxMenuItem("Low Library Level");
        profile.add(lowLibraryLevel);

        menuBar.add(profile);

        file = new JMenu("File");

        newFile = new JMenuItem("New...");
        file.add(newFile);

        openFile = new JMenuItem("Open...");
        file.add(openFile);

        saveFile = new JMenuItem("Save");
        file.add(saveFile);

        closeFile = new JMenuItem("Close");
        file.add(closeFile);

        file.add(new JSeparator()); // SEPARATOR

        prefences = new JMenuItem("Preferences");
        file.add(prefences);

        if(true){
            file.add(new JSeparator()); // SEPARATOR

            quit = new JMenuItem("Quit");
            file.add(quit);            
        }

        menuBar.add(file);


        project = new JMenu("Project");

        newProject = new JMenuItem("New...");
        project.add(newProject);

        openProject = new JMenuItem("Open...");
        project.add(openProject);

        saveProject = new JMenuItem("Save");
        project.add(saveProject);

        closeProject = new JMenuItem("Close");
        project.add(closeProject);

        project.add(new JSeparator()); // SEPARATOR
        addFileToProject = new JMenuItem("Add file...");
        project.add(addFileToProject);

        removeFileFromProject = new JMenuItem("Remove file...");
        project.add(removeFileFromProject);

        project.add(new JSeparator()); // SEPARATOR

        settingsProject = new JMenuItem("Settings");
        project.add(settingsProject);

        menuBar.add(project);


        tools = new JMenu("Tools");

//        compile = new JMenuItem("Compile");
//        tools.add(compile);

        asm = new JMenuItem("Asm");
        tools.add(asm);

        run = new JMenuItem("Run");
        tools.add(run);


        debug = new JCheckBoxMenuItem("Debug", false);
        tools.add(debug);


//        upload = new JMenuItem("Upload");
//        tools.add(upload);

        menuBar.add(tools);


        debugMenu = new JMenu("Debug");
        debugStep = new JMenuItem("Step");
        debugMenu.add(debugStep);
        debugRun = new JMenuItem("Run");
        debugMenu.add(debugRun);
        debugContinue = new JMenuItem("Continue");
        debugMenu.add(debugContinue);
        debugExit = new JMenuItem("Exit");
        debugMenu.add(debugExit);
        menuBar.add(debugMenu);

        view = new JMenu("View");
        viewWindowInFullScreen = new JMenuItem("View window in FullScreen");
        view.add(viewWindowInFullScreen);
        menuBar.add(view);



        help = new JMenu("Help");

        userManual = new JMenuItem("User manual");
        help.add(userManual);

        reportABug = new JMenuItem("Report a bug");
        help.add(reportABug);

        checkForUpdates = new JMenuItem("Check for updates...");
        help.add(checkForUpdates);

        menuBar.add(help);


        gui.setJMenuBar(menuBar);
        refresh();
    }

//-------------------------------------------------------------------------------------
    private void addMenuHandlers() {
        setupProfileAppLevel();
        setupProfileHighLibraryLevel();
        setupProfileLowLibraryLevel();

        setupNewFile();
        setupOpenFile();
        setupSaveFile();
        setupCloseFile();
        setupPreferences();
        setupQuit();

        setupNewProject();
        setupOpenProject();
        setupSaveProject();
        setupCloseProject();
        setupAddFileToProject();
        setupRemoveFileFromProject();
        setupSettingsProject();

        setupAsm();
        setupRun();
        setupDebug();

        setupDebugStep();
        setupDebugRun();
        setupDebugContinue();
        setupDebugExit();

        setupViewWindowInFullScreen();

        setupCheckForUpdates();
    }

//-------------------------------------------------------------------------------------
    public MenuHandlers getMenuHandlers() {
        return menuHandlers;
    }

//-------------------------------------------------------------------------------------
    private void setupProfileAppLevel() {
        appLevel.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_N, ActionEvent.META_MASK));
        appLevel.getAccessibleContext().setAccessibleDescription("App Level");
        appLevel.setActionCommand("App Level");
        appLevel.addActionListener(new ActionListener() {
              @Override
              public void actionPerformed(ActionEvent e) {
                menuHandlers.switchToProfile(Context.PROFILE_APP_LEVEL);
            }
        });
    }

//-------------------------------------------------------------------------------------
    private void setupProfileHighLibraryLevel() {
        highLibraryLevel.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_N, ActionEvent.META_MASK));
        highLibraryLevel.getAccessibleContext().setAccessibleDescription("High Library Level");
        highLibraryLevel.setActionCommand("High Library Level");
        highLibraryLevel.addActionListener(new ActionListener() {
              @Override
              public void actionPerformed(ActionEvent e) {
                menuHandlers.switchToProfile(Context.PROFILE_HIGH_LEVEL);
            }
        });
    }

//-------------------------------------------------------------------------------------
    private void setupProfileLowLibraryLevel() {
        lowLibraryLevel.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_N, ActionEvent.META_MASK));
        lowLibraryLevel.getAccessibleContext().setAccessibleDescription("Low Library Level");
        lowLibraryLevel.setActionCommand("Low Library Level");
        lowLibraryLevel.addActionListener(new ActionListener() {
              @Override
              public void actionPerformed(ActionEvent e) {
                menuHandlers.switchToProfile(Context.PROFILE_LOW_LEVEL);
            }
        });
    }

//-------------------------------------------------------------------------------------
    private void setupNewFile() {
        newFile.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_N, ActionEvent.META_MASK));
        newFile.getAccessibleContext().setAccessibleDescription("New file");
        newFile.setActionCommand("New file");
        newFile.addActionListener(new ActionListener() {
              @Override
              public void actionPerformed(ActionEvent e) {
                menuHandlers.newFile();
            }
        });
    }

//-------------------------------------------------------------------------------------
    private void setupOpenFile() {
        openFile.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_O, ActionEvent.META_MASK));
        openFile.getAccessibleContext().setAccessibleDescription("Open file");
        openFile.setActionCommand("Open file");
        openFile.addActionListener(new ActionListener() {
              @Override
              public void actionPerformed(ActionEvent e) {
                menuHandlers.openFile();
            }
        });
    }

//-------------------------------------------------------------------------------------
    private void setupSaveFile() {
//        _actions.setMnemonic(KeyEvent.VK_A);
        saveFile.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_S, ActionEvent.META_MASK));
        saveFile.getAccessibleContext().setAccessibleDescription("Save file");
        saveFile.setActionCommand("Save file");
        saveFile.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                menuHandlers.saveFile();
            }
        });
    }

//-------------------------------------------------------------------------------------
    private void setupCloseFile() {
//        _actions.setMnemonic(KeyEvent.VK_A);
        closeFile.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_C, ActionEvent.META_MASK));
        closeFile.getAccessibleContext().setAccessibleDescription("Close file");
        closeFile.setActionCommand("Close file");
        closeFile.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                menuHandlers.closeFile();
            }
        });
    }

//-------------------------------------------------------------------------------------
    private void setupPreferences() {
        prefences.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_P, ActionEvent.META_MASK));
        prefences.getAccessibleContext().setAccessibleDescription("Preferences");
        prefences.setActionCommand("Preferences");
        prefences.addActionListener(new ActionListener() {
              @Override
              public void actionPerformed(ActionEvent e) {
                    menuHandlers.preferences(null);                    
              }
        });
    }

//-------------------------------------------------------------------------------------
    private void setupQuit() {
//        prefences.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_Q, ActionEvent.META_MASK));
//        prefences.getAccessibleContext().setAccessibleDescription("Quit");
  //      prefences.setActionCommand("Quit");
//        System.out.println("Quit....0");
        if(quit != null){
            quit.addActionListener(new ActionListener() {
                  @Override
                  public void actionPerformed(ActionEvent e) {
    //        System.out.println("Quit....1");
                        menuHandlers.quit();
                  }
            });
        }
    }

//-------------------------------------------------------------------------------------
    private void setupNewProject() {
        newProject.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_N, ActionEvent.CTRL_MASK));
        newProject.getAccessibleContext().setAccessibleDescription("New project");
        newProject.setActionCommand("New project");
        newProject.addActionListener(new ActionListener() {
              @Override
              public void actionPerformed(ActionEvent e) {
                    menuHandlers.newProject();
            }
        });
    }

//-------------------------------------------------------------------------------------
    private void setupOpenProject() {
        openProject.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_O, ActionEvent.CTRL_MASK));
        openProject.getAccessibleContext().setAccessibleDescription("Open project");
        openProject.setActionCommand("Open project");
        openProject.addActionListener(new ActionListener() {
              @Override
              public void actionPerformed(ActionEvent e) {
                menuHandlers.openProject();
            }
        });
    }

//-------------------------------------------------------------------------------------
    private void setupSaveProject() {
        saveProject.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_S, ActionEvent.CTRL_MASK));
        saveProject.getAccessibleContext().setAccessibleDescription("Save project");
        saveProject.setActionCommand("Save project");
        saveProject.addActionListener(new ActionListener() {
              @Override
              public void actionPerformed(ActionEvent e) {
                menuHandlers.saveProject();
            }
        });
    }

//-------------------------------------------------------------------------------------
    private void setupCloseProject() {
        closeProject.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_C, ActionEvent.CTRL_MASK));
        closeProject.getAccessibleContext().setAccessibleDescription("Close project");
        closeProject.setActionCommand("Close project");
        closeProject.addActionListener(new ActionListener() {
              @Override
              public void actionPerformed(ActionEvent e) {
                menuHandlers.closeProject();
            }
        });
    }

//-------------------------------------------------------------------------------------
    private void setupAddFileToProject() {
        addFileToProject.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_A, ActionEvent.CTRL_MASK));
        addFileToProject.getAccessibleContext().setAccessibleDescription("Add file to project");
        addFileToProject.setActionCommand("Add file to project");
        addFileToProject.addActionListener(new ActionListener() {
              @Override
              public void actionPerformed(ActionEvent e) {
                menuHandlers.addFileToProject();
            }
        });
    }

//-------------------------------------------------------------------------------------
    private void setupRemoveFileFromProject() {
        removeFileFromProject.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_R, ActionEvent.CTRL_MASK));
        removeFileFromProject.getAccessibleContext().setAccessibleDescription("Remove file from project");
        removeFileFromProject.setActionCommand("Remove file from project");
        removeFileFromProject.addActionListener(new ActionListener() {
              @Override
              public void actionPerformed(ActionEvent e) {
                menuHandlers.removeFileFromProject();
            }
        });
    }

//-------------------------------------------------------------------------------------
    private void setupSettingsProject() {
        settingsProject.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_T, ActionEvent.CTRL_MASK));
        settingsProject.getAccessibleContext().setAccessibleDescription("Settings");
        settingsProject.setActionCommand("Settings project");
        settingsProject.addActionListener(new ActionListener() {
              @Override
              public void actionPerformed(ActionEvent e) {
                  menuHandlers.settingsProject();
            }
        });
    }
/*
//-------------------------------------------------------------------------------------
    private void setupCompile() {
        compile.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_C, ActionEvent.CTRL_MASK+ActionEvent.ALT_MASK));
        compile.getAccessibleContext().setAccessibleDescription("Compile");
        compile.setActionCommand("compile");
        compile.addActionListener(new ActionListener() {
              @Override
              public void actionPerformed(ActionEvent e) {
                menuHandlers.compile();
              }
        });
    }
*/

//-------------------------------------------------------------------------------------
    private void setupAsm() {
        asm.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_A, ActionEvent.CTRL_MASK+ActionEvent.ALT_MASK));
        asm.getAccessibleContext().setAccessibleDescription("Asm");
        asm.setActionCommand("asm");
        asm.addActionListener(new ActionListener() {
              @Override
              public void actionPerformed(ActionEvent e) {
                menuHandlers.asm();
              }
        });
    }

//-------------------------------------------------------------------------------------
    private void setupRun() {
        run.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_R, ActionEvent.CTRL_MASK+ActionEvent.ALT_MASK));
        run.getAccessibleContext().setAccessibleDescription("Run");
        run.setActionCommand("run");
        run.addActionListener(new ActionListener() {
              @Override
              public void actionPerformed(ActionEvent e) {
                menuHandlers.remoteRun();
              }
        });
    }


//-------------------------------------------------------------------------------------
    private void setupDebug() {
        debug.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_D, ActionEvent.CTRL_MASK+ActionEvent.ALT_MASK));
        debug.getAccessibleContext().setAccessibleDescription("Debug");
        debug.setActionCommand("debug");
        debug.addActionListener(new ActionListener() {
              @Override
              public void actionPerformed(ActionEvent e) {
                if(debug.isSelected()){
                    menuHandlers.debugEnter();
                } else {
                    menuHandlers.debugExit();                    
                }
                
              }
        });
    }

//-------------------------------------------------------------------------------------
        private void setupDebugStep() {
        debugStep.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_F6, ActionEvent.ALT_MASK));
        debugStep.getAccessibleContext().setAccessibleDescription("Debug step");
        debugStep.setActionCommand("debugStep");
        debugStep.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                menuHandlers.debugStep();
            }
        });
    }

//-------------------------------------------------------------------------------------
        private void setupDebugRun() {
        debugRun.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_F7, ActionEvent.ALT_MASK));
        debugRun.getAccessibleContext().setAccessibleDescription("Debug run");
        debugRun.setActionCommand("debugRun");
        debugRun.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                menuHandlers.debugRun();
            }
        });
    }

//-------------------------------------------------------------------------------------
    private void setupDebugContinue() {
        debugContinue.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_F8, ActionEvent.ALT_MASK));
        debugContinue.getAccessibleContext().setAccessibleDescription("debug continue");
        debugContinue.setActionCommand("debugContinue");
        debugContinue.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                menuHandlers.debugContinue();
            }
        });
    }

//-------------------------------------------------------------------------------------
    private void setupDebugExit() {
        debugExit.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_F10, ActionEvent.ALT_MASK));
        debugExit.getAccessibleContext().setAccessibleDescription("Debug exit");
        debugExit.setActionCommand("debugExit");
        debugExit.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                menuHandlers.debugExit();
            }
        });
    }

//-------------------------------------------------------------------------------------
    private void setupViewWindowInFullScreen() {
        viewWindowInFullScreen.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_F, ActionEvent.ALT_MASK));
        viewWindowInFullScreen.getAccessibleContext().setAccessibleDescription("View window in FullScreen");
        viewWindowInFullScreen.setActionCommand("viewWindowInFullScreen");
        viewWindowInFullScreen.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
//                log.debug("Menu.viewWindowInFullScreen");
                menuHandlers.viewWindowInFullScreen();
            }
        });
    }

//-------------------------------------------------------------------------------------
    private void setupCheckForUpdates() {
//        checkForUpdates.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_U, ActionEvent.ALT_MASK));
        checkForUpdates.getAccessibleContext().setAccessibleDescription("Check for new updates...");
        checkForUpdates.setActionCommand("checkForUpdates");
        checkForUpdates.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                menuHandlers.checkForUpdates();
            }
        });
    }

//-------------------------------------------------------------------------------------
    public void setDebugStatus(boolean _selected) {
        debug.setSelected(_selected);
        debugMenu.setVisible(_selected);
    }

//-------------------------------------------------------------------------------------
    public void refresh() {
        String _profile = context.getProfile();
        appLevel.setSelected(_profile.equals(Context.PROFILE_APP_LEVEL));
        highLibraryLevel.setSelected(_profile.equals(Context.PROFILE_HIGH_LEVEL));
        lowLibraryLevel.setSelected(_profile.equals(Context.PROFILE_LOW_LEVEL));

        asm.setVisible(_profile.equals(Context.PROFILE_LOW_LEVEL));
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
