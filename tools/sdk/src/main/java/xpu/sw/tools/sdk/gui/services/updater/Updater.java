//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.services.updater;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.nio.file.*;
import java.nio.file.StandardCopyOption.*;

import org.apache.http.*;
import org.apache.http.client.methods.*;
import org.apache.http.impl.client.*;
import org.apache.http.util.*;

import com.fasterxml.jackson.core.*;
import com.fasterxml.jackson.databind.*;

import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.xbasics.*;
import xpu.sw.tools.sdk.common.utils.*;

//-------------------------------------------------------------------------------------
public class Updater extends XStatus {
    private org.apache.commons.configuration2.Configuration sdkConfig;

    private int mode;
    private Object __objUpdateLocker;
    private UpdaterListener updaterListener;

/*    private static final int STATUS_SLEEP = 0;
    private static final int STATUS_CHECK = 1;
    private static final int STATUS_DOWNLOAD = 2;
    private static final int STATUS_INSTALL = 3;
    private static final int STATUS_EXIT = 4;
*/

    public static final int MODE_SILENT    = 0;
    public static final int MODE_FORCE     = 1;

    private UpdateList updateList;

//-------------------------------------------------------------------------------------
    public Updater(Context _context) {
        this(_context, MODE_SILENT);
    }

//-------------------------------------------------------------------------------------
    public Updater(Context _context, int _mode) {
        super(_context);
        sdkConfig = context.getSdkConfig();
        mode = _mode;
        __objUpdateLocker = new Object();
//            log.debug("Try update from url: " + url);


        setRunning();
        if(_mode == MODE_SILENT){
//            updateList = new UpdateList(_context, _mode);
//            start();
        } else if(_mode == MODE_FORCE){
            log.debug("Requesting explicit update...");
            updateList = new UpdateList(_context, _mode);
            run();
        }
    }

//-------------------------------------------------------------------------------------
    public void run() {
        while (isRunning()) {
            try {
                Thread.sleep((mode == MODE_FORCE) ? 0 : 36000);
            } catch (InterruptedException _e) {
            }
            if(sdkConfig.getBoolean("gui.menu.file.preferences.general.automaticallyCheckForUpdates.enabled", true) || (mode == MODE_FORCE)) {
                updateList.check();
            }
            if(sdkConfig.getBoolean("gui.menu.file.preferences.general.automaticallyInstallUpdates.enabled", true) || ((mode == MODE_FORCE))) {
                updateList.download();
                updateList.install();
            }
            if(mode == MODE_FORCE){
                break;
            }
        }
    }


//-------------------------------------------------------------------------------------
    public void triggerCheckForUpdates() {
        this.interrupt();
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
