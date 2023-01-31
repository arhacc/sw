//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <transformers/onnx/XpuL3Library.h>
//#include <targets/fpga/FpgaTarget.h>

//-------------------------------------------------------------------------------------
XpuL3Library::XpuL3Library(DirectTransformer* _directTransformer) {
    directTransformer = _directTransformer;
    std::ifstream _file;
    std::cout << "Loading lib/libxpu.json ..." << std::endl;
    _file.open("../lib/libxpu.json");
    if(!_file) {
        printf("Failed to load libxpu!\n");
        exit(1);
    }
    try{
        libxpu = json::parse(_file);
        loadSegments();
    }catch(std::exception& _e){
        std::cout << "Exception:" << _e.what() << '\n';
    }
}

//-------------------------------------------------------------------------------------
void XpuL3Library::loadSegments() {
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
void XpuL3Library::loadFeaturesSegment(json::iterator _it) {
}

//-------------------------------------------------------------------------------------
void XpuL3Library::loadCodeSegment(json::iterator _it) {
//    std::cout << _it.value() << '\n';
    for (auto& _code : _it.value().items()){
//        std::cout << _item.value() << '\n';
        loadFunction(_code);
    }

//    for (json::iterator _it = libxpu.begin(); _it != libxpu.end(); ++_it) {
//        std::cout << *_it << '\n';
//        loadFunction(_it);
//    }
}

//-------------------------------------------------------------------------------------
void XpuL3Library::loadDataSegment(json::iterator _it) {
}

//-------------------------------------------------------------------------------------
void XpuL3Library::loadCrc(json::iterator _it) {
}

//-------------------------------------------------------------------------------------
void XpuL3Library::loadFunction(auto& _code) {
//        std::ios_base::fmtflags original_flags = out.flags();
//        std::cout << "[" << name << "]: " << DUMP_HEX0x_FORMAT( 16 ) << value << " : "<< DUMP_HEX0x_FORMAT( 16 ) << size << std::endl;
        struct FunctionInfo _functionInfo;
//        std::cout << ":" << _code.value() << "" <<  std::endl;
        std::string _name = _code.value()["name"];
        _functionInfo.address = _code.value()["address"];
        int _length = _code.value()["length"];
        _functionInfo.length = _length;
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
void XpuL3Library::loadFunction1(json::iterator _it) {
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
FunctionInfo* XpuL3Library::getFunction(std::string _name) {
    std::unordered_map<std::string, FunctionInfo>::const_iterator _iterator = functionMap.find(_name);
    if(_iterator == functionMap.end()){
        return NULL;
    } else {
        return (FunctionInfo*)(&_iterator -> second);
    }
}


//-------------------------------------------------------------------------------------
void XpuL3Library::writeFunction(FunctionInfo* _functionInfo) {
    directTransformer->writeCode(_functionInfo->address, _functionInfo->code, _functionInfo->length);

/*    for(int i = 0; i < _functionInfo->length; i++){
        FunctionCode _functionCode = _functionInfo->codes.at(i);
        xpuL4Driver->loadCode(_functionCode.address, _functionCode.code, _functionCode.byteCount);
    }*/

//    std::cout << "Loading " << _name << std::endl;
/*        uint32_t _size = _iterator -> second.byteCount;
        std::vector<uint32_t> _data = _iterator -> second.data;
        for(int i = 0; i < _size; i++){
            uint32_t _dataWord = _data[i];
            xpuL4Driver -> AXI_LITE_write(0, _dataWord);
        }        */
}

//-------------------------------------------------------------------------------------
void XpuL3Library::writeData(void* _address, uint32_t _length){

}

//-------------------------------------------------------------------------------------
