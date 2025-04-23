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
#include <manager/libmanager/LibManager.hpp>
#include <manager/memmanager/MemManager.hpp>
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
class LibraryResolver;
class Targets;
class MatrixView;
struct Arch;
enum class ArchConstant : unsigned int;
struct LowLevelFunctionInfo;

class Manager {
    std::shared_ptr<Arch> arch_;
    std::unique_ptr<Targets> targets_;

    Driver driver_;
    LibManager libManager_;
    MemManager memManager_;
    DebugManager debugManager_;

    void loadLowLevelFunction(LowLevelFunctionInfo& function, bool sticky = false);
    // void loadUserBreakpoints(std::span<UserBreakpoint> _userBreakpoints, uint32_t _functionAddress);

    // Breakpoint makeHWBreakpoint(const UserBreakpoint&, uint32_t _functionAddress);

  public:
    Manager(std::unique_ptr<Targets> targets, std::shared_ptr<Arch> arch);
    ~Manager();

    void runLowLevel(std::string_view name, std::span<const std::uint32_t> args = {});
    void runLowLevel(LowLevelFunctionInfo& function, std::span<const std::uint32_t> args = {});

    inline void runLowLevel(std::string_view name, std::initializer_list<std::uint32_t> args) {
        std::vector<std::uint32_t> argv(args);
        runLowLevel(name, std::move(argv));
    }

    inline void runLowLevel(LowLevelFunctionInfo& function, std::initializer_list<std::uint32_t> args) {
        std::vector<std::uint32_t> argv(args);
        runLowLevel(function, std::move(argv));
    }

    LowLevelFunctionInfo& lowLevel(std::string_view name) const;
    void loadLowLevel(const std::filesystem::path& path, std::string_view name);
    void initLowLevelStdlib();

    void runClockCycle();
    void runClockCycles(unsigned n);

    // debug manager encapsulation

    unsigned addBreakpointToSet(std::string_view, std::unique_ptr<ComplexBreakpoint>);
    void clearSet(std::string_view);
    std::shared_ptr<AcceleratorImage> getAcceleratorImage();
    [[nodiscard]] bool isInBreakpoint() const;
    [[nodiscard]] unsigned getActiveBreakpointIndex() const;

    // arch encapsulation

    [[nodiscard]] unsigned constant(ArchConstant constant) const;

    // used in callbacks

    std::uint32_t readRegister(std::uint32_t address);
    void writeRegister(std::uint32_t address, std::uint32_t value);

    void readMatrixArray(
        std::uint32_t accMemStart, MatrixView& view, bool accRequireResultReady, std::uint32_t reorderCommand = 0);

    void writeMatrixArray(std::uint32_t accMemStart, const MatrixView& matrixView, std::uint32_t reorderCommand = 0);

    void readMatrixController(
        std::uint32_t accMemStart, MatrixView& view, bool accRequireResultReady, std::uint32_t reorderCommand = 0);

    void writeMatrixController(std::uint32_t accMemStart, const MatrixView& view, std::uint32_t reorderCommand = 0);

    void writeInstruction(std::uint32_t instruction);
    void writeInstruction(std::uint8_t instructionByte, std::uint32_t argument);
    void writeTransferInstruction(std::uint32_t instruction);

    // unsigned registerBreakpoint(std::string_view _functionName, std::uint32_t _lineNumber, BreakpointCallback _callback);
    // void registerBreakpoint(Breakpoint _breakpoint, unsigned _breakpointID);
    // void clearBreakpoint(unsigned _breakpointID);
    void continueAfterBreakpoint();
    // unsigned hwBreakpoint2UserBreakpointID(unsigned);
    //
    [[nodiscard]] std::uint64_t getSimSteps() const;
    [[nodiscard]] std::uint64_t getSimCycles() const;

    void setMaxSimSteps(std::uint64_t cycles);
    void setMaxSimCycles(std::uint64_t cycles);
};
//-------------------------------------------------------------------------------------
