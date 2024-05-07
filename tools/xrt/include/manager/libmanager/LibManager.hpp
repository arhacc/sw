//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <manager/libmanager/LowLevelFunctionInfo.hpp>
#include <manager/libmanager/HexLibraryLoader.hpp>
#include <manager/libmanager/InternalLibraryLoader.hpp>

#include <filesystem>
#include <memory>
#include <unordered_map>
#include <vector>

// forward declarations
struct Arch;
class MemManager;

class LibManager {
    const Arch& arch;

    std::unique_ptr<InternalLibraryLoader> internalLibraryLoader;
    std::unique_ptr<HexLibraryLoader> hexLibraryLoader;

    std::unordered_map<std::string, std::unique_ptr<LowLevelFunctionInfo>> functionMap;

    // See initLowLevelStdLib
    std::vector<std::filesystem::path> getLowLevelStandardLibrary();

  public:
    LibManager(const Arch& _arch);
    ~LibManager() = default;

    // Must be called separetly from constructor
    // Only used when xrtcore is used without xrt
    // xrt uses the ResourceLoader class which manages this functionallity
    void initLowLevelStdlib();

    LowLevelFunctionInfo& resolve(std::string_view _name) const;

    void load(const std::filesystem::path& _path, std::string_view _name);

    std::vector<std::unique_ptr<LowLevelFunctionInfo>> stickyFunctionsToLoad();
};
