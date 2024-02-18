//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/Utils.hpp>
#include <common/arch/Arch.hpp>
#include <common/cache/Cache.hpp>
#include <common/log/Logger.hpp>
#include <common/types/Matrix.hpp>
#include <manager/Manager.hpp>
#include <manager/driver/Driver.hpp>
#include <manager/libmanager/FunctionInfo.hpp>
#include <manager/libmanager/LibManager.hpp>
#include <manager/libmanager/LibraryResolver.hpp>
#include <manager/libmanager/lowlevel/LowLevelFunctionInfo.hpp>
#include <manager/libmanager/midlevel/ModFunctionInfo.hpp>
#include <manager/memmanager/MemManager.hpp>
#include <manager/memmanager/SymbolInfo.hpp>
#include <targets/Targets.hpp>
#include <targets/common/Future.hpp>

#include <cstdint>
#include <filesystem>
#include <functional>
#include <memory>
#include <span>
#include <stdexcept>
#include <string_view>

#include "common/arch/generated/ArchConstants.hpp"
#include "common/debug/Debug.hpp"
#include "manager/memmanager/UserBreakpoint.hpp"
#include <fmt/core.h>

//-------------------------------------------------------------------------------------
Manager::Manager(std::unique_ptr<Targets> _targets, std::shared_ptr<Arch> _arch)
    : driver(this, _targets.get(), *_arch), arch(std::move(_arch)), targets(std::move(_targets)) {
    memManager = new MemManager(*arch);
    libManager = new LibManager(*arch, memManager, this);

    for (std::unique_ptr<LowLevelFunctionInfo>& _stickyFunction : libManager->stickyFunctionsToLoad()) {
        auto _future = loadLowLevelFunctionAsync(*_stickyFunction, true);
        _future->wait();
    }
}

//-------------------------------------------------------------------------------------
Manager::~Manager() {
    delete libManager;
    delete memManager;
}

//-------------------------------------------------------------------------------------
void Manager::runClockCycle() {
    driver.runClockCycle();
}

//-------------------------------------------------------------------------------------
void Manager::runClockCycles(unsigned _n) {
    driver.runClockCycles(_n);
}

//-------------------------------------------------------------------------------------
std::shared_ptr<Future> Manager::loadLowLevelFunctionAsync(LowLevelFunctionInfo& _lowLevelFunction, bool sticky) {
    memManager->loadFunction(_lowLevelFunction, sticky);

    return driver.writeCodeAsync(_lowLevelFunction.address, _lowLevelFunction.code);
}

//-------------------------------------------------------------------------------------
void Manager::loadUserBreakpoints(std::span<UserBreakpoint> _userBreakpoints, uint32_t _functionAddress) {
    if (_userBreakpoints.size() > arch->get(ArchConstant::DEBUG_NR_BREAKPOINTS)) {
        throw std::runtime_error("too many breakpoints in function");
    }

    // reset breakpoints in hardware
    for (uint32_t _breakpointID = 0; _breakpointID < arch->get(ArchConstant::DEBUG_NR_BREAKPOINTS); _breakpointID++) {
        clearBreakpoint(_breakpointID);
    }

    // reset all other user breakpoints
    for (UserBreakpoint* _activeUserBreakpoint : activeUserBreakpoints) {
        _activeUserBreakpoint->hardwareBreakpointID = std::nullopt;
    }
    activeUserBreakpoints.clear();

    uint32_t _hardwareBreakpointID = 0;
    for (UserBreakpoint& _breakpoint : _userBreakpoints) {
        driver.registerBreakpoint(makeHWBreakpoint(_breakpoint, _functionAddress), _hardwareBreakpointID);

        _breakpoint.hardwareBreakpointID = _hardwareBreakpointID;

        _hardwareBreakpointID++;

        logWork.print(fmt::format(
            "Activated UserBreakpoint[f = {}, line = {}, id = {}] in hw slot {}\n",
            _breakpoint.functionName,
            _breakpoint.lineNumber,
            _breakpoint.id.value(),
            _hardwareBreakpointID));

        activeUserBreakpoints.push_back(&_breakpoint);
    }
}

//-------------------------------------------------------------------------------------
Breakpoint Manager::makeHWBreakpoint(const UserBreakpoint& _userBreakpoint, uint32_t _functionAddress) {
    // TODO: Rollback
    std::vector<BreakpointCondition> _breakpointConditions{
        {arch->get(ArchConstant::DEBUG_BP_COND_COND_EQUAL),
         // arch->get(ArchConstant::DEBUG_BP_COND_OPERAND0_SEL_PC),
         arch->get(ArchConstant::DEBUG_BP_COND_OPERAND0_SEL_CONTROLLER_ACC),
         //     _functionAddress + _userBreakpoint.lineNumber}};
         12345}};

    return Breakpoint{_userBreakpoint.callback, _breakpointConditions, *arch};
}

//-------------------------------------------------------------------------------------
std::shared_ptr<Future> Manager::runLowLevelAsync(FunctionInfo _function, std::span<const uint32_t> _args) {
    if (_function.level != LibLevel::LOW_LEVEL) {
        throw std::runtime_error("Higher class function passed to runLowLevel");
    }
    return runRuntimeAsync(_function.lowLevel, _args);
}

//-------------------------------------------------------------------------------------
std::shared_ptr<Future> Manager::runLowLevelAsync(std::string_view _name, std::span<const uint32_t> _args) {
    return runRuntimeAsync(lowLevel(_name).lowLevel, _args);
}

//-------------------------------------------------------------------------------------
std::shared_ptr<Future> Manager::runLowLevelAsync(std::string_view _name, std::vector<uint32_t>&& _args) {
    return runLowLevelAsync(_name, std::span<const uint32_t>(_args));
}

//-------------------------------------------------------------------------------------
std::shared_ptr<Future> Manager::runLowLevelAsync(FunctionInfo _function, std::vector<uint32_t>&& _args) {
    return runLowLevelAsync(_function, std::span<const uint32_t>(_args));
}

//-------------------------------------------------------------------------------------
void Manager::runLowLevel(FunctionInfo _function, std::span<const uint32_t> _args) {
    std::shared_ptr<Future> _f = runLowLevelAsync(_function, _args);
    _f->wait();
}

//-------------------------------------------------------------------------------------
void Manager::runLowLevel(std::string_view _name, std::span<const uint32_t> _args) {
    std::shared_ptr<Future> _f = runLowLevelAsync(_name, _args);
    _f->wait();
}

//-------------------------------------------------------------------------------------
void Manager::runLowLevel(FunctionInfo _function, std::vector<uint32_t>&& _args) {
    std::shared_ptr<Future> _f = runLowLevelAsync(_function, _args);
    _f->wait();
}

//-------------------------------------------------------------------------------------
void Manager::runLowLevel(std::string_view _name, std::vector<uint32_t>&& _args) {
    std::shared_ptr<Future> _f = runLowLevelAsync(_name, _args);
    _f->wait();
}

//-------------------------------------------------------------------------------------
std::shared_ptr<Future> Manager::runRuntimeAsync(LowLevelFunctionInfo* _function, std::span<const uint32_t> _args) {
    SymbolInfo* _symbol = memManager->resolve(_function->name);

    std::shared_ptr<Future> _writeCodeFuture;

    if (_symbol == nullptr) {
        _writeCodeFuture = loadLowLevelFunctionAsync(*_function);
        _symbol          = memManager->resolve(_function->name);

        assert(_symbol != nullptr);

        logWork.print(fmt::format(
            "Loaded lowlevel function {} at {} size {}\n", _function->name, _symbol->address, _function->memLength()));
    }

    logWork.print(fmt::format("Running lowlevel function {}(", _function->name));
    for (size_t _argIndex = 0; _argIndex < _args.size(); ++_argIndex) {
        uint32_t _arg = _args[_argIndex];

        logWork.print(fmt::format("{}", _arg));

        if (_argIndex != _args.size() - 1) {
            logWork.print(", ");
        }
    }
    logWork.print(fmt::format(") loaded at {}\n", _symbol->address));

    if (_function->breakpoints.size() > 0) {
        if (_writeCodeFuture) {
            _writeCodeFuture->wait();
        }

        loadUserBreakpoints(_function->breakpoints, _symbol->address);
    }

    std::shared_ptr<Future> _runFuture = driver.runAsync(_symbol->address, _args);

    return _writeCodeFuture == nullptr
               ? _runFuture
               : std::make_shared<AndFuture>(this, std::vector<std::shared_ptr<Future>>{_writeCodeFuture, _runFuture});
}

//-------------------------------------------------------------------------------------
unsigned Manager::registerBreakpoint(std::string_view _name, uint32_t _lineNumber, BreakpointCallback _callback) {
    LowLevelFunctionInfo& _function = *libManager->resolve(_name, LibLevel::LOW_LEVEL).lowLevel;

    unsigned _id = static_cast<unsigned>(allUserBreakpoints.size());

    _function.breakpoints.push_back(UserBreakpoint{
        .id                   = _id,
        .hardwareBreakpointID = std::nullopt,
        .callback             = _callback,
        .functionName         = _function.name,
        .lineNumber           = _lineNumber});

    logWork.print(fmt::format(
        "Registered breakpoint: UserBreakpoint[f = {}, line = {}, id = {}]\n", _function.name, _lineNumber, _id));

    allUserBreakpoints.push_back(&_function.breakpoints.back());

    return _id;
};

//-------------------------------------------------------------------------------------
unsigned Manager::hwBreakpoint2UserBreakpointID(unsigned _hardwareBreakpointID) {
    return activeUserBreakpoints.at(_hardwareBreakpointID)->id.value();
}

//-------------------------------------------------------------------------------------
void Manager::runRuntime(LowLevelFunctionInfo* _function, std::span<const uint32_t> _args) {
    std::shared_ptr<Future> _f = runRuntimeAsync(_function, _args);
    _f->wait();
}

//-------------------------------------------------------------------------------------
FunctionInfo Manager::lowLevel(std::string_view _name) {
    FunctionInfo _function = libManager->resolve(_name, LibLevel::LOW_LEVEL);

    assert(_function.level == LibLevel::LOW_LEVEL);

    return _function;
}

//-------------------------------------------------------------------------------------
void Manager::writeMatrixArray(uint32_t _accMemStart, MatrixView&& _matrixView) {
    std::shared_ptr<MatrixView> _matrixViewPtr = std::make_shared<MatrixView>(std::move(_matrixView));
    writeMatrixArray(_accMemStart, _matrixViewPtr);
}

//-------------------------------------------------------------------------------------
void Manager::readMatrixArray(uint32_t _accMemStart, MatrixView&& _matrixView, bool _accRequireResultReady) {
    std::shared_ptr<MatrixView> _matrixViewPtr = std::make_shared<MatrixView>(std::move(_matrixView));
    readMatrixArray(_accMemStart, _matrixViewPtr, _accRequireResultReady);
}

//-------------------------------------------------------------------------------------
void Manager::load(const std::filesystem::path& _givenPath, LibLevel _level) {
    libManager->load(_givenPath, _level);
}

//-------------------------------------------------------------------------------------
unsigned Manager::constant(ArchConstant _constant) const {
    return arch->get(_constant);
}

//-------------------------------------------------------------------------------------
// PURE DRIVER ENCAPSULATION
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
uint32_t Manager::readRegister(uint32_t _address) {
    return driver.readRegister(_address);
}

//-------------------------------------------------------------------------------------
void Manager::writeRegister(uint32_t _address, uint32_t _value) {
    driver.writeRegister(_address, _value);
}

//-------------------------------------------------------------------------------------
void Manager::writeRawInstruction(uint32_t _instruction) {
    driver.writeInstruction(_instruction);
}

//-------------------------------------------------------------------------------------
void Manager::writeRawInstructions(std::span<const uint32_t> _instructions) {
    driver.writeInstructions(_instructions);
}

//-------------------------------------------------------------------------------------
void Manager::writeMatrixArray(uint32_t _accMemStart, std::shared_ptr<const MatrixView> _matrixView) {
    driver.writeMatrixArray(_accMemStart, _matrixView);
}

//-------------------------------------------------------------------------------------
void Manager::readMatrixArray(
    uint32_t _accMemStart, std::shared_ptr<MatrixView> _matrixView, bool _accRequireResultReady) {
    driver.readMatrixArray(_accMemStart, _matrixView, _accRequireResultReady);
}

//-------------------------------------------------------------------------------------
std::shared_ptr<Future> Manager::readRegisterAsync(uint32_t _address, uint32_t* _dataLocation) {
    return driver.readRegisterAsync(_address, _dataLocation);
}

//-------------------------------------------------------------------------------------
std::shared_ptr<Future> Manager::writeRegisterAsync(uint32_t _address, uint32_t _value) {
    return driver.writeRegisterAsync(_address, _value);
}

//-------------------------------------------------------------------------------------
std::shared_ptr<Future> Manager::writeRawInstructionAsync(uint32_t _instruction) {
    return driver.writeInstructionAsync(_instruction);
}

//-------------------------------------------------------------------------------------
std::shared_ptr<Future>
Manager::writeMatrixArrayAsync(uint32_t _accMemStart, std::shared_ptr<const MatrixView> _matrixView) {
    return driver.writeMatrixArrayAsync(_accMemStart, _matrixView);
}

//-------------------------------------------------------------------------------------
std::shared_ptr<Future> Manager::readMatrixArrayAsync(
    uint32_t _accMemStart, std::shared_ptr<MatrixView> _matrixView, bool _accRequireResultReady) {
    return driver.readMatrixArrayAsync(_accMemStart, _matrixView, _accRequireResultReady);
}

//-------------------------------------------------------------------------------------
void Manager::registerBreakpoint(Breakpoint _breakpoint, unsigned _breakpointID) {
    driver.registerBreakpoint(_breakpoint, _breakpointID);
}

//-------------------------------------------------------------------------------------
void Manager::clearBreakpoint(unsigned _breakpointID) {
    // driver.clearBreakpoint(_breakpointID);
}

//-------------------------------------------------------------------------------------
void Manager::continueAfterBreakpoint() {
    driver.continueAfterBreakpoint();
}

//-------------------------------------------------------------------------------------
