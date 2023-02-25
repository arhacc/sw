//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.project;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.nio.file.*;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;

import org.apache.commons.configuration2.*;
import org.apache.commons.configuration2.builder.*;
import org.apache.commons.configuration2.builder.fluent.*;
import org.apache.commons.configuration2.ex.*;
import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.fileformats.asm.*;
//import xpu.sw.tools.sdk.common.fileformats.cl.*;

//-------------------------------------------------------------------------------------
public class Project {
    private Context context;
    private Logger log;
    private String path;
    private String name;


    private String pathToConfigFile;
    private File root;

    private FileBasedConfigurationBuilder<FileBasedConfiguration> builder;
    private Configuration prjConfig;

    private String message;

//-------------------------------------------------------------------------------------
    public Project(Context _context) {
        this(_context, null, null);
    }

//-------------------------------------------------------------------------------------
    public Project(Context _context, String _path, String _name) {
        context = _context;
        log = _context.getLog();
        if((_path != null) && (!_path.endsWith(File.separator))){
            _path = _path + File.separator;
        }
        path = _path + _name;
        name = _name;
        pathToConfigFile = path + File.separator + _name + ".xpuprj";
        root = new File(path);
    }

//-------------------------------------------------------------------------------------
    public Project(Context _context, String _pathAndName) {
        context = _context;
        log = _context.getLog();
        Path _path = Paths.get(_pathAndName);
        Path _parentPath = _path.getParent();
        if(_parentPath != null){
            path = _parentPath.toString();
        } else {
            path = "";
        }
//        path = _path.getParent().toString();
        name = _path.getFileName().toString().split("\\.")[0];
        pathToConfigFile = _pathAndName;
        root = new File(path);
        loadConfig();
    }

//-------------------------------------------------------------------------------------
    public boolean newProject() {
        if (! root.exists()){
            root.mkdir();
        }
//        String _newProjectFile = path + "/" + name + ".prj";
        File _file = new File(pathToConfigFile);
        try {
            log.debug("Creating " + pathToConfigFile + " ...");
            createBuilderConfig();
            loadConfig();

            //sdkConfig.setProperty("creation_date", System.currentTimeMillis());
            Date date = Calendar.getInstance().getTime();
            DateFormat dateFormat = new SimpleDateFormat("yyyy-mm-dd hh:mm:ss");
            String strDate = dateFormat.format(date);

            //prjConfig.setProperty("creation_date", System.currentTimeMillis());
            prjConfig.setProperty("creation_date", strDate);
            
            String _filename = new AsmFile(log, path, name).createTemplateFile();
            if(_filename == null){
                log.error("Cannot create asm file in directory: " + path);
            } else {
                prjConfig.addProperty("files", _filename);
            }
            saveConfig();
        } catch (Exception _e){
//            _e.printStackTrace();
            message = "Error creating: " + pathToConfigFile+ ":\n" + _e.getMessage();
            log.debug(message);
            return false;
        }
        log.debug("Success creating new project: " + path);
        return true;
    }

//-------------------------------------------------------------------------------------
    public static Project loadFrom(Context _context, String _pathAndName) {
        Project _project = new Project(_context, _pathAndName);
        _project.createBuilderConfig();
        _project.loadConfig();
        return _project;            
    }

//-------------------------------------------------------------------------------------
    public boolean isRoot(){
        return (path == null);
    }

//-------------------------------------------------------------------------------------
    public boolean isValid(){
        return root.exists();
    }

//-------------------------------------------------------------------------------------
    public File getRootFile(){
        return root;
    }

//-------------------------------------------------------------------------------------
    public String getPathToConfigFile(){
        return pathToConfigFile;
    }

//-------------------------------------------------------------------------------------
    public String getMessage(){
        return message;
    }

//-------------------------------------------------------------------------------------
    public Configuration getConfiguration(){
        return prjConfig;
    }

//-------------------------------------------------------------------------------------
    public void addFile(File _file){
        try {
            File _dest = new File(root, _file.getName());
            Files.copy(_file.toPath(), _dest.toPath());
//            prjConfig.addProperty("files", _dest.getAbsolutePath());
            Path _pathBase = Paths.get(root.getPath());
            Path _pathFile = Paths.get(_dest.getAbsolutePath());
            String _relativePath = _pathBase.relativize(_pathFile).toString();
            prjConfig.addProperty("files", _relativePath);
            saveConfig();
        } catch(IOException _e){
            log.error("Cannot add file: " + _file.getName());
        }
    }

//-------------------------------------------------------------------------------------
//https://dzone.com/articles/managing-configurations-apache
    private void createBuilderConfig(){
        log.debug("Loading " + pathToConfigFile + "...");
        Parameters params = new Parameters();
        // Read data from this file
        File propertiesFile = new File(pathToConfigFile);

        builder = new FileBasedConfigurationBuilder<FileBasedConfiguration>(PropertiesConfiguration.class)
                .configure(params.fileBased()
                .setFile(propertiesFile));
        
        builder.setAutoSave(true);
/*        File file = new File(pathToConfigFile);
        try{
            FileWriter fw = new FileWriter(file.getAbsoluteFile());
            BufferedWriter bw = new BufferedWriter(fw);
//            bw.write(value);
            bw.close();
        }
        catch (IOException e){
            e.printStackTrace();
//            System.exit(-1);
        }*/
    }

//-------------------------------------------------------------------------------------
//https://dzone.com/articles/managing-configurations-apache
    private void loadConfig(){
        try{
            if(builder == null){
                createBuilderConfig();
            }
            prjConfig = builder.getConfiguration();
            // sdkConfig contains all properties read from the file
        } catch(ConfigurationException _cex) {
            log.debug("Cannot load prjConfig file: " + _cex.getMessage());
            // loading of the configuration file failed
//            saveConfig();
        }
        if(prjConfig == null){
            log.debug("Create new prjConfig for: " + pathToConfigFile + " ...");
            prjConfig = new PropertiesConfiguration();
        }
    }

//-------------------------------------------------------------------------------------
    private void saveConfig(){
        try{
            builder.save();
            // prjConfig contains all properties read from the file
        } catch(ConfigurationException _cex) {
            log.debug("Cannot save prjConfig file: " + _cex.getMessage());
            // loading of the configuration file failed
        }
    }

//-------------------------------------------------------------------------------------
    public String toString(){
        return name;
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
