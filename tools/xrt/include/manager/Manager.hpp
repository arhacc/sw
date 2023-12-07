//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

// FunctionInfo.hpp included for LibLevel::ANY_LEVEL
#include <manager/libmanager/FunctionInfo.hpp>

#include <cstdint>
#include <filesystem>
#include <memory>
#include <span>
#include <string>
#include <string_view>

// forward declaration
class Driver;
class LibManager;
class LibraryResolver;
class MemManager;
class Targets;
struct Arch;
enum class ArchConstant : unsigned int;

//-------------------------------------------------------------------------------------
class Manager {
    LibManager* libManager;
    MemManager* memManager;
    Driver* driver;

    std::shared_ptr<Arch> arch;
    std::unique_ptr<Targets> targets;

  public:
    Manager(std::unique_ptr<Targets> _targets, std::shared_ptr<Arch> _arch);

    ~Manager();

    void run(std::string_view _name);
    void runLowLevel(std::string_view _name, std::span<const uint32_t> _args = {});
    void run(FunctionInfo _function);
    void runLowLevel(FunctionInfo _function, std::span<const uint32_t> _args = {});

    FunctionInfo lowLevel(std::string_view _name);
    void load(const std::filesystem::path& _path, LibLevel _level = LibLevel::ANY_LEVEL);

    uint32_t readRegister(uint32_t _address);
    void writeRegister(uint32_t _address, uint32_t _value);

    // driver encapsulation

    void runRuntime(uint32_t _address, std::span<const uint32_t> _args);

    // arch encapsulation

    unsigned constant(ArchConstant _constant) const;

    // used in callbacks

    void
    runRuntime(LowLevelFunctionInfo* _function, std::span<const uint32_t> _args = {});

    void readMatrixArray(
        uint32_t _accMemStart,
        uint32_t* _ramMatrix,
        uint32_t _ramTotalLines,
        uint32_t _ramTotalColumns,
        uint32_t _ramStartLine,
        uint32_t _ramStartColumn,
        uint32_t _numLines,
        uint32_t _numColumns,
        bool _accRequireResultReady);

    void writeMatrixArray(
        uint32_t _accMemStart,
        uint32_t* _ramMatrix,
        uint32_t _ramTotalLines,
        uint32_t _ramTotalColumns,
        uint32_t _ramStartLine,
        uint32_t _ramStartColumn,
        uint32_t _numLines,
        uint32_t _numColumns);

    void writeRawInstruction(uint32_t _instruction);
    void writeRawInstructions(std::span<const uint32_t> _instructions);
};
//-------------------------------------------------------------------------------------
