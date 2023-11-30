//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <common/arch/Arch.hpp>
#include <manager/libmanager/LibraryResolver.hpp>
#include <manager/libmanager/lowlevel/LowLevelLibManager.hpp>
#include <manager/libmanager/midlevel/ModManager.hpp>

#include <filesystem>

class LibManager {
    LibraryResolver libraryResolver;

    LowLevelLibManager lowLevelLibManager;
    ModManager modManager;

  public:
    LibManager(const Arch& _arch, MemManager* _memManager, Manager* _manager);
    ~LibManager() = default;

    FunctionInfo resolve(const std::string& _name, LibLevel _level);

    void load(const std::string& _path, LibLevel _level);
    void load(const std::filesystem::path& _path, LibLevel _level);

    // Encapsulates LowLevelLibManager TODO: better solution for this
    inline std::vector<LowLevelFunctionInfo>& stickyFunctionsToLoad() {
        return lowLevelLibManager.stickyFunctionsToLoad();
    }

    inline void
    runMidLevel(const ModFunctionInfo& _function, std::vector<std::any> _args) {
        modManager.run(_function, _args);
    }
};
