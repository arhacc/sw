//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once
#include <common/Globals.h>

#include <filesystem>
#include <sstream>
#include <string>
#include <vector>

//-------------------------------------------------------------------------------------
void printUsage();
void signalHandler(int _signal);
int getFileTypeFromGeneralPath(const std::string& _path);
int getFileTypeFromPath(const std::string& _path);
std::string getFileStemFromGeneralPath(const std::string& _path);
std::vector<std::string> split(std::string _value, const std::string& _separator);
inline bool endsWith(std::string const& value, std::string const& ending);
std::string basename(const std::string& _path);
std::string getXpuHome();

enum class ResourceDirectory {
    ArchitectureImplementations,
    LowLevelLibrariesPrefix,
    LowLevelLibrariesPostfix,
};

std::filesystem::path getPath(ResourceDirectory _resourceDirectory);

template<class T>
constexpr unsigned int numDigits(T number) {
    int digits = 0;
    if (number < 0)
        digits = 1;
    while (number) {
        number /= 10;
        digits++;
    }
    return digits;
}
//-------------------------------------------------------------------------------------
