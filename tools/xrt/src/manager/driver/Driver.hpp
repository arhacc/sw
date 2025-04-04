///
/// \file Driver.hpp
///
/// \brief Definition of class Driver.
#pragma once

#include <common/debug/Debug.hpp>

#include <cstdint>
#include <memory>
#include <span>
#include <vector>

// forward declarations
struct Arch;
struct Breakpoint;
class Future;
class MatrixView;
class Manager;
class Targets;

///
/// \brief The driver handles the communication with the accelerator that does not depend on the interface used.
///
/// In other words, this class breaks down complex commands to the accelerator into low-level register read/write and
/// MatrixView read/write.
///
/// There are async and synchronous (unmarked) versions of some functions. The synchronous versions simply call the
/// async versions and wait on the returned future.
class Driver {
    Targets& targets_;
    Manager& ctx_;
    const Arch& arch_;

    std::shared_ptr<AcceleratorImage> accImage_;
    std::pair<uint32_t, uint32_t> accImageArrayMemValidRows_;

    std::vector<std::unique_ptr<Breakpoint>> breakpoints_;

    void readMatrix(
        std::uint32_t accMemStart,
        MatrixView& view,
        bool accRequireResultReady,
        std::uint32_t reorderCommand,
        bool controller);
    void writeMatrix(std::uint32_t accMemStart, const MatrixView& view, std::uint32_t reorderCommand, bool controller);

  public:
    ///
    /// \breif Initialize the class with the provided context, underlying targets and architecture structure.
    ///
    /// \param ctx The overseeing manager. Used for some callbacks. TODO: replace with callback list.
    /// \param targets The underlying target structure. Read/write register/matrixview commands will be sent here.
    /// \param arch The architecture structrue.
    Driver(Manager& ctx, Targets& targets, Arch& arch);

    ///
    /// \brief Destroy the class.
    ~Driver();

    Driver(const Driver& other)                    = delete;
    Driver(Driver&& other)                         = delete;
    auto operator=(const Driver& other) -> Driver& = delete;
    auto operator=(Driver&& other) -> Driver&      = delete;


    ///
    /// \brief Reset the accelerator.
    ///
    /// This calls the reset function of the underlying target and clears the driver internal state.
    void reset();

    ///
    /// \brief Reset breakpoint state.
    void resetBreakpoints();

    void run(uint32_t address, std::span<const uint32_t> args);

    void writeCode(uint32_t address, std::span<const uint32_t> code);
    ///
    /// \brief Advance the simulation by one clock cycle.
    ///
    /// If the target is not a simulator, this is a no-op. Generally, do not use this function.
    void runClockCycle() const;

    ///
    /// \brief Advance the simulation by a number of clock cycles.
    ///
    /// \param n The number of clock cycles.
    ///
    /// If the target is not a simulator, this is a no-op. Generally, do not use this function.
    void runClockCycles(unsigned n) const;

    void handleInterrupt();
    void handleBreakpointHit();
    void handleBreakpointHitFillAcceleratorImage(AcceleratorImage& accImage);
    void handleBreakpointHitFillAcceleratorImageArrayMem(
        AcceleratorImage& accImage, std::pair<uint32_t, uint32_t> accImageArrayMemValidRows);
    void handleBreakpointHitDumpAcceleratorImage(const AcceleratorImage& accImage);
    auto handleBreakpointHitGetBreakpointID() const -> unsigned;

    void registerBreakpoint(Breakpoint breakpoint, unsigned breakpointID);
    void clearBreakpoint(unsigned breakpointID);
    void clearBreakpoints();
    [[nodiscard]] auto nextAvailableBreakpoint() const -> unsigned;

    // TODO: maybe this needs to be removed and the accelerator should have it's own thread
    void continueAfterBreakpoint();

    auto readRegister(std::uint32_t address) const -> std::uint32_t;
    void writeRegister(std::uint32_t address, std::uint32_t value) const;

    void readMatrixArray(
        std::uint32_t accMemStart, MatrixView& view, bool accRequireResultReady, std::uint32_t reorderCommand = 0);

    void writeMatrixArray(std::uint32_t accMemStart, const MatrixView& matrixView, std::uint32_t _reorderCommand = 0);

    void readMatrixController(
        std::uint32_t accMemStart, MatrixView& view, bool accRequireResultReady, std::uint32_t reorderCommand = 0);

    void writeMatrixController(std::uint32_t accMemStart, const MatrixView& view, std::uint32_t reorderCommand = 0);

    void writeInstruction(std::uint32_t instruction);
    void writeInstruction(std::uint8_t instructionByte, std::uint32_t argument);
    void writeTransferInstruction(std::uint32_t instruction);

    [[nodiscard]] auto getSimSteps() const -> std::uint64_t;
    [[nodiscard]] auto getSimCycles() const -> std::uint64_t;

    void setMaxSimSteps(std::uint64_t steps);
    void setMaxSimCycles(std::uint64_t steps);

    auto getAcceleratorImageFromLog() -> std::shared_ptr<AcceleratorImage>;
};
//-------------------------------------------------------------------------------------
