package xpu.sw.tools.sdk.simulator;

import xpu.sw.tools.sdk.common.context.Context;

import java.net.*;
import java.io.*;
import org.apache.logging.log4j.Logger;
import xpu.sw.tools.sdk.common.io.Command;
import xpu.sw.tools.sdk.simulator.goldenmodel.Accelerator;
import xpu.sw.tools.sdk.simulator.stack.NetworkUtil;

public class SimulatorThread extends Thread {
    private Socket socket = null;

    private Context context;
    private Logger log;
    private NetworkUtil net;
    private Accelerator acc;

    public SimulatorThread(Socket _socket, Context _context) {
        super("SimulatorThread");
        this.socket = _socket;
        this.context = _context;
        this.net = new NetworkUtil();
        this.acc = new Accelerator(_context);
    }

    public void run() {
        try {
            log = context.getLog();

            net.inStream = new DataInputStream(socket.getInputStream());
            net.outStream = new DataOutputStream(socket.getOutputStream());

            byte hello = net.receiveByte();
            if (hello == net.ERROR_VALUE) { }
            else {
                net.sendInt(Command.COMMAND_DONE);
                log.debug("Hello received");
                int commandByte;
                do {
                    commandByte = net.receiveByte();
                    log.debug("Receiving command " + Command.toString((byte)commandByte) + "(" + commandByte + ")");

                    if (commandByte == net.ERROR_VALUE) break;
                    switch (commandByte) {
                        case Command.COMMAND_OPEN_CONNECTION: {
                            log.debug("Hello received again");
                            net.sendInt(Command.COMMAND_DONE);
                            break;
                        }

                        case Command.COMMAND_LOAD_CODE_MEMORY: {
                            log.debug("Loading code to accelerator...");
                            int address = net.receiveInt();
                            int payloadLength = net.receiveInt();
                            long[] payload = net.receiveLongArray(payloadLength);
                            if (acc.uploadCode(address, payload)) {
                                net.sendInt(Command.COMMAND_DONE);
                                log.debug("Done!");
                            }
                            else {
                                net.sendInt(Command.COMMAND_ERROR);
                                log.error("Failed!");
                            }
                            break;
                        }

                        case Command.COMMAND_LOAD_DATA_MEMORY: {
                            int address = net.receiveInt();
                            int payloadLength = net.receiveInt();
                            long[] payload = net.receiveLongArray(payloadLength);
                            if (acc.uploadData(address, payload))
                                net.sendInt(Command.COMMAND_DONE);
                            else
                                net.sendInt(Command.COMMAND_ERROR);
                            break;
                        }

                        case Command.COMMAND_UNLOAD_DATA_MEMORY: {
                            int address = net.receiveInt();
                            int payloadLength = net.receiveInt();

                            long[] payload = acc.downloadData(address, payloadLength);

                            net.sendInt(Command.COMMAND_DONE);
                            net.sendInt(payloadLength);
                            net.sendLongArray(payload);
                        }

                        case Command.COMMAND_LOAD_FEATURE_MEMORY: {
                            int address = net.receiveInt();
                            int payloadLength = net.receiveInt();
                            long[] payload = net.receiveLongArray(payloadLength);
                            if (acc.uploadFeatures(address, payload))
                                net.sendInt(Command.COMMAND_DONE);
                            else
                                net.sendInt(Command.COMMAND_ERROR);
                            break;
                        }

                        case Command.COMMAND_UNLOAD_FEATURE_MEMORY: {
                            int address = net.receiveInt();
                            int payloadLength = net.receiveInt();

                            long[] payload = acc.downloadFeatures(address, payloadLength);

                            net.sendInt(Command.COMMAND_DONE);
                            net.sendInt(payloadLength);
                            net.sendLongArray(payload);
                        }

                        case Command.COMMAND_CLOSE_CONNECTION: {
                            net.sendInt(Command.COMMAND_DONE);
                            break;
                        }

                        default: {
                            log.error("Not-implemented command of " + commandByte + " was received");
                            break;
                        }
                    }

                    if (commandByte == Command.COMMAND_ERROR) break;
                } while (commandByte != Command.COMMAND_CLOSE_CONNECTION);
            }
            socket.close();
            log.debug("Client disconnected");
            }
        catch (IOException e) {
            e.printStackTrace();
        }
    }

}