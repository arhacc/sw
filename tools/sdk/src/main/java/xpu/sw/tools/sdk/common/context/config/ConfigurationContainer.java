//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.context.config;
//-------------------------------------------------------------------------------------

import java.io.File;
import java.io.FileFilter;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

import org.apache.commons.configuration2.Configuration;
import org.apache.commons.configuration2.FileBasedConfiguration;
import org.apache.commons.configuration2.PropertiesConfiguration;
import org.apache.commons.configuration2.builder.FileBasedConfigurationBuilder;
import org.apache.commons.configuration2.builder.fluent.Parameters;
import org.apache.commons.configuration2.ex.ConfigurationException;
import org.apache.commons.io.FilenameUtils;
import org.apache.logging.log4j.Logger;

//-------------------------------------------------------------------------------------
public class ConfigurationContainer implements FileFilter {

    private org.apache.logging.log4j.Logger log;
    private String[] extensions;
    private boolean autoSave;

    private java.util.List<FileBasedConfigurationBuilder<FileBasedConfiguration>> builders;
    private Map<String, Configuration> configs;

//-------------------------------------------------------------------------------------
    public ConfigurationContainer(Logger _log, String _path) {
        this(_log, _path, "*.config, *.conf, *.cfg", true);
    }

//-------------------------------------------------------------------------------------
    public ConfigurationContainer(Logger _log, String _path, boolean _autoSave) {
        this(_log, _path, "*.config, *.conf, *.cfg", _autoSave);
    }

//-------------------------------------------------------------------------------------
    public ConfigurationContainer(Logger _log, String _path, String _extensions, boolean _autoSave) {
        log = _log;
        extensions = _extensions.split(",");
        for (int i = 0; i < extensions.length; i++) {
            extensions[i] = extensions[i].replace("*", "").trim();
        }
        autoSave = _autoSave;

        builders = new ArrayList<>();
        configs = new HashMap<>();
//        config = new CombinedConfiguration(new OverrideCombiner());
        load(_path);
    }

//-------------------------------------------------------------------------------------
    public Configuration getConfiguration(String _key) {
        return configs.get(_key);
    }

//-------------------------------------------------------------------------------------
    public Map<String, Configuration> getConfigurations() {
        return configs;
    }

//-------------------------------------------------------------------------------------
    private void load(String _path) {
        Path _p = Paths.get(_path);
        File _file = _p.toFile();
        load(_file);
    }

//-------------------------------------------------------------------------------------
    public void load(File _file) {
        if (_file.isDirectory()) {
            File[] _files = _file.listFiles(this);
            for (int i = 0; i < _files.length; i++) {
                load(_files[i]);
            }
        } else {
            loadFile(_file);
        }
//        return config;
    }

//-------------------------------------------------------------------------------------
    public boolean accept(File _file) {
//        log.info("accept...0 [" + _file.toString() +"]...");
        if (_file.isDirectory()) {
//            log.info("accept...1 [" + _file.toString() +"]...");
            return true;
        }
        String _name = _file.getName();
        for (int i = 0; i < extensions.length; i++) {
            if (_name.endsWith(extensions[i])) {
//        log.info("accept...2 [" + _file.toString() +"]...");
                return true;
            }
        }
//        log.info("accept...3 [" + _file.toString() +"]...");
        return false;
    }

//https://dzone.com/articles/managing-configs-apache
//-------------------------------------------------------------------------------------
    public void loadFile(File _file) {
        Parameters params = new Parameters();

        // Read data from this file
        FileBasedConfigurationBuilder<FileBasedConfiguration> _builder
                = new FileBasedConfigurationBuilder<FileBasedConfiguration>(PropertiesConfiguration.class)
                        .configure(params.fileBased()
                                .setFile(_file));
        _builder.setAutoSave(autoSave);
        builders.add(_builder);

        try {
//            config.addConfiguration(_builder.getConfiguration());//this overrides config2
            String _name = FilenameUtils.removeExtension(_file.getName());
            configs.put(_name, _builder.getConfiguration());
//            log.info("Loading [" + _name + "] to configs...");
            // config contains all properties read from the file
        } catch (ConfigurationException _cex) {
            log.error("Cannot load config file[" + _file.getPath() + "]: " + _cex.getMessage());
//            log.error(_cex.getLocalizedMessage());
//            _cex.printStackTrace();
            // loading of the config file failed
//            save();
            System.exit(0);
        } catch (Exception _e) {
            log.error("Cannot load config file[" + _file.getPath() + "]: " + _e.getMessage());
            System.exit(1);
        }
    }

//-------------------------------------------------------------------------------------
    public void save() {
        builders.forEach(_builder -> {
            try {
//                log.debug("Saving >>> " + _builder);
                _builder.save();
            } catch (ConfigurationException _cex) {
                log.error("Cannot save config file: " + _cex.getMessage());
            }
        });
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
