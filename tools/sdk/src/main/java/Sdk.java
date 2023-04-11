/**
 * Graphics is the abstract base class for all graphics contexts
 * which allow an application to draw onto components realized on
 * various devices or onto off-screen images.
 * A Graphics object encapsulates the state information needed
 * for the various rendering opcodes that Java supports.  This
 * state information includes:
 * <ul>
 * <li>The Component to draw on
 * <li>A translation origin for rendering and clipping coordinates
 * <li>The current clip
 * <li>The current color
 * <li>The current font
 * <li>The current logical pixel opcode function (XOR or Paint)
 * <li>The current XOR alternation color
 * (see <a href="#setXORMode">setXORMode</a>)
 * </ul>
 * <p>
 * Coordinates are infinitely thin and lie between the pixels of the
 * output device.
 * Operations which draw the outline of a figure operate by traversing
 * along the infinitely thin path with a pixel-sized pen that hangs
 * down and to the right of the anchor point on the path.
 * Operations which fill a figure operate by filling the interior
 * of the infinitely thin path.
 * Operations which render horizontal text render the ascending
 * portion of the characters entirely above the baseline coordinate.
 * <p>
 * Some important points to consider are that drawing a figure that
 * covers a given rectangle will occupy one extra row of pixels on
 * the right and bottom edges compared to filling a figure that is
 * bounded by that same rectangle.
 * Also, drawing a horizontal line along the same y coordinate as
 * the baseline of a line of text will draw the line entirely below
 * the text except for any descenders.
 * Both of these properties are due to the pen hanging down and to
 * the right from the path that it traverses.
 * <p>
 * All coordinates which appear as arguments to the methods of this
 * Graphics object are considered relative to the translation origin
 * of this Graphics object prior to the invocation of the method.
 * All rendering opcodes modify only pixels which lie within the
 * area bounded by both the current clip of the graphics context
 * and the extents of the Component used to create the Graphics object.
 *
 * @author Marius C. Stoian
 * @author Gheorghe M. Stefan
 * @version %I%, %G%
 * @since 1.0
 */
//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk;

//-------------------------------------------------------------------------------------
import xpu.sw.tools.sdk.common.wrappers.logs.LogUtil;
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

//-------------------------------------------------------------------------------------
public class Sdk implements Runnable {

    private Context context;
    private static XpuSdkLogLoader mpuSdkLogLoader;
    private static org.apache.logging.log4j.Logger log;

//    private FileBasedConfigurationBuilder<FileBasedConfiguration> builderEnv;
//    private FileBasedConfigurationBuilder<FileBasedConfiguration> builderSdk;
//    private CombinedConfiguration sdkConfig;
//    private Version version;
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
//Options _options,
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
