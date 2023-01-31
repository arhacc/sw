//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
/*
https://en.wikipedia.org/wiki/Intel_HEX
*/
//-------------------------------------------------------------------------------------
#pragma once
#include <map>
#include <unordered_map>
#include <iostream>
#include <cassert>
#include <cstdio>
#include <cstdint>
//#include <targets/fpga/FpgaTarget.h>
#include <algorithm>
#include <string>
#include <ostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

//struct FunctionCode {
//};

struct FunctionInfo {
//    std::vector<FunctionCode>    codes;
    uint32_t     length;
    uint32_t     address;
    uint32_t*    code;
};


//-------------------------------------------------------------------------------------
class XpuL3Library {

public:
	XpuL3Library(DirectTransformer* _directTransformer);
 	~XpuL3Library();

	void loadSegments();
	void loadFeaturesSegment(json::iterator _it);
	void loadCodeSegment(json::iterator _it);
	void loadDataSegment(json::iterator _it);
	void loadCrc(json::iterator _it);

    void loadFunction(auto& _code);
    FunctionInfo* getFunction(std::string _name);

	void writeFunction(FunctionInfo* _functionInfo);
	void writeData(void* _address, uint32_t _length);

private:
    json libxpu;
	std::unordered_map<std::string, FunctionInfo> functionMap;
	DirectTransformer* directTransformer;

};
//-------------------------------------------------------------------------------------






