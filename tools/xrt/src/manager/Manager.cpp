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

#include <cstdint>
#include <filesystem>
#include <functional>
#include <memory>
#include <span>
#include <stdexcept>
#include <string_view>

#include <fmt/core.h>

//-------------------------------------------------------------------------------------
Manager::Manager(std::unique_ptr<Targets> _targets, std::shared_ptr<Arch> _arch)
    : driver(this, _targets.get(), *_arch), arch(std::move(_arch)), targets(std::move(_targets)) {
    memManager = new MemManager(&driver, *arch);
    libManager = new LibManager(*arch, memManager, this);

    for (std::unique_ptr<LowLevelFunctionInfo>& _stickyFunction : libManager->stickyFunctionsToLoad()) {
        memManager->loadFunction(*_stickyFunction, true);
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
void Manager::runLowLevel(std::string_view _name, std::span<const uint32_t> _args) {
    runRuntime(lowLevel(_name).lowLevel, _args);
}

//-------------------------------------------------------------------------------------
void Manager::runLowLevel(FunctionInfo _function, std::span<const uint32_t> _args) {
    if (_function.level != LibLevel::LOW_LEVEL) {
        throw std::runtime_error("Higher class function passed to runLowLevel");
    }
    runRuntime(_function.lowLevel, _args);
}

//-------------------------------------------------------------------------------------
void Manager::runLowLevel(std::string_view _name, std::vector<uint32_t>&& _args) {
    runLowLevel(_name, std::span<const uint32_t>(_args));
}

//-------------------------------------------------------------------------------------
void Manager::runLowLevel(FunctionInfo _function, std::vector<uint32_t>&& _args) {
    runLowLevel(_function, std::span<const uint32_t>(_args));
}

//-------------------------------------------------------------------------------------
void Manager::runRuntime(LowLevelFunctionInfo* _function, std::span<const uint32_t> _args) {
    SymbolInfo* _symbol = memManager->resolve(_function->name);

    if (_symbol == nullptr) {
        memManager->loadFunction(*_function);
        _symbol = memManager->resolve(_function->name);

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

    driver.run(_symbol->address, _args);
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
