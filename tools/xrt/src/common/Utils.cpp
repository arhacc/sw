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
#include <sstream>
#include <fstream>
#include <filesystem>
#include <common/Globals.h>
#include <targets/Targets.h>
#include <transformers/Transformers.h>
#include <sources/Sources.h>

//-------------------------------------------------------------------------------------
int getFileTypeFromGeneralPath(std::string _path) {
	std::vector<std::string> _dirs = split(_path, ".0x");
//	int _npos = find_last_of(".0x");
	switch(_dirs.size()){		
		case 1: {
			return getFileTypeFromPath(_path);
		}
		case 2: {
			return getFileTypeFromPath(_dirs.at(1));
		}
		default: {
  			std::cout << "Unrecognizable path: " << _path << std::endl;
  			exit(1);
			return -1;
		}
	}
}

//-------------------------------------------------------------------------------------
int getFileTypeFromPath(std::string _path) {
	if(endsWith(_path, ".hex")) {
		return XPU_FILE_HEX;
	} else if(endsWith(_path, ".json")) {
		return XPU_FILE_JSON;
	} else if(endsWith(_path, ".obj")) {
		return XPU_FILE_OBJ;
	} else if(endsWith(_path, ".onnx")) {
		return XPU_FILE_ONNX;
	} else {
		std::cout << "Unrecognizable extention: " << _path << std::endl;
		exit(1);
		return -1;
	}
}

//-------------------------------------------------------------------------------------
std::vector<std::string> split(std::string _value, std::string _separator){

}

//-------------------------------------------------------------------------------------
inline bool endsWith(std::string const & value, std::string const & ending) {
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

//-------------------------------------------------------------------------------------
