//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.fileformats.xpu_tensor;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.isa.flow.*;
import xpu.sw.tools.sdk.common.isa.instruction.*;
import xpu.sw.tools.sdk.common.fileformats.core.*;
import xpu.sw.tools.sdk.common.fileformats.abstractexecutable.*;

//-------------------------------------------------------------------------------------
public class XpuTensorFile extends AbstractExecutableFile {
    public static final String EXTENSION = "xpu_tensor";

//-------------------------------------------------------------------------------------
    public XpuTensorFile(Logger _log, String _path) {
        super(_log, _path, EXTENSION);
    }


//-------------------------------------------------------------------------------------
    public void load() {

    }


//-------------------------------------------------------------------------------------
    public void save() {

    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
