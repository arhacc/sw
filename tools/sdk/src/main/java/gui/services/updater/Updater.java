//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.services.updater;
//-------------------------------------------------------------------------------------
import java.awt.*;
import java.awt.event.*;
import java.net.*;
import java.io.*;
import java.nio.file.*;
import java.nio.channels.*;
import java.util.*;
import javax.swing.*;
import java.lang.reflect.*;

import org.apache.commons.configuration2.*;
import org.apache.logging.log4j.*;

/*
import org.eclipse.jgit.api.*;
import org.eclipse.jgit.lib.*;
import org.eclipse.jgit.revwalk.*;
import org.eclipse.jgit.treewalk.*;
import org.eclipse.jgit.treewalk.filter.*;
*/

import codex.common.utils.*;
import codex.common.apps.rxbasics.*;
import codex.common.wrappers.version.*;

import xpu.sw.tools.sdk.*;
import xpu.sw.tools.sdk.common.context.*;

//-------------------------------------------------------------------------------------
public class Updater extends RxStatus {
    private Context context;
//    private Logger log;
    private org.apache.commons.configuration2.Configuration sdkConfig;

    private int status;
    private String xpuHome;
    private Object __objUpdateLocker;
    private UpdaterListener updaterListener;
    private String lastVersionRemote;
    private String lastVersionInstalled;

    private static final int STATUS_SLEEP               = 0;
    private static final int STATUS_CHECK               = 1;
    private static final int STATUS_DOWNLOAD            = 2;
    private static final int STATUS_INSTALL             = 3;
    private static final int STATUS_EXIT                = 4;

//https://github.com/arhacc/sw/releases/download/v0.1/xpu-sdk-0.1.22.jar
    private static final String DEFAULT_URL_UPDATE = "https://www.github.com/arhacc/sw";
    private static final String GIT_PATH = "releases/download/";

//-------------------------------------------------------------------------------------
    public Updater(Context _context) {
        super(_context.getLog());
        context = _context;
        sdkConfig = context.getSdkConfig();
        status = STATUS_SLEEP;
        xpuHome = SystemUtils.getHomeDirectory() + "/.xpu";
        __objUpdateLocker = new Object();
        lastVersionRemote = "";
        lastVersionInstalled = context.getVersion();
        setRunning();
        start();
    }

//-------------------------------------------------------------------------------------
    public void run(){
        while(isRunning()){
            switch(status){
                case STATUS_SLEEP: {
                    try{
                        Thread.sleep(360000);                        
                    } catch(InterruptedException _e){

                    }
                    status = STATUS_CHECK;
                    break;
                }
                case STATUS_CHECK: {
                    if(sdkConfig.getBoolean("gui.menu.file.preferences.general.automaticallyCheckForUpdates.enabled", false)){
                        if(!check()){
                            status = STATUS_SLEEP;
                        } else {
                            if(sdkConfig.getBoolean("gui.menu.file.preferences.general.automaticallyInstallUpdates.enabled", false)){
                                status = STATUS_DOWNLOAD;
                            }                        
                        }
                    } else {
                        status = STATUS_SLEEP;                        
                    }
                    break;
                }
                case STATUS_DOWNLOAD: {
                    if(sdkConfig.getBoolean("gui.menu.file.preferences.general.automaticallyInstallUpdates.enabled", false)){
                        if(!download()){
                            status = STATUS_INSTALL;
                            break;
                        }
                    }
                    status = STATUS_SLEEP;
                    break;
                }
                case STATUS_INSTALL: {
                    if(sdkConfig.getBoolean("gui.menu.file.preferences.general.automaticallyInstallUpdates.enabled", false)){
                        install();
                    }
                    status = STATUS_SLEEP;
                    break;
                }
                case STATUS_EXIT: {
                    return;
                }
                default: {
                    log.error("Unknown status in Updater: " + status);
                    System.exit(0);
                    break;
                }
            }
        }
    }

//-------------------------------------------------------------------------------------
//        sdkConfig.setProperty("last.project.location", _lastProjectLocation);
//        String _default_lastProjectLocation = FileSystemView.getFileSystemView().getHomeDirectory().getAbsolutePath() + "/.xpu/projects/";
//        String _lastProjectLocation = sdkConfig.getString("last.project.location", _default_lastProjectLocation);


//sdk.version = 0.1.0
//sdk.libs.version = 0.1.0
//-------------------------------------------------------------------------------------
    private boolean check(){
        boolean _foundNewVersion = false;
/*        String _pathToLocalRepo = sdkConfig.getString("git.local.repo", null);
        if(_pathToLocalRepo == null){
            log.error("git.local.repo is not set in sdk.conf. Cannot install updates!");
            return false;
        }*/
        log.debug("Checking...");
        String _filePath = GIT_PATH;
        try {
            ReadableByteChannel _in = Channels.newChannel(new URL(DEFAULT_URL_UPDATE + _filePath).openStream());

            FileOutputStream _fileOS = new FileOutputStream(xpuHome + _filePath);
            FileChannel _out = _fileOS.getChannel();
            _out.transferFrom(_in, 0, Long.MAX_VALUE);
        } catch(Exception _e){
            log.error("Cannot retreive: " + _filePath + " from: " + DEFAULT_URL_UPDATE);
        }

//        log.debug("_foundNewVersion=" + _foundNewVersion + ",lastVersionRemote="+lastVersionRemote+", lastVersionInstalled="+lastVersionInstalled);
        return (lastVersionRemote.compareTo(lastVersionInstalled) > 0);
    }

//-------------------------------------------------------------------------------------
    private boolean download(){
        boolean _foundNewVersion = false;
        String _pathToLocalRepo = sdkConfig.getString("git.local.repo", null);
        if(_pathToLocalRepo == null){
            log.error("git.local.repo is not set in sdk.conf. Cannot install updates!");
            return false;
        }

        log.debug("Downloading...");
/*        try {

        } catch(IOException _e){
            log.error("Cannot open local repository: " + _pathToLocalRepo + ": " + _e.getMessage());
            _e.printStackTrace();
            return false;
        }*/
        return _foundNewVersion & (lastVersionRemote.compareTo(lastVersionInstalled) > 0);
    }

//-------------------------------------------------------------------------------------
    private boolean install(){
        String _pathToLocalRepo = sdkConfig.getString("git.local.repo", null);
        if(_pathToLocalRepo == null){
            log.error("git.local.repo is not set in sdk.conf. Cannot install updates!");
            return false;
        }

        lastVersionInstalled = lastVersionRemote;
        return true;
    }

//-------------------------------------------------------------------------------------
    private boolean setLastVersion(String _releasePath){
        Path _path = Paths.get(_releasePath);
        String _version = _path.getFileName().toString();        
        if(compare(_version, lastVersionRemote)){
            lastVersionRemote = _version;
            return true;
        }
        return false;
    }

//-------------------------------------------------------------------------------------
    private boolean compare(String _version, String _currentLastVersion){
        return _version.trim().compareTo(_currentLastVersion.trim()) > 0;
    }

//-------------------------------------------------------------------------------------
    public void triggerCheckForUpdates() {
        this.interrupt();
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
