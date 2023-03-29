//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <string>
#include <vector>
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <common/Globals.h>
#include <common/Utils.h>
#include <targets/Targets.h>
#include <manager/Manager.h>
#include <transformers/Transformers.h>
#include <sources/Sources.h>

//-------------------------------------------------------------------------------------
Targets *targets;
Manager *manager;
Transformers *transformers;
Sources *sources;

//-------------------------------------------------------------------------------------
void startModules(const std::string &_serverPort, const std::string &_batch, const std::vector<std::string> &_files,
        bool _enableCmd, bool _enableFpgaTarget, bool _enableSimTarget, bool _enableGoldenModelTarget);

//-------------------------------------------------------------------------------------
int main(int _argc, char **_argv) {
    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = signalHandler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, nullptr);
    //  pause();

    std::cout << XRT_LOGO << "..." << std::endl;
    std::vector<std::string> _args(_argv + 1, _argv + _argc);
    std::string _serverPort;
    std::string _batch;
    std::vector<std::string> _files;
    bool _enableCmd = false;
    bool _enableFpgaTarget = false;
    bool _enableSimTarget = false;
    bool _enableGoldenModelTarget = false;

    for (auto i = _args.begin(); i != _args.end(); ++i) {
        if (*i == "-h" || *i == "--help") {
            printUsage();
            return 0;
        }
        if (*i == "-source:net") {
            _serverPort = *++i;
        } else if (*i == "-source:batch") {
            _batch = *++i;
        } else if (*i == "-source:file") {
            _files.push_back(*++i);
        } else if (*i == "-source:cmd") {
            _enableCmd = true;
        } else if (*i == "-target:fpga") {
            _enableFpgaTarget = true;
        } else if (*i == "-target:sim") {
            _enableSimTarget = true;
        } else if (*i == "-target:gm") {
            _enableGoldenModelTarget = true;
        } else {
            printUsage();
            return 0;
        }
    }

    try {
        startModules(_serverPort, _batch, _files, _enableCmd, _enableFpgaTarget, _enableSimTarget,
                _enableGoldenModelTarget);
    } catch (std::exception &_ex) {
        std::cout << "Start-up failed with exception: " << _ex.what() << std::endl;
    } catch (...) {
        std::cout << "Start-up failed with unknown exception type!" << std::endl;
    }
    return 0;
}

//-------------------------------------------------------------------------------------
void startModules(const std::string &_serverPort, const std::string &_batch, const std::vector<std::string> &_files,
        bool _enableCmd, bool _enableFpgaTarget, bool _enableSimTarget, bool _enableGoldenModelTarget) {
    //    printf("startModules: _enableFpgaTarget=%d, _enableSimTarget=%d\n", _enableFpgaTarget, _enableSimTarget);
    std::cout << "startModules: _enableCmd=" << _enableCmd << ", _files=" << _files.size() << std::endl;
    targets = new Targets(_enableFpgaTarget, _enableSimTarget, _enableGoldenModelTarget);
    manager = new Manager(targets);
    transformers = new Transformers(manager);
    sources = new Sources(transformers, _serverPort, _batch, _files, _enableCmd);
}

//-------------------------------------------------------------------------------------
void printUsage() {
    std::cout << "Syntax: xrt [-source:[net/batch/file/cmd] [path]] [-target:[fpga/sim/gm]]" << std::endl;
}

//-------------------------------------------------------------------------------------
void signalHandler(int _signal) {
    printf("Stopping XRT with signal %d...\n", _signal);
    delete targets;
    delete manager;
    delete transformers;
    delete sources;
    exit(1);
}

//-------------------------------------------------------------------------------------
