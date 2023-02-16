//-------------------------------------------------------------------------------------
package codex.utils;

//-------------------------------------------------------------------------------------
import codex.common.utils.Debug;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.file.Path;
import java.nio.file.Paths;
import org.apache.logging.log4j.Logger;

//-------------------------------------------------------------------------------------
public class FileUtils {

//-------------------------------------------------------------------------------------
    public static File loadCryptoFile(Debug _debug, String _cryptoTarget, String _taskPath, String _extension) {
//        String _wavesHome = System.getenv("WAVES_HOME_PATH");
        String _filename = _taskPath + _cryptoTarget + _extension;
        File _file = new File(_filename);
        return _file;
    }

//-------------------------------------------------------------------------------------
    public static FileInputStream loadCryptoFileInputStream(Debug _debug, String _cryptoTarget, String _taskPath,
            String _extension) throws IOException {
//        String _wavesHome = System.getenv("WAVES_HOME_PATH");
        String _filename = _taskPath + _cryptoTarget + _extension;
        FileInputStream _fileIS = new FileInputStream(_filename);
        return _fileIS;
    }

//-------------------------------------------------------------------------------------
    public static FileOutputStream loadCryptoFileOutputStream(Debug _debug, String _cryptoTarget, String _taskPath,
            String _extension) throws IOException {
//        String _wavesHome = System.getenv("WAVES_HOME_PATH");
        String _filename = _taskPath + _cryptoTarget + _extension;
//        _debug.outln(Debug.IMPORTANT3, "Saving____ "+_filename+" ...");
        FileOutputStream _fileOS = new FileOutputStream(_filename);
        return _fileOS;
    }

//-------------------------------------------------------------------------------------
    public static void changeDirectory(Debug _debug, String _path) {
        String _wavesHome = System.getenv("WAVES_HOME_PATH");
        String _dirname = _wavesHome + _path;
        _debug.outln(Debug.IMPORTANT3, "Change directory to " + _dirname + " ...");
        System.setProperty("user.dir", _dirname);
//        _debug.outln(Debug.IMPORTANT3, " done");
    }

//-------------------------------------------------------------------------------------
    public static void deleteFile(Debug _debug, String _path) {
//        String _wavesHome = System.getenv("WAVES_HOME_PATH");
//        String _filename = _wavesHome + _path;
        _debug.outln(Debug.IMPORTANT3, "Deleting " + _path + " ...");
        File _file = new File(_path);
        _file.delete();
//        _debug.outln(Debug.IMPORTANT3, " done");
    }

//-------------------------------------------------------------------------------------
    public static void createDirectory(Debug _debug, String _path) {
        _debug.outln(Debug.IMPORTANT3, "Create " + _path + " ...");
        File _dir = new File(_path);
        _dir.mkdir();
    }

//-------------------------------------------------------------------------------------
    public static void deleteDirectoryContent(Debug _debug, String _path) {
//        String _wavesHome = System.getenv("WAVES_HOME_PATH");
//        String _dataDir = _wavesHome + _path;
        _debug.outln(Debug.IMPORTANT3, "Deleting " + _path + " ...");
        File _dir = new File(_path);
        File[] _listFiles = _dir.listFiles();
        for (File _file : _listFiles) {
            delete(_file);
//            int _code = _file.delete() ? Debug.IMPORTANT3 : Debug.IMPORTANT4;
//            _debug.out(_code, ".");
        }
//        _debug.outln(Debug.IMPORTANT3, " done");
    }

//-------------------------------------------------------------------------------------
    public static void deleteDirectoryContent(Logger _log, String _path) {
//        String _wavesHome = System.getenv("WAVES_HOME_PATH");
//        String _dataDir = _wavesHome + _path;
        _log.info("Deleting " + _path + " ...");
        File _dir = new File(_path);
        File[] _listFiles = _dir.listFiles();
        for (File _file : _listFiles) {
            delete(_file);
//            int _code = _file.delete() ? Debug.IMPORTANT3 : Debug.IMPORTANT4;
//            _debug.out(_code, ".");
        }
//        _debug.outln(Debug.IMPORTANT3, " done");
    }

//-------------------------------------------------------------------------------------
    public static void delete(File _file) {
        if (_file.isDirectory()) {
            File[] _listFiles = _file.listFiles();
            for (File _file1 : _listFiles) {
                delete(_file1);
            }
        }
        _file.delete();
    }

//-------------------------------------------------------------------------------------
    public static void ifDoesntExistCreate(String _path) {
        File _file = new File(_path);
        if (!_file.exists()) {
            _file.mkdirs();
        }
    }

//-------------------------------------------------------------------------------------
    public static String putFileInDirectory(String _filePath, String _directory) {
        if (_filePath == null) {
            return null;
        }
        Path _p = Paths.get(_filePath);
        String _path = _p.getParent().toString();
        String _filename = _p.getFileName().toString();
        String _directoryPath = _path + "/" + _directory;
        ifDoesntExistCreate(_directoryPath);
        return _directoryPath + "/" + _filename;
    }

//-------------------------------------------------------------------------------------
    public static String getLastModifiedDirectoryName(String directoryFilePath) {
        File _file = getLastModifiedDirectory(directoryFilePath);
        if (_file == null) {
            return null;
        }
        return _file.getName();
    }

//-------------------------------------------------------------------------------------
    public static File getLastModifiedDirectory(String directoryFilePath) {
        File directory = new File(directoryFilePath);
        File[] files = directory.listFiles(File::isDirectory);
        long lastModifiedTime = Long.MIN_VALUE;
        File chosenFile = null;

        if (files != null) {
            for (File file : files) {
                if (file.lastModified() > lastModifiedTime) {
                    chosenFile = file;
                    lastModifiedTime = file.lastModified();
                }
            }
        }

        return chosenFile;
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
