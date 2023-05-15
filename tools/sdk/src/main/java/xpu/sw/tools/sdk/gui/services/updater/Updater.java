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

    private static final int STATUS_SLEEP = 0;
    private static final int STATUS_CHECK = 1;
    private static final int STATUS_DOWNLOAD = 2;
    private static final int STATUS_INSTALL = 3;
    private static final int STATUS_EXIT = 4;

    private UpdateList updateList;

//-------------------------------------------------------------------------------------
    public Updater(Context _context) {
        super(_context);
        sdkConfig = context.getSdkConfig();
        status = STATUS_CHECK;
        __objUpdateLocker = new Object();
//            log.debug("Try update from url: " + url);

        updateList = new UpdateList(_context);

        setRunning();
        start();
    }

//-------------------------------------------------------------------------------------
    public void run() {
        while (isRunning()) {
            switch (status) {
                case STATUS_SLEEP: {
                    try {
                        Thread.sleep(360000);
                    } catch (InterruptedException _e) {

                    }
                    status = STATUS_CHECK;
                    break;
                }
                case STATUS_CHECK: {
                    if (sdkConfig.getBoolean("gui.menu.file.preferences.general.automaticallyCheckForUpdates.enabled", true)) {
                        if (!updateList.check()) {
                            status = STATUS_SLEEP;
                        } else {
                            if (sdkConfig.getBoolean("gui.menu.file.preferences.general.automaticallyInstallUpdates.enabled", true)) {
                                status = STATUS_DOWNLOAD;
                            }
                        }
                    } else {
                        status = STATUS_SLEEP;
                    }
                    break;
                }
                case STATUS_DOWNLOAD: {
                    if (sdkConfig.getBoolean("gui.menu.file.preferences.general.automaticallyInstallUpdates.enabled", true)) {
                        if (!updateList.download()) {
                            status = STATUS_INSTALL;
                            break;
                        }
                    }
                    status = STATUS_SLEEP;
                    break;
                }
                case STATUS_INSTALL: {
                    if (sdkConfig.getBoolean("gui.menu.file.preferences.general.automaticallyInstallUpdates.enabled", true)) {
                        updateList.install();
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
    public void triggerCheckForUpdates() {
        this.interrupt();
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
