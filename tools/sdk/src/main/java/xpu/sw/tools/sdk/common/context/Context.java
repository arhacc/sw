//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.context;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;
import java.time.*;

import org.apache.commons.cli.*;
import org.apache.commons.configuration2.*;
import org.apache.logging.log4j.*;
import xpu.sw.tools.sdk.*;

import xpu.sw.tools.sdk.common.utils.*;
import xpu.sw.tools.sdk.common.context.arch.*;
import xpu.sw.tools.sdk.common.context.config.*;
import xpu.sw.tools.sdk.common.context.version.*;

//-------------------------------------------------------------------------------------
public class Context {
    private Sdk sdk;
    private Logger log;
    private CommandLine commandLine;
    private ConfigurationContainer configurationContainer;
    private Configuration sdkConfig;
    private VersionApp versionApp;
    private String sessionId;
    private long startTime;
    private String pathToSdkHome;
    private int state;
    private int debugMode;

//--- XPU Config
    private ArchitectureImplementations architectureImplementations;

    public static final String PROFILE_LEVEL_APP            = "AppLevel";
    public static final String PROFILE_LEVEL_MID            = "MidLevel";
    public static final String PROFILE_LEVEL_LOW            = "LowLevel";

    public static final int CONTEXT_STATE_INIT              = 0;
    public static final int CONTEXT_STATE_RUNNING           = 1;

    public static final int DEBUG_MODE_OFF                  = 0;
    public static final int DEBUG_MODE_ON                   = 1;

//-------------------------------------------------------------------------------------
    public Context(Sdk _sdk, Logger _log, CommandLine _commandLine) {
        sdk = _sdk;
        log = _log;
        commandLine = _commandLine;

        TimeUtils.setZoneId(ZoneId.of("Europe/Bucharest"));
        pathToSdkHome = System.getProperty("user.home") + "/.xpu";
        versionApp = new VersionApp(log, new String[]{"xpu-sdk-libs", "xpu-sdk"});
//        log.info("Starting Sdk v" + getVersion() + " ...");

//        String _daoConfigurationPath = SystemUtils.getHomeDirectory() + "/.sdk/etc/";
        configurationContainer = new ConfigurationContainer(log, pathToSdkHome + "/etc/");
        sdkConfig = configurationContainer.getConfiguration("sdk");

        sessionId = TimeUtils.getTimeAsString();
        startTime = System.currentTimeMillis();
        state = CONTEXT_STATE_INIT;
        debugMode = sdkConfig.getInteger("debug", 0);
        architectureImplementations = new ArchitectureImplementations(this);
    }

//-------------------------------------------------------------------------------------
    public Context getCopy() {
        Context _context = new Context(sdk, log, commandLine);
        
        return _context;
    }

//-------------------------------------------------------------------------------------
    public Sdk getSdk() {
        return sdk;
    }

//-------------------------------------------------------------------------------------
    public Logger getLog() {
        return log;
    }

//-------------------------------------------------------------------------------------
    public void setLog(Logger _log) {
        log = _log;
    }

//-------------------------------------------------------------------------------------
    public String getProfile() {
        return getSdkConfig().getString("profile", PROFILE_LEVEL_APP);
    }

//-------------------------------------------------------------------------------------
    public CommandLine getCommandLine() {
        return commandLine;
    }

//-------------------------------------------------------------------------------------
    public void setCommandLine(CommandLine _commandLine) {
        commandLine = _commandLine;
    }

//-------------------------------------------------------------------------------------
    public ConfigurationContainer getConfigurationContainer() {
        return configurationContainer;
    }

//-------------------------------------------------------------------------------------
    public Configuration getSdkConfig() {
        return sdkConfig;
    }

//-------------------------------------------------------------------------------------
    public VersionApp getVersionObject() {
        return versionApp;
    }

//-------------------------------------------------------------------------------------
    public String getVersion() {
        return "v" + versionApp.getVersion(1);
    }

//-------------------------------------------------------------------------------------
    public String getSessionId() {
        return sessionId;
    }

//-------------------------------------------------------------------------------------
    public long getStartTime() {
        return startTime;
    }

//-------------------------------------------------------------------------------------
    public synchronized int getState() {
        return state;
    }

//-------------------------------------------------------------------------------------
    public synchronized int setState(int _state) {
        return state = _state;
    }

//-------------------------------------------------------------------------------------
    public String getPathToSdkHome() {
        return pathToSdkHome;
    }

//-------------------------------------------------------------------------------------
    public int getDebugMode() {
        return debugMode;
    }

//-------------------------------------------------------------------------------------
    public void setDebugMode(int _debugMode) {
        debugMode = _debugMode;
        sdkConfig.setProperty("debug", _debugMode);
//        log.debug("_debugMode=" + _debugMode);
//        new Throwable().printStackTrace();
    }

//-------------------------------------------------------------------------------------
// XPU configs
//-------------------------------------------------------------------------------------
    public ArchitectureImplementations getArchitectureImplementations() {
        return architectureImplementations;
    }

//-------------------------------------------------------------------------------------
    public void save() {
        configurationContainer.save();
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
