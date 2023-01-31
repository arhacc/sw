@echo off

for %%A in ("%~dp0..\build.gradle") Do set BUILD_GRADLE_FILE=%%~fA
gradle -b %BUILD_GRADLE_FILE% publish %*%
