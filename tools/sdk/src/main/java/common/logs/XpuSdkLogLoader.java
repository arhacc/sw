//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.logs;

//-------------------------------------------------------------------------------------
import java.net.*;
import java.nio.charset.*;

import org.apache.logging.log4j.*;
import org.apache.logging.log4j.core.*;
import org.apache.logging.log4j.core.config.*;
import org.apache.logging.log4j.core.appender.*;
import org.apache.logging.log4j.core.layout.*;
import org.apache.logging.log4j.core.config.builder.api.*;
import org.apache.logging.log4j.core.config.builder.impl.*;

//-------------------------------------------------------------------------------------
public class XpuSdkLogLoader {
    private Configuration configuration;

//-------------------------------------------------------------------------------------
    public XpuSdkLogLoader(){
        init();
    }

//-------------------------------------------------------------------------------------
    public void init(){
        configuration = new XpuSdkConfigurationFactory().getConfiguration(null, "-pp", null);
        Configurator.initialize(configuration);
    }

//-------------------------------------------------------------------------------------
    public Configuration getConfiguration(){
        return configuration;
    }

//-------------------------------------------------------------------------------------

}
//-------------------------------------------------------------------------------------
