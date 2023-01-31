//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <Globals.h>
#include <targets/Targets.h>
#include <transformers/Transformers.h>
#include <sources/Sources.h>

//-------------------------------------------------------------------------------------
Targets* targets;
Transformers* transformers;
Sources* sources;

//-------------------------------------------------------------------------------------
void printUsage();
void startModules(std::string _serverPort, std::string _batch, std::vector<std::string> _files, bool _enableCmd, bool _enableFpgaTarget, bool _enableSimTarget, bool _enableGoldenModelTarget);
void signalHandler(int _signal);

//-------------------------------------------------------------------------------------
int main(int _argc, char** _argv) { 
  struct sigaction sigIntHandler;

  sigIntHandler.sa_handler = signalHandler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;

  sigaction(SIGINT, &sigIntHandler, NULL);
//  pause();

  std::cout << XRT_LOGO << "..." << std::endl;
  std::vector<std::string> _args(_argv + 1, _argv + _argc);
  std::string _serverPort     = "";
  std::string _batch          = "";
  std::vector<std::string> _files;
  bool _enableCmd               = 0;
  bool _enableFpgaTarget        = 0;
  bool _enableSimTarget         = 0;
  bool _enableGoldenModelTarget = 0;
  
  for (auto i = _args.begin(); i != _args.end(); ++i) {
      if (*i == "-h" || *i == "--help") {
          printUsage();
          return 0;
      } else if (*i == "-source:net") {
        _serverPort = *++i;
      } else if (*i == "-source:batch") {
        _batch = *++i;
      } else if (*i == "-source:file") {
        _files.push_back(*++i);
      } else if (*i == "-source:cmd") {
        _enableCmd = 1;
      } else if (*i == "-target:fpga") {
        _enableFpgaTarget = 1;
      } else if (*i == "-target:sim") {
        _enableSimTarget = 1;
      } else if (*i == "-target:gm") {
        _enableGoldenModelTarget = 1;
      } else {
          printUsage();
          return 0;
      }
  }

  startModules(_serverPort, _batch, _files, _enableCmd, _enableFpgaTarget, _enableSimTarget, _enableGoldenModelTarget);
  return 0;
}

//-------------------------------------------------------------------------------------
void printUsage() { 
  std::cout << "Syntax: xrt [-source:[net/batch/file/cmd] [path]] [-target:[fpga/sim/gm]]" << std::endl;
}

//-------------------------------------------------------------------------------------
void startModules(std::string _serverPort, std::string _batch, std::vector<std::string> _files, bool _enableCmd, bool _enableFpgaTarget, bool _enableSimTarget, bool _enableGoldenModelTarget){
//    printf("startModules: _enableFpgaTarget=%d, _enableSimTarget=%d\n", _enableFpgaTarget, _enableSimTarget);
    std::cout << "startModules: _enableCmd=" << _enableCmd << ", _files=" << _files.size() << std::endl;
    targets = new Targets(_enableFpgaTarget, _enableSimTarget, _enableGoldenModelTarget);    
    transformers = new Transformers(targets);    
    sources = new Sources(transformers, _serverPort, _batch, _files, _enableCmd);
}

//-------------------------------------------------------------------------------------
void signalHandler(int _signal){
    printf("XRT stopping...\n");
    delete(targets);
    delete(transformers);
    delete(sources);
    exit(1); 
}

//-------------------------------------------------------------------------------------
