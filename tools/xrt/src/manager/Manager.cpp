//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/Utils.hpp>
#include <common/arch/Arch.hpp>
#include <common/arch/generated/ArchConstants.hpp>
#include <common/cache/Cache.hpp>
#include <common/debug/Debug.hpp>
#include <common/log/Logger.hpp>
#include <common/types/Matrix.hpp>
#include <manager/Manager.hpp>
#include <manager/driver/Driver.hpp>
#include <manager/memmanager/SymbolInfo.hpp>
#include <manager/memmanager/UserBreakpoint.hpp>
#include <targets/Targets.hpp>

#include <cstdint>
#include <filesystem>
#include <memory>
#include <ranges>
#include <span>
#include <string_view>

#include <fmt/core.h>

Manager::Manager(std::unique_ptr<Targets> targets, std::shared_ptr<Arch> arch)
    : arch_(std::move(arch)),
      targets_(std::move(targets)),
      driver_(*this, *targets_, *arch_),
      libManager_(*arch_),
      memManager_(
          *arch_,
          [this]() {
              return driver_.getSimSteps();
          }),
      debugManager_(*arch_) {
    for (std::unique_ptr<LowLevelFunctionInfo>& _stickyFunction : libManager_.stickyFunctionsToLoad()) {
        loadLowLevelFunction(*_stickyFunction, true);
    }
}

Manager::~Manager() = default;

void Manager::runClockCycle() {
    driver_.runClockCycle();
}

void Manager::runClockCycles(const unsigned n) {
    driver_.runClockCycles(n);
}

void Manager::loadLowLevelFunction(LowLevelFunctionInfo& function, const bool sticky) {
    memManager_.loadFunction(function, sticky);

    driver_.writeCode(function.address, function.code);
}

void Manager::runLowLevel(const std::string_view name, const std::span<const std::uint32_t> args) {
    runLowLevel(lowLevel(name), args);
}

void Manager::runLowLevel(LowLevelFunctionInfo& function, const std::span<const std::uint32_t> args) {
    SymbolInfo* symbol = memManager_.resolve(function.name);

    std::shared_ptr<Future> _writeCodeFuture;

    if (symbol == nullptr) {
        loadLowLevelFunction(function);
        symbol = memManager_.resolve(function.name);

        assert(symbol != nullptr);

        logWork.println<InfoHigh>(
            "Loaded lowlevel function {} at time {} at {} size {}",
            function.name,
            getSimSteps(),
            symbol->address,
            function.memLength());
    }

    logWork.print<InfoMedium>("Running lowlevel function {}(", function.name);
    std::size_t argIndex = 0;
    for (auto arg : args) {
        logWork.print<InfoMedium>("{}", arg);

        if (argIndex != args.size() - 1) {
            logWork.print<InfoMedium>(", ");
        }

        argIndex++;
    }
    logWork.println<InfoMedium>(") at time {} loaded at {}", getSimSteps(), symbol->address);

    const auto breakpoints = debugManager_.getSetAsHW(function.name, symbol->address);

    driver_.clearBreakpoints();

    if (breakpoints.size() > 0) {
        logWork.print<InfoMedium>("Loading {} breakpoints for function {}", breakpoints.size(), function.name);
        for (unsigned i = 0; i < breakpoints.size(); i++) {
            driver_.registerBreakpoint(breakpoints[i], i);
        }
    }

    driver_.run(symbol->address, args);
}

unsigned Manager::addBreakpointToSet(const std::string_view setName, std::unique_ptr<ComplexBreakpoint> breakpoint) {
    return debugManager_.addBreakpointToSet(setName, std::move(breakpoint));
}

void Manager::clearSet(const std::string_view setName) {
    debugManager_.clearSet(setName);
}

std::shared_ptr<AcceleratorImage> Manager::getAcceleratorImage() {
    return driver_.getAcceleratorImageFromLog();
}

bool Manager::isInBreakpoint() const {
    return debugManager_.isInBreakpoint();
}

unsigned Manager::getActiveBreakpointIndex() const {
    return debugManager_.getActiveBreakpointIndex();
}

LowLevelFunctionInfo& Manager::lowLevel(const std::string_view name) const {
    return libManager_.resolve(name);
}

void Manager::loadLowLevel(const std::filesystem::path& path, const std::string_view name) {
    libManager_.load(path, name);
}

void Manager::initLowLevelStdlib() {
    libManager_.initLowLevelStdlib();
}

unsigned Manager::constant(const ArchConstant constant) const {
    return arch_->get(constant);
}

//-------------------------------------------------------------------------------------
// PURE DRIVER ENCAPSULATION
//-------------------------------------------------------------------------------------

std::uint32_t Manager::readRegister(const std::uint32_t address) {
    return driver_.readRegister(address);
}

void Manager::writeRegister(const std::uint32_t address, const std::uint32_t value) {
    driver_.writeRegister(address, value);
}

void Manager::writeInstruction(const std::uint32_t instruction) {
    driver_.writeInstruction(instruction);
}

void Manager::writeInstruction(const std::uint8_t instructionByte, const std::uint32_t argument) {
    driver_.writeInstruction(instructionByte, argument);
}

void Manager::writeTransferInstruction(const std::uint32_t instruction) {
    driver_.writeTransferInstruction(instruction);
}

void Manager::readMatrixArray(
    const std::uint32_t accMemStart, MatrixView& view, const bool accRequireResultReady, const std::uint32_t reorderCommand) {
    driver_.readMatrixArray(accMemStart, view, accRequireResultReady, reorderCommand);
}

void Manager::writeMatrixArray(const std::uint32_t accMemStart, const MatrixView& matrixView, const std::uint32_t reorderCommand) {
    driver_.writeMatrixArray(accMemStart, matrixView, reorderCommand);
}

void Manager::readMatrixController(
    const std::uint32_t accMemStart, MatrixView& view, const bool accRequireResultReady, const std::uint32_t reorderCommand) {
    driver_.readMatrixController(accMemStart, view, accRequireResultReady, reorderCommand);
}

void Manager::writeMatrixController(const std::uint32_t accMemStart, const MatrixView& view, const std::uint32_t reorderCommand) {
    driver_.writeMatrixController(accMemStart, view, reorderCommand);
}

std::uint64_t Manager::getSimSteps() const {
    return driver_.getSimSteps();
}

std::uint64_t Manager::getSimCycles() const {
    return driver_.getSimCycles();
}

void Manager::setMaxSimSteps(const std::uint64_t cycles) {
    driver_.setMaxSimSteps(cycles);
}

void Manager::setMaxSimCycles(const std::uint64_t cycles) {
    driver_.setMaxSimCycles(cycles);
}

void Manager::continueAfterBreakpoint() {
    debugManager_.continueAfterBreakpoint();
    driver_.continueAfterBreakpoint();
}
