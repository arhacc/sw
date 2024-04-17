//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.rexec.remotehandler.resolver;
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

import xpu.sw.tools.sdk.common.utils.*;

import xpu.sw.tools.sdk.*;
import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.project.*;
import xpu.sw.tools.sdk.common.xbasics.*;
import xpu.sw.tools.sdk.common.fileformats.core.*;
import xpu.sw.tools.sdk.common.fileformats.obj.*;
import xpu.sw.tools.sdk.common.fileformats.json.*;
import xpu.sw.tools.sdk.common.fileformats.onnx.*;
import xpu.sw.tools.sdk.common.io.*;
import xpu.sw.tools.sdk.common.io.targetmanager.*;
import xpu.sw.tools.sdk.asm.parser.*;

//-------------------------------------------------------------------------------------
public class Resolver extends XBasic {
    private Map<String, Path> repositories;

//-------------------------------------------------------------------------------------
    public Resolver(Context _context) {
        super(_context);
        repositories = new HashMap<String, Path>();
        addLocalLibraries();
    }

//-------------------------------------------------------------------------------------
    private void addLocalLibraries() {
        addLocalLibrary("app_level");
        addLocalLibrary("mid_level");
        addLocalLibrary("low_level");
    }

//-------------------------------------------------------------------------------------
    private void addLocalLibrary(String _name) {
        repositories.put(_name, Paths.get(PathResolver.ARHACC_PATH + PathResolver.separator + _name));
    }

//-------------------------------------------------------------------------------------
    public String resolve(String _graphNodeDescriptor) {
        String[] _graphNodeDescriptorParts = _graphNodeDescriptor.split("[@#-]");

        for (Map.Entry<String, Path> _entry : repositories.entrySet()) {
            String _path = resolve(_entry.getKey(), _entry.getValue(), _graphNodeDescriptorParts);
            if(_path != null){
                return _path;
            }
        }
        return null;
    }

//-------------------------------------------------------------------------------------
    private String resolve(String _repositoryName, Path _repositoryPath, String[] _graphNodeDescriptorParts) {
        File[] _files = new File(_repositoryPath.toString()).listFiles();
        for (File _file : _files) {
            if (_file.isDirectory()) {
                String _path = resolve(_repositoryName, Paths.get(_file.getPath()), _graphNodeDescriptorParts);
                if(_path != null){
                    return _path;
                }
            } else if(_file.getName().equals(_graphNodeDescriptorParts[0])){
                return _file.getPath();
            }
        }
        return null;
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
