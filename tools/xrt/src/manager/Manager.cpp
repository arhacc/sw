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
#include <manager/libmanager/LibManager.hpp>
#include <manager/memmanager/MemManager.hpp>
#include <manager/memmanager/SymbolInfo.hpp>
#include <targets/Targets.hpp>
#include <targets/common/Future.hpp>

#include <cstdint>
#include <filesystem>
#include <memory>
#include <span>
#include <string_view>

#include <common/arch/generated/ArchConstants.hpp>
#include <common/debug/Debug.hpp>
#include <manager/debugmanager/DebugManager.hpp>
#include <manager/memmanager/UserBreakpoint.hpp>
#include <fmt/core.h>

//-------------------------------------------------------------------------------------
Manager::Manager(std::unique_ptr<Targets> _targets, std::shared_ptr<Arch> _arch)
    : driver(this, _targets.get(), *_arch), arch(std::move(_arch)), targets(std::move(_targets)) {
   
    memManager = new MemManager(*arch, [this](){
        return driver.getSimSteps();
    });
    libManager = new LibManager(*arch);
    debugManager = new DebugManager(*arch);

    for (std::unique_ptr<LowLevelFunctionInfo>& _stickyFunction : libManager->stickyFunctionsToLoad()) {
        auto _future = loadLowLevelFunctionAsync(*_stickyFunction, true);
        _future->wait();
    }
}

//-------------------------------------------------------------------------------------
Manager::~Manager() {
    delete libManager;
    delete memManager;
    delete debugManager;
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
std::shared_ptr<Future> Manager::runLowLevelAsync(LowLevelFunctionInfo& _function, std::span<const uint32_t> _args) {
    return runRuntimeAsync(_function, _args);
}

//-------------------------------------------------------------------------------------
std::shared_ptr<Future> Manager::runLowLevelAsync(std::string_view _name, std::span<const uint32_t> _args) {
    return runRuntimeAsync(lowLevel(_name), _args);
}

//-------------------------------------------------------------------------------------
std::shared_ptr<Future> Manager::runLowLevelAsync(std::string_view _name, std::vector<uint32_t>&& _args) {
    return runLowLevelAsync(_name, std::span<const uint32_t>(_args));
}

//-------------------------------------------------------------------------------------
std::shared_ptr<Future> Manager::runLowLevelAsync(LowLevelFunctionInfo& _function, std::vector<uint32_t>&& _args) {
    return runLowLevelAsync(_function, std::span<const uint32_t>(_args));
}

//-------------------------------------------------------------------------------------
void Manager::runLowLevel(LowLevelFunctionInfo& _function, std::span<const uint32_t> _args) {
    std::shared_ptr<Future> _f = runLowLevelAsync(_function, _args);
    _f->wait();
}

//-------------------------------------------------------------------------------------
void Manager::runLowLevel(std::string_view _name, std::span<const uint32_t> _args) {
    std::shared_ptr<Future> _f = runLowLevelAsync(_name, _args);
    _f->wait();
}

//-------------------------------------------------------------------------------------
void Manager::runLowLevel(LowLevelFunctionInfo& _function, std::vector<uint32_t>&& _args) {
    std::shared_ptr<Future> _f = runLowLevelAsync(_function, _args);
    _f->wait();
}

//-------------------------------------------------------------------------------------
void Manager::runLowLevel(std::string_view _name, std::vector<uint32_t>&& _args) {
    std::shared_ptr<Future> _f = runLowLevelAsync(_name, _args);
    _f->wait();
}

//-------------------------------------------------------------------------------------
std::shared_ptr<Future> Manager::runRuntimeAsync(LowLevelFunctionInfo& _function, std::span<const uint32_t> _args) {
    SymbolInfo* _symbol = memManager->resolve(_function.name);

    std::shared_ptr<Future> _writeCodeFuture;

    if (_symbol == nullptr) {
        _writeCodeFuture = loadLowLevelFunctionAsync(_function);
        _symbol          = memManager->resolve(_function.name);

        assert(_symbol != nullptr);

        logWork.println<InfoHigh>(
            "Loaded lowlevel function {} at time {} at {} size {}", _function.name, getSimSteps(), _symbol->address, _function.memLength());
    }

    logWork.print<InfoMedium>("Running lowlevel function {}(", _function.name);
    for (size_t _argIndex = 0; _argIndex < _args.size(); ++_argIndex) {
        uint32_t _arg = _args[_argIndex];

        logWork.print<InfoMedium>("{}", _arg);

        if (_argIndex != _args.size() - 1) {
            logWork.print<InfoMedium>(", ");
        }
    }
    logWork.println<InfoMedium>(") at time {} loaded at {}", getSimSteps(), _symbol->address);

    auto _breakpoints = debugManager->getSetAsHW(_function.name, _symbol->address);

    driver.clearBreakpoints();

    if (_breakpoints.size() > 0) {
        logWork.print<InfoMedium>("Loading {} breakpoints for function {}", _breakpoints.size(), _function.name);
        for (unsigned i = 0; i < _breakpoints.size(); i++) {
            driver.registerBreakpoint(_breakpoints[i], i);
        }
    }

    std::shared_ptr<Future> _runFuture = driver.runAsync(_symbol->address, _args);
    if (_writeCodeFuture == nullptr) {
        return _runFuture;
    } else {
        std::vector<std::shared_ptr<Future>> v{_writeCodeFuture, _runFuture};
        auto f = std::make_shared<AndFuture>(std::move(v));
        return std::dynamic_pointer_cast<Future>(f);
    }
}

//-------------------------------------------------------------------------------------
unsigned Manager::addBreakpointToSet(std::string_view _setName, std::unique_ptr<ComplexBreakpoint> _breakpoint) {
    return debugManager->addBreakpointToSet(_setName, std::move(_breakpoint));
}

//-------------------------------------------------------------------------------------
void Manager::clearSet(std::string_view setName) {
    debugManager->clearSet(setName);
}

//-------------------------------------------------------------------------------------
std::shared_ptr<AcceleratorImage> Manager::getAcceleratorImage() {
    //return debugManager->getAcceleratorImage();
    return driver.getAcceleratorImageFromLog();
}

//-------------------------------------------------------------------------------------
bool Manager::isInBreakpoint() const {
    return debugManager->isInBreakpoint();
}

//-------------------------------------------------------------------------------------
unsigned Manager::getActiveBreakpointIndex() const {
    return debugManager->getActiveBreakpointIndex();
}

//-------------------------------------------------------------------------------------
void Manager::runRuntime(LowLevelFunctionInfo& _function, std::span<const uint32_t> _args) {
    std::shared_ptr<Future> _f = runRuntimeAsync(_function, _args);
    _f->wait();
}

//-------------------------------------------------------------------------------------
LowLevelFunctionInfo& Manager::lowLevel(std::string_view _name) {
    return libManager->resolve(_name);
}

//-------------------------------------------------------------------------------------
void Manager::writeMatrixArray(uint32_t _accMemStart, MatrixView&& _matrixView, uint32_t _reorderCommand) {
    std::shared_ptr<MatrixView> _matrixViewPtr = std::make_shared<MatrixView>(std::move(_matrixView));
    writeMatrixArray(_accMemStart, _matrixViewPtr, _reorderCommand);
}

//-------------------------------------------------------------------------------------
void Manager::readMatrixArray(uint32_t _accMemStart, MatrixView&& _matrixView, bool _accRequireResultReady, uint32_t _reorderCommand) {
    std::shared_ptr<MatrixView> _matrixViewPtr = std::make_shared<MatrixView>(std::move(_matrixView));
    readMatrixArray(_accMemStart, _matrixViewPtr, _accRequireResultReady, _reorderCommand);
}

//-------------------------------------------------------------------------------------
void Manager::writeMatrixController(uint32_t _accMemStart, MatrixView&& _matrixView, uint32_t _reorderCommand) {
    std::shared_ptr<MatrixView> _matrixViewPtr = std::make_shared<MatrixView>(std::move(_matrixView));
    writeMatrixController(_accMemStart, _matrixViewPtr, _reorderCommand);
}

//-------------------------------------------------------------------------------------
void Manager::readMatrixController(uint32_t _accMemStart, MatrixView&& _matrixView, bool _accRequireResultReady, uint32_t _reorderCommand) {
    std::shared_ptr<MatrixView> _matrixViewPtr = std::make_shared<MatrixView>(std::move(_matrixView));
    readMatrixController(_accMemStart, _matrixViewPtr, _accRequireResultReady, _reorderCommand);
}

//-------------------------------------------------------------------------------------
void Manager::loadLowLevel(const std::filesystem::path& _givenPath, std::string_view _name) {
    libManager->load(_givenPath, _name);
}

//-------------------------------------------------------------------------------------
void Manager::initLowLevelStdlib() {
    libManager->initLowLevelStdlib();
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
void Manager::writeMatrixArray(uint32_t _accMemStart, std::shared_ptr<const MatrixView> _matrixView, uint32_t _reorderCommand) {
    driver.writeMatrixArray(_accMemStart, _matrixView, _reorderCommand);
}

//-------------------------------------------------------------------------------------
void Manager::readMatrixArray(
    uint32_t _accMemStart, std::shared_ptr<MatrixView> _matrixView, bool _accRequireResultReady, uint32_t _reorderCommand) {
    driver.readMatrixArray(_accMemStart, _matrixView, _accRequireResultReady, _reorderCommand);
}

//-------------------------------------------------------------------------------------
void Manager::writeMatrixController(uint32_t _accMemStart, std::shared_ptr<const MatrixView> _matrixView, uint32_t _reorderCommand) {
    driver.writeMatrixController(_accMemStart, _matrixView, _reorderCommand);
}

//-------------------------------------------------------------------------------------
void Manager::readMatrixController(
    uint32_t _accMemStart, std::shared_ptr<MatrixView> _matrixView, bool _accRequireResultReady, uint32_t _reorderCommand) {
    driver.readMatrixController(_accMemStart, _matrixView, _accRequireResultReady, _reorderCommand);
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
Manager::writeMatrixArrayAsync(uint32_t _accMemStart, std::shared_ptr<const MatrixView> _matrixView, uint32_t _reorderCommand) {
    return driver.writeMatrixArrayAsync(_accMemStart, _matrixView, _reorderCommand);
}

//-------------------------------------------------------------------------------------
std::shared_ptr<Future> Manager::readMatrixArrayAsync(
    uint32_t _accMemStart, std::shared_ptr<MatrixView> _matrixView, bool _accRequireResultReady, uint32_t _reorderCommand) {
    return driver.readMatrixArrayAsync(_accMemStart, _matrixView, _accRequireResultReady, _reorderCommand);
}

//-------------------------------------------------------------------------------------
std::shared_ptr<Future>
Manager::writeMatrixControllerAsync(uint32_t _accMemStart, std::shared_ptr<const MatrixView> _matrixView, uint32_t _reorderCommand) {
    return driver.writeMatrixControllerAsync(_accMemStart, _matrixView, _reorderCommand);
}

//-------------------------------------------------------------------------------------
std::shared_ptr<Future> Manager::readMatrixControllerAsync(
    uint32_t _accMemStart, std::shared_ptr<MatrixView> _matrixView, bool _accRequireResultReady, uint32_t _reorderCommand) {
    return driver.readMatrixControllerAsync(_accMemStart, _matrixView, _accRequireResultReady, _reorderCommand);
}

// //-------------------------------------------------------------------------------------
// void Manager::registerBreakpoint(Breakpoint _breakpoint, unsigned _breakpointID) {
//     driver.registerBreakpoint(_breakpoint, _breakpointID);
// }

// //-------------------------------------------------------------------------------------
// void Manager::clearBreakpoint(unsigned _breakpointID) {
//     // driver.clearBreakpoint(_breakpointID);
// }
//
uint64_t Manager::getSimSteps() const {
	return driver.getSimSteps();
}

uint64_t Manager::getSimCycles() const {
	return driver.getSimCycles();
}

void Manager::setMaxSimSteps(uint64_t _max) {
	driver.setMaxSimSteps(_max);
}

void Manager::setMaxSimCycles(uint64_t _max) {
	driver.setMaxSimCycles(_max);
}

//-------------------------------------------------------------------------------------
void Manager::continueAfterBreakpoint() {
    debugManager->continueAfterBreakpoint();
    driver.continueAfterBreakpoint();
}

//-------------------------------------------------------------------------------------
