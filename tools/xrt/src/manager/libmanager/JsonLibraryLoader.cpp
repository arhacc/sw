//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <manager/libmanager/JsonLibraryLoader.h>
//#include <targets/fpga/FpgaTarget.h>

//-------------------------------------------------------------------------------------
JsonLibraryLoader::JsonLibraryLoader() {
    std::ifstream _file;
    std::cout << "Loading external lib..." << std::endl;
    _file.open("../lib/lowlevel.json");
    if(!_file) {
        printf("Failed to load lowlevel!\n");
        exit(1);
    }
    try {
        libxpu = json::parse(_file);
        loadSegments();
    } catch(std::exception& _e) {
        std::cout << "Exception:" << _e.what() << '\n';
    }
}

//-------------------------------------------------------------------------------------
FunctionInfo* JsonLibraryLoader::resolve(std::string _name) {
    std::unordered_map<std::string, FunctionInfo>::const_iterator _iterator = functionMap.find(_name);
    if(_iterator == functionMap.end()) {
        return NULL;
    } else {
        return (FunctionInfo*)(&_iterator -> second);
    }
}

//-------------------------------------------------------------------------------------
void JsonLibraryLoader::loadSegments() {
//    std::cout << libxpu.dump() << std::endl;
    for (json::iterator _it = libxpu.begin(); _it != libxpu.end(); ++_it) {
//        std::cout << *_it << '\n';
        std::string _segment = _it.key();
        if(_segment.compare("features") == 0){
            loadFeaturesSegment(_it);
        } else if(_segment.compare("code") == 0){
            loadCodeSegment(_it);
        } else if(_segment.compare("data") == 0){
            loadDataSegment(_it);
        } else if(_segment.compare("crc") == 0){
            loadCrc(_it);
        } else {
            printf("Unknown segment:%s\n", _segment.c_str());
            exit(1);
        }
    }
}

//-------------------------------------------------------------------------------------
void JsonLibraryLoader::loadFeaturesSegment(json::iterator _it) {
}

//-------------------------------------------------------------------------------------
void JsonLibraryLoader::loadCodeSegment(json::iterator _it) {
//    std::cout << _it.value() << '\n';
    for (auto& _code : _it.value().items()){
        std::cout << _code.value() << '\n';
        loadFunction(_code);
    }

//    for (json::iterator _it = libxpu.begin(); _it != libxpu.end(); ++_it) {
//        std::cout << *_it << '\n';
//        loadFunction(_it);
//    }
}

//-------------------------------------------------------------------------------------
void JsonLibraryLoader::loadDataSegment(json::iterator _it) {
}

//-------------------------------------------------------------------------------------
void JsonLibraryLoader::loadCrc(json::iterator _it) {
}

//-------------------------------------------------------------------------------------
void JsonLibraryLoader::loadFunction(auto& _code) {
//        std::ios_base::fmtflags original_flags = out.flags();
//        std::cout << "[" << name << "]: " << DUMP_HEX0x_FORMAT( 16 ) << value << " : "<< DUMP_HEX0x_FORMAT( 16 ) << size << std::endl;
        struct FunctionInfo _functionInfo;
//        std::cout << ":" << _code.value() << "" <<  std::endl;
        std::string _name = _code.value()["name"];
//!!! WE will have dinamic addresses
        _functionInfo.address = -1; // _code.value()["address"];
        int _length = _code.value()["length"];
        _functionInfo.length = _length;
        _functionInfo.name = _name;
        _functionInfo.code = new uint32_t[_length];
        for(int i = 0; i < _length; i++){
            _functionInfo.code[i] = _code.value()["data"][i];
        }

//        for (auto& _data : _it.value().items()){
//            std::cout << _data.value() << '\n';
//            std::string _line = _data.value();
//            std::cout << _line << '\n';
/*            uint32_t _byteCount = stoi(_line.substr(1, 3));
            uint32_t _address = stoi(_line.substr(3, 7));
            std::vector<uint32_t> _code;
            std::string::iterator _it = _line.begin();
            std::advance(_it, 8);
            _code.assign(_it, _line.end());
            struct FunctionCode _functionCode = {_byteCount, _address, _code.data()};
            _functionInfo.codes.push_back(_functionCode);*/
//        }
        std::pair<std::string, FunctionInfo> _functionEntry = {_name, _functionInfo};
        functionMap.insert(_functionEntry);
    }
/*
//-------------------------------------------------------------------------------------
void JsonLibraryLoader::loadFunction1(json::iterator _it) {
//        std::ios_base::fmtflags original_flags = out.flags();
//        std::cout << "[" << name << "]: " << DUMP_HEX0x_FORMAT( 16 ) << value << " : "<< DUMP_HEX0x_FORMAT( 16 ) << size << std::endl;
//        std::cout << "[" << _it.value() << "]" <<  std::endl;

        struct FunctionInfo _functionInfo;
        for (auto& _data : _it.value().items()){
            std::cout << _data.value() << '\n';
            std::string _line = _data.value();
            uint32_t _byteCount = stoi(_line.substr(1, 3));
            uint32_t _address = stoi(_line.substr(3, 7));
            std::vector<uint32_t> _code;
            std::string::iterator _it = _line.begin();
            std::advance(_it, 8);
            _code.assign(_it, _line.end());
            struct FunctionCode _functionCode = {_byteCount, _address, _code.data()};
            _functionInfo.codes.push_back(_functionCode);
        }
        std::pair<std::string, FunctionInfo> _functionEntry = {_it.key(), _functionInfo};
        functionMap.insert(_functionEntry);
    }
*/
//-------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------
