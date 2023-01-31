@echo off

REM for %%A in ("%~dp0..\bin\windows_version") Do set HOME=%%~fA
SET XPU_HOME=%HOME%\.xpu
SET XPU_LIB=%HOME%\.xpu\lib
SET CLASSPATH=%XPU_LIB%\*;%CLASSPATH%
REM SET JAVA_PATH=%HOME%\xpu
REM SET JAVA_ACCESS_OPTIONS=--add-opens java.base/java.nio=ALL-UNNAMED java.base/jdk.internal.misc=ALL-UNNAMED -Dio.netty.tryReflectionSetAccessible=true
SET JAVA_ACCESS_OPTIONS=
REM SET JAVA_PROFILING_OPTIONS="-agentpath:\Applications\JProfiler.app\Contents\Resources\app\bin\macos\libjprofilerti.jnilib=port=8849"
SET JAVA_PROFILING_OPTIONS=
REM echo %JAVA_ACCESS_OPTIONS%
REM echo %CLASSPATH%
java %JAVA_PROFILING_OPTIONS% %JAVA_ACCESS_OPTIONS% -Djava.classpath=. xpu.sw.tools.sdk.Sdk %*%