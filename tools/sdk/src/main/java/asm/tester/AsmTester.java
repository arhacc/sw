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

import xpu.sw.tools.sdk.*;
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
            String[] _args = new String[]{_testDirectory.getAbsolutePath()};
            CommandLine _commandLine = Sdk.getCommandLine(_args);
            _contextTest.setCommandLine(_commandLine);
            AsmLinker _linker = new AsmLinker(_contextTest, _errorListener);
            compareHexFiles(_testDirectory);
        }
    }

//-------------------------------------------------------------------------------------
    private void compareHexFiles(File _testDirectory) {
        List<File> _listOfHexFiles = Arrays.asList(_testDirectory.listFiles(new FilenameFilter() {
            public boolean accept(File _dirFiles, String _filename) {
                _filename = _filename.toLowerCase();
                return !_filename.endsWith(".expected.hex") &&
                        _filename.toLowerCase().endsWith(".hex");
            }
        }));
        
        List<File> _listOfExpectedHexFiles = Arrays.asList(_testDirectory.listFiles(new FilenameFilter() {
            public boolean accept(File _dirFiles, String _filename) {
                return _filename.toLowerCase().endsWith(".expected.hex");
            }
        }));

        _listOfHexFiles.forEach(_hexFile -> {
            File _expectedHexFile = null;
            String _hexFilePath = _hexFile.getAbsolutePath();
            String _expectedHexPath = _hexFilePath.substring(0, _hexFilePath.length() - 4) + ".expected.hex";
            for(int i = 0; i < _listOfExpectedHexFiles.size(); i++){
                File _expectedHexFileInList = _listOfExpectedHexFiles.get(i);
                if(_expectedHexFileInList.getAbsolutePath().equals(_expectedHexPath)){
                    _expectedHexFile = _expectedHexFileInList;
                }
            }
            if(_expectedHexFile == null){
                log.error("Cannot find hex file for the expected.hex:" + _expectedHexFile.getAbsolutePath());
            } else {
                compareHexFiles(_hexFile, _expectedHexFile);
                _listOfExpectedHexFiles.remove(_expectedHexFile);                
            }
        });

        _listOfExpectedHexFiles.forEach(_expectedHexFile -> {
            log.error("Cannot find hex file for the expected.hex:" + _expectedHexFile.getAbsolutePath());
        });
    }

//-------------------------------------------------------------------------------------
    private void compareHexFiles(File _hex, File _expectedHex) {
        List<String> _hexLines = getLines(_hex);
        List<String> _expectedHexLines = getLines(_expectedHex);

        int _hexLinesSize = (_hexLines != null) ? _hexLines.size() : 0;
        int _expectedHexLinesSize = (_expectedHexLines != null) ? _expectedHexLines.size() : 0;

        int _maxSize = Math.max(_hexLinesSize, _expectedHexLinesSize);
        for(int i = 0; i < _maxSize; i++){
            String _hexLine = (i < _hexLinesSize) ? _hexLines.get(i) : "";
            String _expectedHexLine  = (i < _expectedHexLinesSize) ? _expectedHexLines.get(i) : "";
            if(!compareHexLine(_hexLine, _expectedHexLine)){
                log.error("Hex don't match at line --> " + i);
                break;
            }
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
