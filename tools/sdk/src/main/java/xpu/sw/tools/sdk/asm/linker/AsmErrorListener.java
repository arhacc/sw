//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.asm.linker;
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

import xpu.sw.tools.sdk.common.project.*;
import xpu.sw.tools.sdk.common.fileformats.hex.*;
import xpu.sw.tools.sdk.asm.parser.*;

//-------------------------------------------------------------------------------------
public class AsmErrorListener extends BaseErrorListener {
//    private boolean success = true;

//-------------------------------------------------------------------------------------
    public void syntaxError(Recognizer<?,?> recognizer,
               Object offendingSymbol,
               int _line,
               int _charPositionInLine,
               String _msg,
               RecognitionException e){
        System.out.println("Error @ " +_line + ":" + _charPositionInLine + ": " +_msg);
//        success = false;
    }
/*
//-------------------------------------------------------------------------------------
    public boolean getSuccess () {
        return success;
    }
*/
//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
