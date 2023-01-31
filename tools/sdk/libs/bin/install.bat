@echo off

for %%A in ("%~dp0..\build.gradle") Do set BUILD_GRADLE_FILE=%%~fA
echo %BUILD_GRADLE_FILE%
gradle -b %BUILD_GRADLE_FILE% publishToMavenLocal runtime %*%