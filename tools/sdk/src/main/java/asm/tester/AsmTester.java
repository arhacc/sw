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
        
        List<String> _args = _context.getCommandLine().getArgList();
        if((_args == null) || (_args.size() == 0)){
            String _gitLocalRepo = _context.getSdkConfig().getString("git.local.repo");
            String _testPath = _gitLocalRepo + PATH_TESTS;
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
        } else if(_testPathFile.getName().endsWith(".asm")){
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
        compareHexFiles(_testFile);
    }

//-------------------------------------------------------------------------------------
    private void compareHexFiles(String _testFile) {
        String _basePath = _testFile.substring(0, _testFile.length() - 4);
        File _hexFile = new File(_basePath + ".hex");
        File _expectedHexFile = new File(_basePath + ".expected.hex");
        compareHexFiles(_hexFile, _expectedHexFile);

/*        List<File> _listOfHexFiles = Arrays.asList(_testDirectory.listFiles(new FilenameFilter() {
            public boolean accept(File _dirFiles, String _filename) {
                _filename = _filename.toLowerCase();
                return !_filename.endsWith(".expected.hex") &&
                        _filename.toLowerCase().endsWith(".hex");
            }
        }));
        
        List<File> _listOfExpectedHexFiles = new ArrayList<File>(Arrays.asList(_testDirectory.listFiles(new FilenameFilter() {
            public boolean accept(File _dirFiles, String _filename) {
                return _filename.toLowerCase().endsWith(".expected.hex");
            }
        })));

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
                log.error("Cannot find hex file for the expected.hex:" + _hexFile.getAbsolutePath());
            } else {
                compareHexFiles(_hexFile, _expectedHexFile);
                _listOfExpectedHexFiles.remove(_expectedHexFile);                
            }
        });

        _listOfExpectedHexFiles.forEach(_expectedHexFile -> {
            log.error("Cannot find hex file for the expected.hex:" + _expectedHexFile.getAbsolutePath());
        });
*/
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
