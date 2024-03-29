//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

// FunctionInfo.hpp included for LibLevel::ANY_LEVEL
#include <manager/driver/Driver.hpp>
#include <manager/libmanager/FunctionInfo.hpp>

#include <cstdint>
#include <filesystem>
#include <initializer_list>
#include <memory>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "manager/memmanager/UserBreakpoint.hpp"

// forward declaration
class Driver;
class LibManager;
class LibraryResolver;
class MemManager;
class Targets;
class MatrixView;
struct Arch;
class Future;
enum class ArchConstant : unsigned int;
struct LowLevelFunctionInfo;

//-------------------------------------------------------------------------------------
class Manager {
    Driver driver;
    LibManager* libManager;
    MemManager* memManager;

    std::shared_ptr<Arch> arch;
    std::unique_ptr<Targets> targets;

    std::vector<UserBreakpoint*> activeUserBreakpoints;
    std::vector<UserBreakpoint*> allUserBreakpoints;

    std::shared_ptr<Future> loadLowLevelFunctionAsync(LowLevelFunctionInfo&, bool sticky = false);
    void loadUserBreakpoints(std::span<UserBreakpoint> _userBreakpoints, uint32_t _functionAddress);

    Breakpoint makeHWBreakpoint(const UserBreakpoint&, uint32_t _functionAddress);

  public:
    Manager(std::unique_ptr<Targets> _targets, std::shared_ptr<Arch> _arch);

    ~Manager();

    void runLowLevel(std::string_view _name, std::span<const uint32_t> _args = {});
    void runLowLevel(std::string_view _name, std::vector<uint32_t>&& _args);
    void runLowLevel(FunctionInfo _function, std::span<const uint32_t> _args = {});
    void runLowLevel(FunctionInfo _name, std::vector<uint32_t>&& _args);

    std::shared_ptr<Future> runLowLevelAsync(std::string_view _name, std::span<const uint32_t> _args = {});
    std::shared_ptr<Future> runLowLevelAsync(std::string_view _name, std::vector<uint32_t>&& _args);
    std::shared_ptr<Future> runLowLevelAsync(FunctionInfo _function, std::span<const uint32_t> _args = {});
    std::shared_ptr<Future> runLowLevelAsync(FunctionInfo _name, std::vector<uint32_t>&& _args);

    inline void runLowLevel(FunctionInfo _function, std::initializer_list<uint32_t> _args) {
        std::vector<uint32_t> _argv(_args);
        runLowLevel(_function, _argv);
    }

    FunctionInfo lowLevel(std::string_view _name);
    void load(const std::filesystem::path& _path, LibLevel _level = LibLevel::ANY_LEVEL);

    void process(std::shared_ptr<Future> _future);

    void runClockCycle();
    void runClockCycles(unsigned);

    // arch encapsulation

    unsigned constant(ArchConstant _constant) const;

    // used in callbacks

    void runRuntime(LowLevelFunctionInfo* _function, std::span<const uint32_t> _args = {});
    std::shared_ptr<Future> runRuntimeAsync(LowLevelFunctionInfo* _function, std::span<const uint32_t> _args = {});

    uint32_t readRegister(uint32_t _address);
    void writeRegister(uint32_t _address, uint32_t _value);
    void readMatrixArray(uint32_t _accMemStart, std::shared_ptr<MatrixView> _matrixView, bool _accRequireResultReady);
    void writeMatrixArray(uint32_t _accMemStart, std::shared_ptr<const MatrixView> _matrixView);
    void readMatrixArray(uint32_t _accMemStart, MatrixView&& _matrixView, bool _accRequireResultReady);
    void writeMatrixArray(uint32_t _accMemStart, MatrixView&& _matrixView);
    void writeRawInstruction(uint32_t _instruction);
    void writeRawInstructions(std::span<const uint32_t> _instructions);

    std::shared_ptr<Future> readRegisterAsync(uint32_t _address, uint32_t* _valueLocation);
    std::shared_ptr<Future> writeRegisterAsync(uint32_t _address, uint32_t _value);
    std::shared_ptr<Future>
    readMatrixArrayAsync(uint32_t _accMemStart, std::shared_ptr<MatrixView> _matrixView, bool _accRequireResultReady);
    std::shared_ptr<Future> writeMatrixArrayAsync(uint32_t _accMemStart, std::shared_ptr<const MatrixView> _matrixView);
    std::shared_ptr<Future>
    readMatrixArrayAsync(uint32_t _accMemStart, MatrixView&& _matrixView, bool _accRequireResultReady);
    std::shared_ptr<Future> writeMatrixArrayAsync(uint32_t _accMemStart, MatrixView&& _matrixView);
    std::shared_ptr<Future> writeRawInstructionAsync(uint32_t _instruction);

    unsigned registerBreakpoint(std::string_view _functionName, uint32_t _lineNumber, BreakpointCallback _callback);
    void registerBreakpoint(Breakpoint _breakpoint, unsigned _breakpointID);
    void clearBreakpoint(unsigned _breakpointID);
    void continueAfterBreakpoint();
    unsigned hwBreakpoint2UserBreakpointID(unsigned);
};
//-------------------------------------------------------------------------------------
