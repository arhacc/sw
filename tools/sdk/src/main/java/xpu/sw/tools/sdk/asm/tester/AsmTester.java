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
import xpu.sw.tools.sdk.common.io.*;
import xpu.sw.tools.sdk.common.isa.flow.*;
import xpu.sw.tools.sdk.common.isa.instruction.*;
import xpu.sw.tools.sdk.common.fileformats.hex.*;
import xpu.sw.tools.sdk.common.fileformats.obj.*;
import xpu.sw.tools.sdk.common.fileformats.json.*;

import xpu.sw.tools.sdk.*;
import xpu.sw.tools.sdk.asm.parser.*;
import xpu.sw.tools.sdk.asm.linker.*;

//-------------------------------------------------------------------------------------
public class AsmTester {
    private Context context;
    private Logger log;
    private ANTLRErrorListener errorListener;

    private String PATH_TESTS = "low_level/tests";

//-------------------------------------------------------------------------------------
    public AsmTester(Context _context, ANTLRErrorListener _errorListener) {
        context = _context;
        log = _context.getLog();
        errorListener = (_errorListener == null) ? (new AsmErrorListener()) : _errorListener;
        
        List<String> _args = _context.getCommandLine().getArgList();
        if((_args == null) || (_args.size() == 0)){
//            String _gitLocalRepo = ;//_context.getSdkConfig().getString("git.local.repo");
            String _testPath = PathResolver.XPU_LIBRARIES_PATH + PATH_TESTS;
            log.debug("Load all tests from: [" + _testPath + "]");
            testPath(_testPath);
        } else {
            _args.forEach(_testPath -> testPath(_testPath));
        }
    }

//-------------------------------------------------------------------------------------
    private void testPath(String _testPath) {
        File _testPathFile = new File(_testPath);
        if(_testPathFile.isDirectory()){
            log.debug("Load tests from [" + _testPath + "]...");
            File[] _fileList = _testPathFile.listFiles();
            for(File _file : _fileList) {
                testPath(_file.getAbsolutePath());
            }
        } 
        String _filename = _testPathFile.getName();
        if(_filename.endsWith(".asm") && !_filename.equals("configurations.asm")){
            testFile(_testPath);
        }
    }

//-------------------------------------------------------------------------------------
    private void testFile(String _testFile) {
        Context _contextTest = context.getCopy();
//        log.debug("Testing [" + _testFile + "]...");
        String[] _args = new String[]{_testFile};
        CommandLine _commandLine = Sdk.getCommandLine(_args);
        _contextTest.setCommandLine(_commandLine);
        AsmLinker _linker = new AsmLinker(_contextTest, errorListener);
        compareHexFiles(_linker, _testFile);
    }

//-------------------------------------------------------------------------------------
    private void compareHexFiles(AsmLinker _linker, String _testFile) {
        String _basePath = _testFile.substring(0, _testFile.length() - 4);
        File _hexFile = new File(_basePath + ".hex");
        File _expectedHexFile = new File(_basePath + ".expected_hex");
//        compareHexFiles(_hexFile, _expectedHexFile);
        List<String> _hexLines = getLines(_hexFile);
        List<String> _expectedHexLines = getLines(_expectedHexFile);

        int _hexLinesSize = (_hexLines != null) ? _hexLines.size() : 0;
        int _expectedHexLinesSize = (_expectedHexLines != null) ? _expectedHexLines.size() : 0;

        int _maxSize = Math.max(_hexLinesSize, _expectedHexLinesSize);
        int _errorCounter = 0;
        for(int i = 0; i < _maxSize; i++){
            String _hexLine = (i < _hexLinesSize) ? _hexLines.get(i) : "";
            String _expectedHexLine  = (i < _expectedHexLinesSize) ? _expectedHexLines.get(i) : "";
//            _expectedHexLine = _expectedHexLine.trim().replaceAll(" ", "_").toUpperCase();
            _expectedHexLine = _expectedHexLine.trim().toUpperCase();
            if(!compareHexLine(_hexLine, _expectedHexLine)){
//                break;
                if(_errorCounter < 10){
                    log.error("Hex doesn't match at index [" + i + "] --> [" + _hexLine + "] should be [" + _expectedHexLine + "] --> " + _linker.getLineTextAt(i));
                }
                _errorCounter++;
            }
        }
        if(_errorCounter >= 10){
            log.error("More errors(" + (_errorCounter - 9) + ")...");
        }
    }

//-------------------------------------------------------------------------------------
    private boolean compareHexLine(String _hexLine, String _expectedHexLine) {
        return _hexLine.trim().equals(_expectedHexLine.trim());
    }

//-------------------------------------------------------------------------------------
    private List<String> getLines(File _file) {
        try {
            List<String> _allLines = Files.readAllLines(_file.toPath());
            return _allLines;
        } catch (IOException _e) {
            _e.printStackTrace();
        }
        return null;
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
