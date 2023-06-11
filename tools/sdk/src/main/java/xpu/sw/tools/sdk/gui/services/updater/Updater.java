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

    private int status;
    private Object __objUpdateLocker;
    private UpdaterListener updaterListener;

/*    private static final int STATUS_SLEEP = 0;
    private static final int STATUS_CHECK = 1;
    private static final int STATUS_DOWNLOAD = 2;
    private static final int STATUS_INSTALL = 3;
    private static final int STATUS_EXIT = 4;
*/

    private UpdateList updateList;

//-------------------------------------------------------------------------------------
    public Updater(Context _context) {
        super(_context);
        sdkConfig = context.getSdkConfig();
//        status = STATUS_CHECK;
        __objUpdateLocker = new Object();
//            log.debug("Try update from url: " + url);

        updateList = new UpdateList(_context);

        setRunning();
//        start();
    }

//-------------------------------------------------------------------------------------
    public void run() {
        while (isRunning()) {
            try {
                Thread.sleep(360);
            } catch (InterruptedException _e) {
            }
            if(sdkConfig.getBoolean("gui.menu.file.preferences.general.automaticallyCheckForUpdates.enabled", true)) {
                updateList.check();
            }
            if(sdkConfig.getBoolean("gui.menu.file.preferences.general.automaticallyInstallUpdates.enabled", true)) {
                updateList.download();
                updateList.install();
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
