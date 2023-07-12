//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk;

//-------------------------------------------------------------------------------------
import xpu.sw.tools.sdk.common.context.logs.LogUtil;
import org.apache.commons.cli.*;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import xpu.sw.tools.sdk.asm.Asm;
import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.context.arch.*;
import xpu.sw.tools.sdk.common.logs.XpuSdkLogLoader;
import xpu.sw.tools.sdk.gui.Gui;
import xpu.sw.tools.sdk.rexec.Rexec;
import xpu.sw.tools.sdk.simulator.Simulator;
import xpu.sw.tools.sdk.simulator.goldenmodel.Accelerator;
import xpu.sw.tools.sdk.gui.services.updater.*;


//-------------------------------------------------------------------------------------
public class Sdk implements Runnable {
    private Context context;
    private static XpuSdkLogLoader mpuSdkLogLoader;
    private static org.apache.logging.log4j.Logger log;

    private Asm asm;
    private Rexec rexec;
    private Gui gui;
    private Simulator sim;

//-------------------------------------------------------------------------------------
    public static void main(String[] _args) {
        Thread.currentThread().setName("sdk");

        mpuSdkLogLoader = new XpuSdkLogLoader();
        log = LogManager.getLogger(Sdk.class);

        // Sets the application name on the menu bar
        System.setProperty("Xdock:name", "XPU SDK");

        CommandLine _commandLine = getCommandLine(_args);

        if (_commandLine != null) {
            new Sdk(log, _commandLine);
        }
    }

//-------------------------------------------------------------------------------------

    public Sdk(Logger _log, CommandLine _commandLine) {
        context = new Context(this, _log, _commandLine);

        try {
            Thread _t = new Thread(this);
            _t.setName("Shutdown");
//            Runtime.getRuntime().addShutdownHook(_t);
//            Runtime.getRuntime().runFinalizersOnExit(true);
            Runtime.getRuntime().addShutdownHook(_t);
            LogUtil.disableShutDown();
        } catch (Throwable _t) {
            log.debug("[Main thread] Could not add Shutdown hook");
        }

        if (_commandLine.hasOption("update")) {
            new Updater(context, Updater.MODE_FORCE);
            System.exit(0);
        }

        if (!_commandLine.hasOption("cmd")) {
            log.debug("Missing argument: cmd");
//            printUsage(_options);
        } else {
            String _cmd = _commandLine.getOptionValue("cmd");
            switch (_cmd) {
                case "asm": {
                    asm = new Asm(context);
                    break;
                }
                case "rexec": {
                    rexec = new Rexec(context);
                    break;
                }
                case "gui": {
                    gui = new Gui(context);
                    break;
                }
                case "sim": {
                    //TODO: pass architecture from comand line
                    ArchitectureImplementation _architectureImplementation = context.getArchitectureImplementations().getArchitecture("noarch");
                    sim = new Simulator(context, _architectureImplementation);
                    break;
                }
                case "testsim": {
                    //TODO: pass architecture from comand line
                    ArchitectureImplementation _architectureImplementation = context.getArchitectureImplementations().getArchitecture("noarch");
                    Simulator.testSimulator(context, _architectureImplementation);
                    break;
                }
                case "testacc": {
                    //TODO: pass architecture from comand line
                    ArchitectureImplementation _architectureImplementation = context.getArchitectureImplementations().getArchitecture("noarch");
                    Accelerator.testAccelerator(context, _architectureImplementation);
                    break;
                }

                default: {
                    log.debug("Unknown sdk command: " + _cmd);
//                    printUsage(_options);
                }
            }
        }
    }

//-------------------------------------------------------------------------------------
    public static CommandLine getCommandLine(String[] _args) {
        Options _options = new Options();
        _options.addOption("cmd", true, "command. Possible commands: [asm] [gui] ");
        _options.addOption("testasm", false, "Start AsmTester");
        _options.addOption("update", false, "Update SDK");

        _options.addOption("prf", "profiling", false, "Profiling flag");

        CommandLineParser _parser = new DefaultParser();
        CommandLine _commandLine = null;
        try {
            _commandLine = _parser.parse(_options, _args);
        } catch (ParseException _e) {
            log.debug("Error: " + _e.getMessage());
//            printUsage(_options);
        }
        return _commandLine;
    }

//-------------------------------------------------------------------------------------
    public void run() {
//        log.debug("Shutdown hook...");
        log.debug("Saving...");
        context.save();
        if (asm != null) {
//            asm.save();
        }
        if (rexec != null) {
//            rexec.save();
        }
        if (gui != null) {
            gui.save();
        }
        if (sim != null) {
//            sim.save();
        }
    }

//-------------------------------------------------------------------------------------
    public static void printUsage(Options _options) {
        HelpFormatter _formatter = new HelpFormatter();
        _formatter.printHelp("sdk", _options);
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
