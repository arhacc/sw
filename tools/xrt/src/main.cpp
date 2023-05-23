//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include "common/arch/Arch.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <common/Globals.h>
#include <common/Utils.h>
#include <common/cache/Cache.h>
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
void startModules(const std::string &_serverPort, const std::vector<std::string> &_batchFiles,
        const std::vector<std::string> &_sourceFiles, const std::vector<std::string> &_targetFiles,
        bool _enableCmd, bool _enableFpgaTarget, bool _enableSimTarget, bool _enableGoldenModelTarget,
        std::string _arch);

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
    std::vector<std::string> _batchFiles;
    std::vector<std::string> _sourceFiles;
    std::vector<std::string> _targetFiles;
    std::string _arch;
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
            _batchFiles.push_back(*++i);
        } else if (*i == "-source:file") {
            _sourceFiles.push_back(*++i);
        } else if (*i == "-source:cmd") {
            _enableCmd = true;
        } else if (*i == "-target:file") {
            _targetFiles.push_back(*++i);
        } else if (*i == "-target:fpga") {
            _enableFpgaTarget = true;
        } else if (*i == "-target:sim") {
            _enableSimTarget = true;
        } else if (*i == "-target:gm") {
            _enableGoldenModelTarget = true;
        } else if (*i == "-arch") {
            _arch = *++i;
        } else {
            printUsage();
            return 0;
        }
    }

    try {
        startModules(_serverPort, _batchFiles, _sourceFiles, _targetFiles, _enableCmd, _enableFpgaTarget, _enableSimTarget,
                _enableGoldenModelTarget, _arch);
    } catch (std::exception &_ex) {
        std::cout << "Start-up failed with exception: " << _ex.what() << std::endl;
    } catch (...) {
        std::cout << "Start-up failed with unknown exception type!" << std::endl;
    }
    return 0;
}

//-------------------------------------------------------------------------------------
void startModules(const std::string &_serverPort, const std::vector<std::string> &_batchFiles,
        const std::vector<std::string> &_sourceFiles, const std::vector<std::string> &_targetFiles,
        bool _enableCmd, bool _enableFpgaTarget, bool _enableSimTarget, bool _enableGoldenModelTarget,
        std::string _archStr) {
    //    printf("startModules: _enableFpgaTarget=%d, _enableSimTarget=%d\n", _enableFpgaTarget, _enableSimTarget);
    std::cout << "startModules: _enableCmd=" << _enableCmd << ", _files=" << _sourceFiles.size() << std::endl;


    Cache *_cache = new Cache;

    const Arch& _arch = (_archStr != "") ? parseArchFile(_archStr) : parseArchFile();

    targets = new Targets(_arch, _targetFiles, _enableFpgaTarget, _enableSimTarget, _enableGoldenModelTarget);
    manager = new Manager(targets, _cache, _arch);
    transformers = new Transformers(manager);
    sources = new Sources(transformers, _cache, _serverPort, _batchFiles, _sourceFiles, _enableCmd);
}

//-------------------------------------------------------------------------------------
void printUsage() {
    std::cout << "Syntax: xrt [-arch architecture] [-source:[net/batch/file/cmd] [path]] [-target:[fpga/sim/gm/file]]" << std::endl;
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
