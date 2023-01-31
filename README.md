# <p align="center"> XPU
### <p align="center"> The aXelerator Processing Unit: a map-scan-reduce parallel accelerator architecture

[gmstefan description here]


## Requirements:

Make sure you have java 18 or later version installed Update variable JAVA in make/tools.mk to point to your local java installation then you can use below command to generate rtl, ral and cmod

- JDK > 18.0
- gradle > 7.1 : https://gradle.org/

### Linux:

The tools can be installed automatically by running the following command in the XPU directory:
```
  $ cd xpu/
  $ ./install_requirements
```
Set a new enviroment variable and add the current directory to path in ~/.bashrc:
```
  $ export XPU_HOME="$HOME/.xpu"
  $ export PATH=.:$PATH
  $ export JAVA_HOME=$(/usr/libexec/java_home)
  $ export GIT_ROOT="$HOME/projects"
  $ export ONNX_ROOT="$GIT_ROOT/onnx/install"
  $ export BINUTILS_HOME="/opt/homebrew/opt/binutils"
  $ export BINUTILS_LIB="$BINUTILS_HOME/lib"
  $ export BINUTILS_INCLUDE="$BINUTILS_HOME/include"
  $ export FMT_HOME="/opt/homebrew/Cellar/fmt/9.1.0"
  $ export XILINX_HOME="$HOME/apps/Xilinx"
  $ export VITIS_HOME="$XILINX_HOME/Vitis/2022.2"
  
```
Set your github credentials in ~/.gradle/gradle.properties ( Note there are no quotations marks ):
```
  $ github_maven_user=your_github_username
  $ github_maven_password=your_github_password
```
Clone the *common* repository and build it:
```
  $ git clone https://github.com/mcstoian/common.git
  $ cd common/
  $ gradle
```
Then run the *install* and *checkForLatestVersion* scripts:
```
  $ cd xpu/sw/tools/sdk/bin
  $ ./install
  $ ./checkForLatestVersion
```
Open the gui in the *.xpu* directory:
```
  $ cd .xpu/bin
  $ ./gui
```

WINDOWS :
```
1.Open the Start Search, type in “env”, and choose “Edit the system environment variables”:
2.Click the “Environment Variables…” button.
3.Set the environment variables as needed. The New button adds an additional variable. ...
4.Dismiss all of the dialogs by choosing “OK”. Your changes are saved!
```


This is a research project, not an official product yet. Expect bugs and sharp edges. Please help by trying it out, reporting bugs, and letting us know what you think!
