//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/log/Logger.hpp>
#include <manager/libmanager/JsonLibraryLoader.hpp>
#include <manager/libmanager/LowLevelFunctionInfo.hpp>

#include <memory>
#include <stdexcept>

#include <fmt/format.h>

#if 0

//-------------------------------------------------------------------------------------
void JsonLibraryLoader::load(const std::string& _path) {
    std::ifstream _file;
    logInit.print(fmt::format("Loading JSON library {}...\n", _path));
    _file.open(_path);
    if (!_file) {
        throw std::runtime_error("Failed to load JSON library");
    }
    try {
        json _libxpu = json::parse(_file);
        loadSegments(_libxpu);
    } catch (std::exception& _e) {
        std::cout << "Exception:" << _e.what() << '\n';
    }
}

//-------------------------------------------------------------------------------------
LowLevelFunctionInfo* JsonLibraryLoader::resolve(std::string _name) {
    auto _iterator = functionMap.find(_name);
    if (_iterator == functionMap.end()) {
        return nullptr;
    }

    LowLevelFunctionInfo* _functionInfo = _iterator->second.get();

    logWork.print(fmt::format("Found function {} in loaded JSON libraries\n", _name));

    return _functionInfo;
}

//-------------------------------------------------------------------------------------
void JsonLibraryLoader::loadSegments(json _libxpu) {
    //    std::cout << libxpu.dump() << std::endl;
    for (json::iterator _it = _libxpu.begin(); _it != _libxpu.end(); ++_it) {
        //        std::cout << *_it << '\n';
        const std::string& _segment = _it.key();
        if (_segment == "features") {
            loadFeaturesSegment(_it);
        } else if (_segment == "code") {
            loadCodeSegment(_it);
        } else if (_segment == "data") {
            loadDataSegment(_it);
        } else if (_segment == "crc") {
            loadCrc(_it);
        } else {
            throw std::runtime_error("Unknown segment: " + _segment);
        }
    }
}

//-------------------------------------------------------------------------------------
void JsonLibraryLoader::loadFeaturesSegment(const json::iterator& _it) {
    // TODO: JsonLibraryLoader::loadFeaturesSegment
}

//-------------------------------------------------------------------------------------
void JsonLibraryLoader::loadCodeSegment(const json::iterator& _it) {
    //    std::cout << _it.value() << '\n';
    for (auto& _code : _it.value().items()) {
        //        std::cout << _code.value() << '\n';
        loadFunction(_code);
    }

    //    for (json::iterator _it = libxpu.begin(); _it != libxpu.end(); ++_it) {
    //        std::cout << *_it << '\n';
    //        loadFunction(_it);
    //    }
}

//-------------------------------------------------------------------------------------
void JsonLibraryLoader::loadDataSegment(const json::iterator& _it) {
    // TODO: JsonLibraryLoader::loadDataSegment
}

//-------------------------------------------------------------------------------------
void JsonLibraryLoader::loadCrc(const json::iterator& _it) {
    // TODO: JsonLibraryLoader::loadCrc
}

//-------------------------------------------------------------------------------------
void JsonLibraryLoader::loadFunction(auto& _code) {
    LowLevelFunctionInfo _functionInfo;
    std::string _name = _code.value()["name"];

    std::cout << fmt::format("Loading JSON function {}", _name) << std::endl;

    // JSON representation stores one instruction pair (controller + cells) in one int
    // this needs to be decoded in two separate ints for internal representation

    //!!! WE will have dinamic addresses
    int _length           = _code.value()["length"];
    _functionInfo.name    = _name;
    _functionInfo.address = -1; // _code.value()["address"];
    _functionInfo.code.resize(_length * 2);
    for (int i = 0; i < _length; i++) {
        _functionInfo.code[2 * i]     = ((uint64_t) _code.value()["payload"][i]) >> 32;
        _functionInfo.code[2 * i + 1] = _code.value()["payload"][i];
    }

    std::pair<std::string, std::unique_ptr<LowLevelFunctionInfo>> _functionEntry = {
        _name, std::make_unique<LowLevelFunctionInfo>(_functionInfo)};
    functionMap.insert(std::move(_functionEntry));
}

#endif

//-------------------------------------------------------------------------------------
