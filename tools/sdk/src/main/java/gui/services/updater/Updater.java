//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.services.updater;
//-------------------------------------------------------------------------------------

import codex.common.apps.rxbasics.RxStatus;
import codex.common.utils.SystemUtils;
import java.nio.file.Path;
import java.nio.file.Paths;
import org.apache.http.HttpResponse;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClientBuilder;
import org.apache.http.util.EntityUtils;
import xpu.sw.tools.sdk.common.context.Context;

//-------------------------------------------------------------------------------------
public class Updater extends RxStatus {

    private Context context;
    private org.apache.commons.configuration2.Configuration sdkConfig;

    private int status;
    private String xpuHome;
    private Object __objUpdateLocker;
    private UpdaterListener updaterListener;
    private String lastVersionRemote;
    private String lastVersionInstalled;

    private static final int STATUS_SLEEP = 0;
    private static final int STATUS_CHECK = 1;
    private static final int STATUS_DOWNLOAD = 2;
    private static final int STATUS_INSTALL = 3;
    private static final int STATUS_EXIT = 4;

    private static final String DEFAULT_URL_UPDATE = "https://api.github.com/repos/arhacc/sw/releases/latest";

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
                    if (sdkConfig.getBoolean("gui.menu.file.preferences.general.automaticallyCheckForUpdates.enabled",
                            false)) {
                        if (!check()) {
                            status = STATUS_SLEEP;
                        } else {
                            if (sdkConfig.getBoolean(
                                    "gui.menu.file.preferences.general.automaticallyInstallUpdates.enabled", false)) {
                                status = STATUS_DOWNLOAD;
                            }
                        }
                    } else {
                        status = STATUS_SLEEP;
                    }
                    break;
                }
                case STATUS_DOWNLOAD: {
                    if (sdkConfig.getBoolean("gui.menu.file.preferences.general.automaticallyInstallUpdates.enabled",
                            false)) {
                        if (!download()) {
                            status = STATUS_INSTALL;
                            break;
                        }
                    }
                    status = STATUS_SLEEP;
                    break;
                }
                case STATUS_INSTALL: {
                    if (sdkConfig.getBoolean("gui.menu.file.preferences.general.automaticallyInstallUpdates.enabled",
                            false)) {
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
    private boolean check() {
        boolean _foundNewVersion = false;
        /*        String _pathToLocalRepo = sdkConfig.getString("git.local.repo", null);
        if(_pathToLocalRepo == null){
            log.error("git.local.repo is not set in sdk.conf. Cannot install updates!");
            return false;
        }*/
//        log.debug("Checking...");
        try {
            String url = DEFAULT_URL_UPDATE;
            log.debug("Try update from url: " + url);
            CloseableHttpClient httpClient = HttpClientBuilder.create().build();
            HttpGet request = new HttpGet(url);
            request.addHeader("content-type", "application/vnd.github+json");
            HttpResponse result = httpClient.execute(request);
            String json = EntityUtils.toString(result.getEntity(), "UTF-8");
//            JsonObject myObject = new JsonObject(json);
//            System.out.println(json);

        } catch (Exception _e) {
            log.error("Cannot update from: " + DEFAULT_URL_UPDATE);
        }

//        log.debug("_foundNewVersion=" + _foundNewVersion + ",lastVersionRemote="+lastVersionRemote+", lastVersionInstalled="+lastVersionInstalled);
        return (lastVersionRemote.compareTo(lastVersionInstalled) > 0);
    }

//-------------------------------------------------------------------------------------
    private boolean download() {
        boolean _foundNewVersion = false;
        String _pathToLocalRepo = sdkConfig.getString("git.local.repo", null);
        if (_pathToLocalRepo == null) {
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
    private boolean install() {
        String _pathToLocalRepo = sdkConfig.getString("git.local.repo", null);
        if (_pathToLocalRepo == null) {
            log.error("git.local.repo is not set in sdk.conf. Cannot install updates!");
            return false;
        }

        lastVersionInstalled = lastVersionRemote;
        return true;
    }

//-------------------------------------------------------------------------------------
    private boolean setLastVersion(String _releasePath) {
        Path _path = Paths.get(_releasePath);
        String _version = _path.getFileName().toString();
        if (compare(_version, lastVersionRemote)) {
            lastVersionRemote = _version;
            return true;
        }
        return false;
    }

//-------------------------------------------------------------------------------------
    private boolean compare(String _version, String _currentLastVersion) {
        return _version.trim().compareTo(_currentLastVersion.trim()) > 0;
    }

//-------------------------------------------------------------------------------------
    public void triggerCheckForUpdates() {
        this.interrupt();
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
