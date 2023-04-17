//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.asm;

//-------------------------------------------------------------------------------------
import org.apache.commons.cli.*;
import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;

import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.tree.*;

import xpu.sw.tools.sdk.*;
import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.asm.parser.*;
import xpu.sw.tools.sdk.asm.tester.*;
import xpu.sw.tools.sdk.asm.linker.*;

//-------------------------------------------------------------------------------------
public class Asm {
    private Sdk sdk;
    private Logger log;
    private AsmTester tester;
    private AsmLinker linker;

//-------------------------------------------------------------------------------------
    public Asm(Context _context) {
        this(_context, null);
    }

//-------------------------------------------------------------------------------------
    public Asm(Context _context, ANTLRErrorListener _errorListener) {
        sdk = _context.getSdk();
        log = _context.getLog();
        log.debug("Starting XPU Asm " + _context.getVersion() + "...");
//        String[] _args = _cmd.getArgs();
        CommandLine _commandLine = _context.getCommandLine();
        if(_commandLine.hasOption("testasm")){
            tester = new AsmTester(_context, _errorListener);
        } else {
            linker = new AsmLinker(_context, _errorListener);
        }

    }

//-------------------------------------------------------------------------------------

}
//-------------------------------------------------------------------------------------
