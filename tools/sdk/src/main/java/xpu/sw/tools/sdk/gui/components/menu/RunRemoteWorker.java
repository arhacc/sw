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
import xpu.sw.tools.sdk.common.debug.*;
import xpu.sw.tools.sdk.common.project.*;
import xpu.sw.tools.sdk.common.io.*;
import xpu.sw.tools.sdk.common.io.targetmanager.*;
//import xpu.sw.tools.sdk.common.io.updater.*;
import xpu.sw.tools.sdk.common.fileformats.core.*;

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
public class RunRemoteWorker extends SwingWorker<Boolean, Integer>{
    private Gui gui;
    private Context context;
    private Project project;


    private Logger log;
    private Rexec rexec;
    private Configuration sdkConfig;

//-------------------------------------------------------------------------------------
    public RunRemoteWorker(Gui _gui, Context _context, Project _project) {
        gui = _gui;
        context = _context;
        project = _project;

        log = _context.getLog();
        sdkConfig = _context.getSdkConfig();
        rexec = _gui.getServices().getRexec();
    }

//-------------------------------------------------------------------------------------
    @Override
    protected Boolean doInBackground() throws Exception {
        // Start Progress
        setProgress(0);
//        waitFor(500);
//                gui.changeCursor(Cursor.WAIT_CURSOR);
                log.debug("RemoteRun...");
                File _sourceFile = project.getDefaultSourceFile();
                File _runningFile = project.getDefaultRunningFile();
                EditorTab _editorTab = gui.getMyComponents().getEditor().getActiveEditor().getActiveEditor().getEditorTabByPath(_sourceFile.getPath());
                if(_editorTab == null){
                    log.warn("Cannot find EditorTab:" + _sourceFile.getPath());
                    gui.changeCursor(Cursor.DEFAULT_CURSOR);
                    return false;
                }
                EditorTabDebugInformation _editorTabDebugInformation = _editorTab.getEditorTabDebugInformation();
                DebugInformation _debugInformation = _editorTabDebugInformation.getDebugInformation();
                log.debug("MenuHandlers: _sourceFile=" + _sourceFile.getPath() +", _runningFile=" + _runningFile.getPath() +", DebugInformation=" + _debugInformation);
                RemoteRunResponse _remoteRunResponse = rexec.remoteRun(project, _runningFile, _debugInformation);
                if(_remoteRunResponse.getCommandCode() == Command.COMMAND_DONE){
                    String[] _outputs = _remoteRunResponse.getOutputs();
                    log.debug("Run: Outputs.number=" + _outputs.length);                    
                    for(int i = 0; i < _outputs.length; i++){
                        log.debug("Run: Output["+i+"]:" + _outputs[i]);                    
                    }
                    _debugInformation.refresh(_remoteRunResponse);
                    gui.getMyComponents().getDebugger().refresh();
                    gui.getMyComponents().getEditor().refresh();
                }
//                gui.changeCursor(Cursor.DEFAULT_CURSOR);
/*
        // Example Loop
        for (int iCount = 1; iCount <= 20; iCount++) {
            // Is an even number?
            if (iCount % 2 == 0) {
                publish(iCount);
            }

            // Set Progress
            setProgress((iCount * 100) / 20);
//            waitFor(250);
        }
*/
        // Finished
        return true;
    }

//-------------------------------------------------------------------------------------
    @Override
    protected void process(java.util.List<Integer> chunks) {

    }

//-------------------------------------------------------------------------------------
    @Override
    protected void done() {
        boolean bStatus = false;
        try {
            bStatus = get();
        } catch (Exception ex) {
            ex.printStackTrace();
        }
//        System.out.println("Finished with status " + bStatus);
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
