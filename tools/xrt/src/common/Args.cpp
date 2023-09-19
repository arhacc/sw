//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------

#include <common/Args.h>

#include <string_view>
#include <vector>

#include <fmt/format.h>

//-------------------------------------------------------------------------------------

// TODO: Replace *everything* with std::string_view

static std::string getNextArgString(
    std::string_view _name,
    std::span<std::string_view>::iterator& _i,
    std::span<std::string_view>::iterator&& _end);

//-------------------------------------------------------------------------------------
Args parseArgs(int argc, const char* const* argv) {
    std::vector<std::string_view> _args;

    for (int i = 1; i < argc; i++) {
        _args.push_back(argv[i]);
    }

    return parseArgs(_args);
}
//-------------------------------------------------------------------------------------
Args parseArgs(std::span<std::string_view> _args) {
    Args _parsedArgs;
    for (auto i = _args.begin(); i != _args.end(); ++i) {
        if (*i == "-h" || *i == "--help") {
            throw std::runtime_error("Unknown argument");
        }
        if (*i == "-source:net") {
            _parsedArgs.enableNetSource = true;
            _parsedArgs.serverPort      = getNextArgString("-source:net", i, _args.end());
        } else if (*i == "-source:batch") {
            _parsedArgs.enableBatchSource = true;
            _parsedArgs.batchFiles.push_back(
                getNextArgString("-source:batch", i, _args.end()));
        } else if (*i == "-source:file") {
            _parsedArgs.enableFileSource = true;
            _parsedArgs.sourceFiles.push_back(
                getNextArgString("-source:file", i, _args.end()));
        } else if (*i == "-source:cmd") {
            _parsedArgs.enableCmdSource = true;
        } else if (*i == "-target:file") {
            _parsedArgs.enableFileTarget = true;
            _parsedArgs.fileTargetPath = getNextArgString("-target:file", i, _args.end());
        } else if (*i == "-target:fpga") {
            _parsedArgs.enableFpgaTarget = true;
        } else if (*i == "-target:sim") {
            _parsedArgs.enableSimTarget = true;
        } else if (*i == "-target:gm") {
            _parsedArgs.enableGoldenModelTarget = true;
        } else if (*i == "-arch") {
            _parsedArgs.archStr = getNextArgString("-arch", i, _args.end());
        } else {
            throw std::runtime_error("Unknown argument");
        }
    }

    return _parsedArgs;
}

static std::string getNextArgString(
    std::string_view _name,
    std::span<std::string_view>::iterator& _i,
    std::span<std::string_view>::iterator&& _end) {
    if (++_i == _end || _i->empty() || _i->front() == '-') {
        throw std::runtime_error(fmt::format("Missing argument for {}", _name));
    }
    return std::string(*_i);
}

//-------------------------------------------------------------------------------------
