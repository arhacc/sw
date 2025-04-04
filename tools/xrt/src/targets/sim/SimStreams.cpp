//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#include <common/arch/Arch.hpp>
#include <targets/sim/SimStream.hpp>
#include <targets/sim/SimStreams.hpp>

#include <cassert>

SimStreams::SimStreams(std::function<void()> runClockCycleCallback, const Arch& arch, Tb& tb, const std::uint32_t wstrb)
    : runClockCycleCallback_(std::move(runClockCycleCallback)),
      registerStream_(arch, tb, wstrb),
      matrixViewReadStream_(arch, tb),
      matrixViewWriteStream_(arch, tb) {}

SimStreams::~SimStreams() = default;

auto SimStreams::readRegister(const std::uint32_t address) -> std::uint32_t {
    std::uint32_t value{};

    assert(registerStream_.status() == SimStreamStatus::Idle);

    registerStream_.read(address, value);

    while (registerStream_.status() != SimStreamStatus::Idle) {
        registerStream_.step();
        runClockCycleCallback_();
    }

    return value;
}

void SimStreams::writeRegister(const std::uint32_t address, const std::uint32_t data) {
    assert(registerStream_.status() == SimStreamStatus::Idle);

    registerStream_.write(address, data);
    while (registerStream_.status() != SimStreamStatus::Idle) {
        registerStream_.step();
        runClockCycleCallback_();
    }
}

void SimStreams::readMatrix(MatrixView& view) {
    assert(matrixViewReadStream_.status() == SimStreamStatus::Idle);

    matrixViewReadStream_.read(view);
    while (matrixViewReadStream_.status() != SimStreamStatus::Idle) {
        matrixViewReadStream_.step();
        runClockCycleCallback_();
    }
}

void SimStreams::writeMatrix(const MatrixView& view) {
    assert(matrixViewWriteStream_.status() == SimStreamStatus::Idle);

    matrixViewWriteStream_.write(view);
    while (matrixViewWriteStream_.status() != SimStreamStatus::Idle) {
        matrixViewWriteStream_.step();
        runClockCycleCallback_();
    }
}
