//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once
#include <common/Constants.hpp>

#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

//-------------------------------------------------------------------------------------
enum class FileType {
    Hex,
    Json,
    Obj,
    Onnx,
    C,
    Cpp,
    So,
};

FileType getFileTypeFromPath(std::filesystem::path _path);
std::string getFileNameFromPath(std::filesystem::path _path);

void createDirIfNotExists(const std::filesystem::path& _path);

//-------------------------------------------------------------------------------------
std::vector<std::string> split(std::string_view _value, std::string_view _separator);
bool beginsWith(std::string_view value, std::string_view beginning);
bool endsWith(std::string_view value, std::string_view ending);

//-------------------------------------------------------------------------------------
std::filesystem::path getXpuHome();

enum class ResourceDirectory {
    ArchitectureImplementations,
    LowLevelLibrariesPrefix,
    LowLevelLibrariesPostfix,
    MidLevelLibraries,
    Onnx2JsonTmpDir,
    OnnxOutputCacheTmpDir,
    DesignsDir,
    XSimFS,
};

std::filesystem::path getPath(ResourceDirectory _resourceDirectory);

//-------------------------------------------------------------------------------------
template<class T>
constexpr unsigned numDigits(T number) {
    unsigned digits = 0;

    do {
        number /= 10;
        digits++;
    } while (number != 0);

    return digits;
}
//-------------------------------------------------------------------------------------
