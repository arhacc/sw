package xpu.sw.tools.sdk.simulator;

import org.apache.commons.configuration2.Configuration;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import xpu.sw.tools.sdk.asm.Asm;
import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.context.arch.*;
import xpu.sw.tools.sdk.common.project.Project;
import xpu.sw.tools.sdk.rexec.remotehandler.*;
import xpu.sw.tools.sdk.rexec.remotehandler.stack.NetworkLayer;

import java.net.*;
import java.io.*;

public class Simulator {
    private Context context;
    private Logger log;

    public Simulator(Context _context, ArchitectureImplementation _architectureImplementation) {
        context = _context;
        log = _context.getLog();

        //HexFile HF = new HexFile(log, "/home/calin/.xpu/projects/XpuProject4/XpuProject4.hex");
        //HF.load();
        //System.out.println(HF.toString());
        //HF.deasm();
        //System.out.println(HF.toString());

        int portNumber = _context.getPort();
        boolean listening = true;

        log.debug("Server started and listening...");
        try (ServerSocket serverSocket = new ServerSocket(portNumber)) {
            while (listening) {
                Socket s = serverSocket.accept();
                log.debug("New client connected");
                new SimulatorThread(s, _context, _architectureImplementation).start();
            }
        } catch (IOException e) {
            System.err.println("Could not listen on port " + portNumber);
            System.exit(-1);
        }
    }

    public static void testSimulator(Context _context, ArchitectureImplementation _architectureImplementation) {
        _context.setHost("127.0.0.1");
        _context.setPort(9000);
        // create simulator server
        Thread server = new Thread() {
            public void run() {
                Configuration sdkConfig = _context.getSdkConfig();
                java.util.List<String> _openProjectsPaths = sdkConfig.getList(String.class, "open_projects");
                for (String path : _openProjectsPaths) {
                    Project _project = new Project(_context, path);
                    if (_project.isValid()) {
                        Asm asm = new Asm(_context);
                        break;
                    }
                }

                Simulator sim = new Simulator(_context, _architectureImplementation);
                while (isInterrupted()) {
                    try {
                        Thread.sleep(1);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                        break;
                    }
                }
            }
        };
        server.start();

        //test with client:
        RemoteHandler remoteHandler = new RemoteHandler(_context);
    }

    public static void main(String[] _args) {
        Thread.currentThread().setName("simulator");
        Logger log = LogManager.getLogger(Simulator.class);

        // Sets the application name on the menu bar
        //System.setProperty("Xdock:name", "XPU SDK");

        //CommandLine _commandLine = getCommandLine(_args);
        //Context context = new Context(null, log, _commandLine);
    }
}
