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
#include <manager/libmanager/LowLevelFunctionInfo.hpp>
#include <manager/debugmanager/DebugManager.hpp>

#include <cstdint>
#include <filesystem>
#include <initializer_list>
#include <memory>
#include <span>
#include <string_view>
#include <vector>

#include <manager/memmanager/UserBreakpoint.hpp>

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
    DebugManager *debugManager;

    std::shared_ptr<Arch> arch;
    std::unique_ptr<Targets> targets;



    std::shared_ptr<Future> loadLowLevelFunctionAsync(LowLevelFunctionInfo&, bool sticky = false);
    // void loadUserBreakpoints(std::span<UserBreakpoint> _userBreakpoints, uint32_t _functionAddress);

    // Breakpoint makeHWBreakpoint(const UserBreakpoint&, uint32_t _functionAddress);

  public:
    Manager(std::unique_ptr<Targets> _targets, std::shared_ptr<Arch> _arch);

    ~Manager();

    void runLowLevel(std::string_view _name, std::span<const uint32_t> _args = {});
    void runLowLevel(std::string_view _name, std::vector<uint32_t>&& _args);
    void runLowLevel(LowLevelFunctionInfo& _function, std::span<const uint32_t> _args = {});
    void runLowLevel(LowLevelFunctionInfo& _function, std::vector<uint32_t>&& _args);

    std::shared_ptr<Future> runLowLevelAsync(std::string_view _name, std::span<const uint32_t> _args = {});
    std::shared_ptr<Future> runLowLevelAsync(std::string_view _name, std::vector<uint32_t>&& _args);
    std::shared_ptr<Future> runLowLevelAsync(LowLevelFunctionInfo& _function, std::span<const uint32_t> _args = {});
    std::shared_ptr<Future> runLowLevelAsync(LowLevelFunctionInfo& _name, std::vector<uint32_t>&& _args);

    inline void runLowLevel(LowLevelFunctionInfo& _function, std::initializer_list<uint32_t> _args) {
        std::vector<uint32_t> _argv(_args);
        runLowLevel(_function, _argv);
    }

    LowLevelFunctionInfo& lowLevel(std::string_view _name);
    void loadLowLevel(const std::filesystem::path& _path, std::string_view _name);
    void initLowLevelStdlib();

    void process(std::shared_ptr<Future> _future);

    void runClockCycle();
    void runClockCycles(unsigned);

    // debug manager encapsulation

    unsigned addBreakpointToSet(std::string_view, std::unique_ptr<ComplexBreakpoint>);
    void clearSet(std::string_view);
    std::shared_ptr<AcceleratorImage> getAcceleratorImage();
    bool isInBreakpoint() const;
    unsigned getActiveBreakpointIndex() const;

    // arch encapsulation

    unsigned constant(ArchConstant _constant) const;

    // used in callbacks

    void runRuntime(LowLevelFunctionInfo& _function, std::span<const uint32_t> _args = {});
    std::shared_ptr<Future> runRuntimeAsync(LowLevelFunctionInfo& _function, std::span<const uint32_t> _args = {});

    uint32_t readRegister(uint32_t _address);
    void writeRegister(uint32_t _address, uint32_t _value);
    void readMatrixArray(uint32_t _accMemStart, std::shared_ptr<MatrixView> _matrixView, bool _accRequireResultReady, uint32_t _reorderCommand=0);
    void writeMatrixArray(uint32_t _accMemStart, std::shared_ptr<const MatrixView> _matrixView, uint32_t _reorderCommand=0);
    void readMatrixArray(uint32_t _accMemStart, MatrixView&& _matrixView, bool _accRequireResultReady, uint32_t _reorderCommand=0);
    void writeMatrixArray(uint32_t _accMemStart, MatrixView&& _matrixView, uint32_t _reorderCommand=0);
    void
    readMatrixController(uint32_t _accMemStart, std::shared_ptr<MatrixView> _matrixView, bool _accRequireResultReady, uint32_t _reorderCommand=0);
    void writeMatrixController(uint32_t _accMemStart, std::shared_ptr<const MatrixView> _matrixView, uint32_t _reorderCommand=0);
    void readMatrixController(uint32_t _accMemStart, MatrixView&& _matrixView, bool _accRequireResultReady, uint32_t _reorderCommand=0);
    void writeMatrixController(uint32_t _accMemStart, MatrixView&& _matrixView, uint32_t _reorderCommand=0);
    void writeRawInstruction(uint32_t _instruction);
    void writeRawInstructions(std::span<const uint32_t> _instructions);

    std::shared_ptr<Future> readRegisterAsync(uint32_t _address, uint32_t* _valueLocation);
    std::shared_ptr<Future> writeRegisterAsync(uint32_t _address, uint32_t _value);
    std::shared_ptr<Future>
    readMatrixArrayAsync(uint32_t _accMemStart, std::shared_ptr<MatrixView> _matrixView, bool _accRequireResultReady, uint32_t _reorderCommand=0);
    std::shared_ptr<Future> writeMatrixArrayAsync(uint32_t _accMemStart, std::shared_ptr<const MatrixView> _matrixView, uint32_t _reorderCommand=0);
    std::shared_ptr<Future> readMatrixControllerAsync(
        uint32_t _accMemStart, std::shared_ptr<MatrixView> _matrixView, bool _accRequireResultReady, uint32_t _reorderCommand=0);
    std::shared_ptr<Future>
    writeMatrixControllerAsync(uint32_t _accMemStart, std::shared_ptr<const MatrixView> _matrixView, uint32_t _reorderCommand=0);
    // std::shared_ptr<Future>
    // readMatrixArrayAsync(uint32_t _accMemStart, MatrixView&& _matrixView, bool _accRequireResultReady);
    // std::shared_ptr<Future> writeMatrixArrayAsync(uint32_t _accMemStart, MatrixView&& _matrixView);
    std::shared_ptr<Future> writeRawInstructionAsync(uint32_t _instruction);

    // unsigned registerBreakpoint(std::string_view _functionName, uint32_t _lineNumber, BreakpointCallback _callback);
    // void registerBreakpoint(Breakpoint _breakpoint, unsigned _breakpointID);
    // void clearBreakpoint(unsigned _breakpointID);
    void continueAfterBreakpoint();
    // unsigned hwBreakpoint2UserBreakpointID(unsigned);
    //
    uint64_t getSimSteps() const;
    uint64_t getSimCycles() const;

    void setMaxSimSteps(uint64_t);
    void setMaxSimCycles(uint64_t);
};
//-------------------------------------------------------------------------------------
