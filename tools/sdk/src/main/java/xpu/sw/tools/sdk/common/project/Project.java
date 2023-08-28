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
import xpu.sw.tools.sdk.common.io.PathResolver;
import xpu.sw.tools.sdk.common.fileformats.asm.*;
//import xpu.sw.tools.sdk.common.fileformats.cl.*;

//-------------------------------------------------------------------------------------
public class Project {
    private Context context;
    private Logger log;
    private String name;
    private String architectureId;


    private String pathToConfigFile;
    private File rootFile;
    private String rootPath;

    private FileBasedConfigurationBuilder<FileBasedConfiguration> builder;
    private Configuration prjConfig;

    private String message;

//-------------------------------------------------------------------------------------
    public Project(Context _context) {
        this(_context, null, null, null);
    }

//-------------------------------------------------------------------------------------
    public Project(Context _context, String _path, String _name, String _architectureId) {
        context = _context;
        log = _context.getLog();
        if((_path != null) && (!_path.endsWith(PathResolver.separator))){
            _path = _path + PathResolver.separator;
        }
        rootPath = _path + _name;
        name = _name;
        pathToConfigFile = rootPath + PathResolver.separator + _name + ".xpuprj";
        rootFile = new File(rootPath);
        architectureId = _architectureId;
    }

//-------------------------------------------------------------------------------------
    public Project(Context _context, String _pathAndName) {
        context = _context;
        log = _context.getLog();
        Path _path = Paths.get(_pathAndName);
        Path _parentPath = _path.getParent();
        if(_parentPath != null){
            rootPath = _parentPath.toString();
        } else {
            rootPath = "";
        }
//        rootPath = _path.getParent().toString();
        name = _path.getFileName().toString().split("\\.")[0];
        pathToConfigFile = _pathAndName;
        rootFile = new File(rootPath);
        loadConfig();
    }

//-------------------------------------------------------------------------------------
    public String getName() {
        return name;
    }

//-------------------------------------------------------------------------------------
    public boolean newProject() {
        if (! rootFile.exists()){
            rootFile.mkdir();
        }
//        String _newProjectFile = rootPath + "/" + name + ".prj";
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
            
            String _filename = new AsmFile(log, rootPath, name).createTemplateFile();
            if(_filename == null){
                log.error("Cannot create asm file in directory: " + rootPath);
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
        log.debug("Success creating new project: " + rootPath);
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
    public boolean equals(Project _otherProject){
        return (rootPath != null) && rootPath.equals(_otherProject.rootPath);
    }

//-------------------------------------------------------------------------------------
    public int hashCode(){
        return pathToConfigFile.hashCode();
    }

//-------------------------------------------------------------------------------------
    public boolean isRoot(){
        return (rootPath == null);
    }

//-------------------------------------------------------------------------------------
    public boolean isValid(){
        return rootFile.exists();
    }

//-------------------------------------------------------------------------------------
    public File getRootFile(){
        return rootFile;
    }

//-------------------------------------------------------------------------------------
    public String getRootPath(){
        return rootPath;
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
            File _dest = new File(rootFile, _file.getName());
            Files.copy(_file.toPath(), _dest.toPath());
//            prjConfig.addProperty("files", _dest.getAbsolutePath());
            Path _pathBase = Paths.get(rootPath);
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
        architectureId = prjConfig.getString("architectureId", "noarch");
    }

//-------------------------------------------------------------------------------------
    public String relativizePath(String _path){
        Path _pathBase = Paths.get(rootPath);
        Path _pathPath = Paths.get(_path);
        String _relativePath = _pathBase.relativize(_pathPath).toString();
        return _relativePath;
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
    public String getArchitectureId(){
        return architectureId;
    }

//-------------------------------------------------------------------------------------
    public String toString(){
        return name;
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
