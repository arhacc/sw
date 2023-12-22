//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/Globals.hpp>
#include <common/Utils.hpp>
#include <common/arch/Arch.hpp>
#include <common/cache/Cache.hpp>
#include <manager/Manager.hpp>
#include <sources/Sources.hpp>
#include <targets/Targets.hpp>
#include <transformers/Transformers.hpp>

#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <iostream>
#include <memory>
#include <span>
#include <string>
#include <vector>

#include <fmt/printf.h>
#include <signal.h>

//-------------------------------------------------------------------------------------
void printVersion();
void printUsage();
void signalHandler(int _signal);

//-------------------------------------------------------------------------------------
class Xrt {
    std::unique_ptr<Manager> manager;
    std::unique_ptr<Transformers> transformers;
    std::unique_ptr<Sources> sources;

    std::unique_ptr<Cache> cache;
    std::shared_ptr<Arch> arch = std::make_shared<Arch>();

    //-------------------------------------------------------------------------------------
    static std::string& getNextArgString(
        std::string_view _name,
        std::vector<std::string>::iterator& _i,
        std::vector<std::string>::iterator&& _end) {
        if (++_i == _end || _i->empty() || _i->front() == '-') {
            throw std::runtime_error(fmt::format("Missing argument for {}", _name));
        }
        return *_i;
    }

    //-------------------------------------------------------------------------------------
    static std::filesystem::path getNextArgPath(
        std::string_view _name,
        std::vector<std::string>::iterator& _i,
        std::vector<std::string>::iterator&& _end) {
        std::string& _pathStr = getNextArgString(_name, _i, std::move(_end));

        // absolute path must be computed here because working directory changes later
        // to satisfy xsim constraints
        try {
            return std::filesystem::absolute(_pathStr);
        } catch (std::exception& _e) {
            throw std::runtime_error(
                fmt::format("Error in argument to {}: {}", _name, _e.what()));
        }
    }

    //-------------------------------------------------------------------------------------
    void startModules(
        const std::string& _serverPort,
        const std::vector<std::string>& _batchFiles,
        const std::vector<std::string>& _sourceFiles,
        const std::string& _targetFile,
        bool _enableCmd,
        bool _enableFpgaTarget,
        bool _enableSimTarget,
        bool _enableGoldenModelTarget,
        std::string _archStr) {
        // if fpga target is enabled, it will set the arch
        if (!_enableFpgaTarget) {
            if (_archStr != "") {
                parseArchFile(*arch, _archStr);
            } else {
                parseArchFile(*arch);
            }
        }

        std::unique_ptr<Targets> targets = std::make_unique<Targets>(
            *arch,
            _targetFile,
            _enableFpgaTarget,
            _enableSimTarget,
            _enableGoldenModelTarget);
        manager      = std::make_unique<Manager>(std::move(targets), arch);
        transformers = std::make_unique<Transformers>(manager.get());
        sources      = std::make_unique<Sources>(
            transformers.get(),
            *arch,
            _serverPort,
            _batchFiles,
            _sourceFiles,
            _enableCmd);
    }

  public:
    //-------------------------------------------------------------------------------------
    void main(int _argc, char** _argv) {
        std::cout << XRT_LOGO << "..." << std::endl;
        std::vector<std::string> _args(_argv + 1, _argv + _argc);
        std::string _serverPort;
        std::vector<std::string> _batchFiles;
        std::vector<std::string> _sourceFiles;
        std::string _targetFile;
        std::string _arch;
        bool _enableCmd               = false;
        bool _enableFpgaTarget        = false;
        bool _enableSimTarget         = false;
        bool _enableGoldenModelTarget = false;

        try {
            for (auto i = _args.begin(); i != _args.end(); ++i) {
                if (*i == "-h" || *i == "--help") {
                    printUsage();
                    return;
                }
                if (*i == "-source:net") {
                    _serverPort = getNextArgString("-source:net", i, _args.end());
                } else if (*i == "-source:batch") {
                    _batchFiles.push_back(
                        getNextArgPath("-source:batch", i, _args.end()));
                } else if (*i == "-source:file") {
                    _sourceFiles.push_back(
                        getNextArgPath("-source:file", i, _args.end()));
                } else if (*i == "-source:cmd") {
                    _enableCmd = true;
                } else if (*i == "-target:file") {
                    _targetFile = getNextArgPath("-target:file", i, _args.end());
                } else if (*i == "-target:fpga") {
                    _enableFpgaTarget = true;
                } else if (*i == "-target:sim") {
                    _enableSimTarget = true;
                } else if (*i == "-target:gm") {
                    _enableGoldenModelTarget = true;
                } else if (*i == "-arch") {
                    _arch = getNextArgString("-arch", i, _args.end());
                } else if (*i == "-version") {
                    printVersion();
                    return;
                } else {
                    printUsage();
                    return;
                }
            }

            startModules(
                _serverPort,
                _batchFiles,
                _sourceFiles,
                _targetFile,
                _enableCmd,
                _enableFpgaTarget,
                _enableSimTarget,
                _enableGoldenModelTarget,
                _arch);
        } catch (std::exception& _ex) {
            std::cout << "Start-up failed with exception: " << _ex.what() << std::endl;
        } catch (...) {
            std::cout << "Start-up failed with unknown exception type!" << std::endl;
        }
    }
};

Xrt* xrt;

//-------------------------------------------------------------------------------------
void signalHandler([[maybe_unused]] int _sig) {
    fmt::println("Caught SIGINT, exiting...");
    std::exit(1);
}

//-------------------------------------------------------------------------------------
int main(int _argc, char** _argv) {
    std::signal(SIGINT, signalHandler);

    xrt = new Xrt();
    xrt->main(_argc, _argv);

    delete xrt;

    return 0;
}

//-------------------------------------------------------------------------------------
void printUsage() {
    std::cout << "Syntax: xrt [-arch architecture] [-source:[net/batch/file/cmd] [path]] "
                 "[-target:[fpga/sim/gm/file]]"
              << std::endl;
}

//-------------------------------------------------------------------------------------
void printVersion() {
    std::cout << cVersionString << std::endl;
}

//-------------------------------------------------------------------------------------
