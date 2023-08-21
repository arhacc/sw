//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.utils;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.nio.file.*;
import java.util.*;
import java.util.stream.*;

import org.apache.logging.log4j.*;

//-------------------------------------------------------------------------------------
public class FileUtils {

//-------------------------------------------------------------------------------------
    public static void ifDoesntExistCreate(String _path){
        File _file = new File(_path);
        if(!_file.exists()){
            _file.mkdirs();
        }
    }

//-------------------------------------------------------------------------------------
    public static List<String> findFilesInDirectory(Path _path, String _fileExtension) throws IOException {
        List<String> _result;

        try (Stream<Path> _walk = Files.walk(_path)) {
            _result = _walk
                    .filter(p -> !Files.isDirectory(p))
                    // this is a path, not string,
                    // this only test if path end with a certain path
                    //.filter(p -> p.endsWith(fileExtension))
                    // convert path to string first
                    .map(p -> p.toString())
                    .filter(f -> f.endsWith(_fileExtension))
                    .collect(Collectors.toList());
        }

        return _result;
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
