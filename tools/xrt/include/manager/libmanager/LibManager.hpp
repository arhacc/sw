//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <any>
#include <filesystem>
#include <vector>

// forward declarations
class LibraryResolver;
class LowLevelLibManager;
class Manager;
class MemManager;
class ModManager;
struct Arch;
struct FunctionInfo;
struct LowLevelFunctionInfo;
struct ModFunctionInfo;
enum class LibLevel;

class LibManager {
    LibraryResolver* libraryResolver;

    LowLevelLibManager* lowLevelLibManager;
    ModManager* modManager;

  public:
    LibManager(const Arch& _arch, MemManager* _memManager, Manager* _manager);
    ~LibManager();

    FunctionInfo resolve(const std::string& _name, LibLevel _level);

    void load(const std::string& _path, LibLevel _level);
    void load(const std::filesystem::path& _path, LibLevel _level);

    // Encapsulates LowLevelLibManager TODO: better solution for this
    std::vector<std::unique_ptr<LowLevelFunctionInfo>>& stickyFunctionsToLoad();

    void runMidLevel(const ModFunctionInfo& _function, std::vector<std::any> _args);
};
