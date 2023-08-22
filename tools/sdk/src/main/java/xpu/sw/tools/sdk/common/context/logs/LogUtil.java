//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.context.logs;

//-------------------------------------------------------------------------------------
import java.util.concurrent.*;

import org.apache.logging.log4j.Level;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.apache.logging.log4j.core.LoggerContext;
import org.apache.logging.log4j.core.config.Configuration;
import org.apache.logging.log4j.core.config.LoggerConfig;
import org.apache.logging.log4j.core.layout.PatternLayout;
import org.apache.logging.log4j.core.*;
import org.apache.logging.log4j.core.config.*;
import org.apache.logging.log4j.core.appender.*;
import org.apache.logging.log4j.core.config.builder.api.*;
import org.apache.logging.log4j.core.config.builder.impl.*;
import org.apache.logging.log4j.spi.*;
import org.apache.logging.log4j.core.impl.*;
import org.apache.logging.log4j.core.util.*;

public class LogUtil {

    private static final String pattern = "[%t]: %msg%n%throwable";
    private static final String datalogDir = "/opt/app/logs/";
    private static final String filePattern = "rolling-%d{MM-dd-yy}.log.gz";

    private static LoggerContext ctx;// = (LoggerContext)LogManager.getContext(false);
//    private static final Configuration config = ctx.getConfiguration();
    private static Configuration config;

    /*
    @SuppressWarnings({"rawtypes", "unchecked"})
    private static void createAppenderAndStart(String loggerName, String fileName) {

        Layout layout = PatternLayout.newBuilder().withConfiguration(config)
            .withPattern("[%d{HH:mm:ss:SSS}] [%p] - %l - %m%n").build();
        TimeBasedTriggeringPolicy tbtp = TimeBasedTriggeringPolicy.createPolicy(null, null);
        TriggeringPolicy tp = SizeBasedTriggeringPolicy.createPolicy("10M");
        CompositeTriggeringPolicy policyComposite = CompositeTriggeringPolicy.createPolicy(tbtp, tp);

        String loggerDir = datalogDir + PathResolver.separator;

        String loggerPathPrefix = loggerDir + PathResolver.separator;
        RollingFileAppender.Builder builder = RollingFileAppender.newBuilder().withFilePattern(filePattern)
            .withStrategy(null).withPolicy(policyComposite).withConfiguration(config);
        RollingFileAppender appender = builder.build();
        appender.start();
        config.addAppender(appender);

        AppenderRef ref = AppenderRef.createAppenderRef(loggerName, Level.INFO, null);
        AppenderRef[] refs = new AppenderRef[] {ref};
        LoggerConfig loggerConfig =
            LoggerConfig.createLogger(false, Level.ALL, loggerName, "true", refs, null, config, null);
        loggerConfig.addAppender(appender, null, null);
        config.addLogger(loggerName, loggerConfig);
        ctx.updateLoggers();
     */
 /*
String pattern = "%d [%t] %-5level: %msg%n%throwable";
//    String fileLogName = "logs/rolling.log";
    String filePattern = "rolling-%d{MM-dd-yy}.log.gz";


    DefaultRolloverStrategy strategy = DefaultRolloverStrategy.newBuilder()
            .withMax("7")
            .withMin("1")
            .withFileIndex("max")
            .withConfig(config)
//            .withCompressionLevelStr(Deflater.NO_COMPRESSION + "")
            .build();

    PatternLayout layout = PatternLayout.newBuilder().withConfiguration(config).withPattern(pattern).build();

    TriggeringPolicy tp = SizeBasedTriggeringPolicy.createPolicy("10M");

    RollingFileAppender.Builder builder = RollingFileAppender.newBuilder();
//    builder.setStatusLevel(Level.ALL);
    builder.setName(loggerName);
    builder.withFileName(fileName);
    builder.withFilePattern(filePattern);
    builder.withPolicy(tp);
    builder.withStrategy(strategy);
    builder.setLayout(layout);
    builder.setConfiguration(config);

    RollingFileAppender raf = builder.build();
    raf.start();
    config.addAppender(raf);
    ctx.updateLoggers();

    }*/
 /*
    private static DefaultRolloverStrategy createStrategyByAction(String loggerName, String loggerDir) {

        IfFileName ifFileName = IfFileName.createNameCondition(null, loggerName + "\\.\\d{4}-\\d{2}-\\d{2}.*");
        IfLastModified ifLastModified = IfLastModified.createAgeCondition(Duration.parse("1d"));
        DeleteAction deleteAction = DeleteAction.createDeleteAction(loggerDir, false, 1, false, null,
            new PathCondition[] {ifLastModified, ifFileName}, null, config);
        Action[] actions = new Action[] {deleteAction};

        return DefaultRolloverStrategy.createStrategy("7", "1", null, null, actions, false, config);
    }
     */
 /*
    public static Logger getLogger(Class _clazz) {
        return getLogger(_clazz, null);
    }
     */

    public static Logger getLogger(Class _clazz, String _fileName) {
        if (config == null) {
            configureLog4J(_fileName);
        }
        String _loggerName = _clazz.getName();
        synchronized (config) {
            if (!config.getLoggers().containsKey(_loggerName)) {
                createAppenderAndStart(_loggerName, _fileName);
            }
        }
        return LogManager.getLogger(_loggerName);
    }

    /*
    public static Logger getLogger(Class loggerName, String fileName) {
        if(config == null){
            configureLog4J(loggerName.getName(), fileName);
        }
        return LogManager.getLogger(loggerName);
    }
     */
    public static void removeLogger(Class _clazz) {
        String _loggerName = _clazz.getName();
        synchronized (config) {
            config.getAppender(_loggerName).stop();
            config.getLoggerConfig(_loggerName).removeAppender(_loggerName);
            config.removeLogger(_loggerName);
            ctx.updateLoggers();
        }
    }

    /*
    public static Logger getLogger(String loggerName, String fileName) {

        ConfigurationBuilder<BuiltConfiguration> _builder = ConfigurationFactory.newConfigurationBuilder();

        _builder.setConfigurationName(loggerName);
        _builder.setStatusLevel(Level.ALL);
        _builder.add(_builder.newFilter("ThresholdFilter", Filter.Result.ACCEPT, Filter.Result.NEUTRAL).
            addAttribute("level", Level.DEBUG));
        AppenderComponentBuilder appenderBuilder = _builder.newAppender("Stdout", "CONSOLE").
            addAttribute("target", ConsoleAppender.Target.SYSTEM_OUT);
//        appenderBuilder.add(builder.newLayout("PatternLayout").
//            addAttribute("pattern", "%d [%t] %-5level: %msg%n%throwable"));

        appenderBuilder.add(_builder.newLayout("PatternLayout").
            addAttribute("pattern", "[%t]: %msg%n"));

        appenderBuilder.add(_builder.newFilter("MarkerFilter", Filter.Result.DENY,
            Filter.Result.NEUTRAL).addAttribute("marker", "FLOW"));
        _builder.add(appenderBuilder);

        _builder.add(_builder.newLogger("org.apache.logging.log4j", Level.ALL).
            add(_builder.newAppenderRef("Stdout")).
            addAttribute("additivity", true));
        _builder.add(_builder.newRootLogger(Level.ALL).add(_builder.newAppenderRef("Stdout")));

    config = _builder.build();
}

     */
    public static void createAppenderAndStart(String _loggerName, String _fileName) {
        LoggerContext _ctx = (LoggerContext) LogManager.getContext(false);

//       LoggerConfig loggerConfig = config.getLoggerConfig(LogManager.ROOT_LOGGER_NAME);
        LoggerConfig loggerConfig = config.getLoggerConfig(_loggerName);
        FileAppender fileAppender = FileAppender.newBuilder()
/*                .withName(_loggerName)
                .withLayout(
                        PatternLayout.newBuilder()
                                //           .withPattern("[%d][%-5.-5p][%-14.-14c{1}:%4L] %-22.-22M - %m%n")
                                .withPattern("[%-5.-5c{1}]: %m%n")
                                .build())*/
                .withFileName(_fileName)
                .build();
        fileAppender.start();
        config.addAppender(fileAppender);
        loggerConfig.addAppender(fileAppender, Level.DEBUG, null);
        _ctx.updateLoggers();
    }

    public static void configureLog4J(String _fileName) {
        ctx = (LoggerContext) LogManager.getContext(true);
//        config = ctx.getConfiguration();

        ConfigurationBuilder<BuiltConfiguration> builder = ConfigurationBuilderFactory.newConfigurationBuilder();
        builder.setConfigurationName("-pp");
        builder.setStatusLevel(Level.ERROR);
        builder.add(builder.newFilter("ThresholdFilter", Filter.Result.ACCEPT, Filter.Result.NEUTRAL).
                addAttribute("level", Level.DEBUG));
        AppenderComponentBuilder appenderBuilder = builder.newAppender("Stdout", "CONSOLE").
                addAttribute("target", ConsoleAppender.Target.SYSTEM_OUT);
//        appenderBuilder.add(builder.newLayout("PatternLayout").
//            addAttribute("pattern", "%d [%t] %-5level: %msg%n%throwable"));

        appenderBuilder.add(builder.newLayout("PatternLayout").
                addAttribute("pattern", "[%t]: %msg%n"));

        appenderBuilder.add(builder.newFilter("MarkerFilter", Filter.Result.DENY,
                Filter.Result.NEUTRAL).addAttribute("marker", "FLOW"));
        builder.add(appenderBuilder);
        builder.add(builder.newLogger("org.apache.logging.log4j", Level.DEBUG).
                add(builder.newAppenderRef("Stdout")).
                addAttribute("additivity", false));
        builder.add(builder.newRootLogger(Level.ERROR).add(builder.newAppenderRef("Stdout")));
        builder.setShutdownTimeout(20, TimeUnit.SECONDS);
        builder.setShutdownHook("disable");
        config = builder.build();
        Configurator.initialize(config);


        /*
   LoggerConfig loggerConfig = config.getLoggerConfig(LogManager.ROOT_LOGGER_NAME);
   FileAppender fileAppender =
     FileAppender.newBuilder()
       .withName("file")
       .withLayout(
         PatternLayout.newBuilder()
//           .withPattern("[%d][%-5.-5p][%-14.-14c{1}:%4L] %-22.-22M - %m%n")
           .withPattern("[%-5.-5c{1}]: %m%n")
           .build())
       .withFileName(_fileName)
       .build();
    fileAppender.start();
   config.addAppender(fileAppender);
   loggerConfig.addAppender(fileAppender, Level.INFO, null);
   ctx.updateLoggers();*/
    }

    private LogUtil() {
    }

    public static void disableShutDown() {
        final LoggerContextFactory factory = LogManager.getFactory();

        if (factory instanceof Log4jContextFactory) {
//            System.out.println("Disable LOG shutdown hook...");
            Log4jContextFactory contextFactory = (Log4jContextFactory) factory;

            ((DefaultShutdownCallbackRegistry) contextFactory.getShutdownCallbackRegistry()).stop();
        }
    }

    public static void shutDown() {
//        Configurator.shutdown((LoggerContext)LogManager.getContext());
/*        if(ctx != null){
            Configurator.shutdown(ctx);
        } else {
            LoggerContext _ctx = (LoggerContext)LogManager.getContext(true);
            Configurator.shutdown(_ctx);
        }*/
        LogManager.shutdown();
    }

}
