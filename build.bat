@echo off


cd \sw\tools\sdk\libs\bin
git tag -d v0.1.7
cd ..\..\..\..\..\..\..\

call "%~dp0\sw\tools\sdk\libs\bin\checkForLatestVersion.bat"

call "%~dp0\sw\tools\sdk\libs\bin\install.bat"

call "%~dp0\sw\tools\sdk\bin\checkForLatestVersion.bat"

call "%~dp0\sw\tools\sdk\bin\install.bat"



