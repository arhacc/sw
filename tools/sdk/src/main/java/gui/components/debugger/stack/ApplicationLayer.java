//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.components.debugger.stack;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.nio.file.*;
import java.util.*;
import java.util.stream.*;

import org.apache.commons.configuration2.*;
import org.apache.logging.log4j.*;

import org.apache.commons.cli.*;
import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;

import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.tree.*;

import xpu.sw.tools.sdk.*;
import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.project.*;
import xpu.sw.tools.sdk.common.fileformats.hex.*;
import xpu.sw.tools.sdk.common.fileformats.obj.*;
import xpu.sw.tools.sdk.asm.parser.*;

//-------------------------------------------------------------------------------------
public class ApplicationLayer extends CommandLayer {

//-------------------------------------------------------------------------------------
    public ApplicationLayer(Context _context) {
        super(_context);
    }

//-------------------------------------------------------------------------------------
    protected void send(ObjFile _objFile) {
/*        while(status != STATUS_RUNNING){
            try{
                sleep(1000);
            }catch(InterruptedException _e){

            }
        }*/
        sendCode(_objFile.getCodeSegment());
        sendData(_objFile.getDataSegment());
    }

//-------------------------------------------------------------------------------------
    protected void disconnect() {
        closeConnection();
    }
//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
