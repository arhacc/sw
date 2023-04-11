//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.asm.tester;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.nio.file.*;
import java.util.*;

import org.apache.commons.configuration2.*;
import org.apache.logging.log4j.*;

import org.apache.commons.cli.*;
import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;

import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.tree.*;

import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.project.*;
import xpu.sw.tools.sdk.common.isa.*;
import xpu.sw.tools.sdk.common.fileformats.hex.*;
import xpu.sw.tools.sdk.common.fileformats.obj.*;
import xpu.sw.tools.sdk.common.fileformats.json.*;

import xpu.sw.tools.sdk.asm.parser.*;
import xpu.sw.tools.sdk.asm.linker.*;

//-------------------------------------------------------------------------------------
public class AsmTester {
    private Context context;
    private Logger log;
    private ANTLRErrorListener errorListener;

    private String PATH_TESTS = "/sw/libraries/asm_tests";

//-------------------------------------------------------------------------------------
    public AsmTester(Context _context, ANTLRErrorListener _errorListener) {
        context = _context;
        log = _context.getLog();
        errorListener = (_errorListener == null) ? (new AsmErrorListener()) : _errorListener;
        
        String _gitLocalRepo = _context.getSdkConfig().getString("git.local.repo");
        String _testsPath = _gitLocalRepo + PATH_TESTS;
        log.debug("Load tests from [" + _testsPath + "]...");
        File[] _testDirectories = new File(_testsPath).listFiles(File::isDirectory);
        if(_testDirectories == null){
            log.debug("Cannot find tests!\nExiting...");
            return;
        }
        Context _contextTest = _context.getCopy();
        for (File _testDirectory : _testDirectories) { 
            log.debug("Testing [" + _testDirectory.getName() + "]...");
            AsmLinker _linker = new AsmLinker(_context, _errorListener);
        }
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
