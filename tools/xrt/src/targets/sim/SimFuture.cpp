//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#include <targets/sim/SimFuture.hpp>
#include <targets/sim/SimTarget.hpp>

bool SimFuture::isDone() const {
    return done_;
}

void SimFuture::wait() {
    while (!isDone()) {
        simTarget_.runClockCycle();
    }
}

bool SimFuture::wait(std::chrono::nanoseconds d) {
    const auto begin{std::chrono::steady_clock::now()};

    while (!isDone()) {
        if (std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - begin) > d) {
            return false;
        }

        simTarget_.runClockCycle();
    }

    return true;
}

bool SimFuture::wait(const std::uint64_t cycles) {
    for (std::uint64_t i{0}; i < cycles; ++i) {
        if (isDone()) {
            return true;
        }

        simTarget_.runClockCycle();
    }

    return false;
}

void SimFuture::setDone() {
    done_ = true;
}


SimRegisterReadFuture::SimRegisterReadFuture(
    SimTarget& simTarget, const std::uint32_t address, std::uint32_t* dataLocation)
: SimRegisterFuture(simTarget), address_(address), dataLocation_(dataLocation)
{}

std::uint32_t SimRegisterReadFuture::getAddress() const {
    return address_;
}
std::uint32_t* SimRegisterReadFuture::getDataLocation() const {
    return dataLocation_;
}

SimRegisterWriteFuture::SimRegisterWriteFuture(SimTarget& simTarget, const uint32_t address, const uint32_t data)
    : SimRegisterFuture(simTarget), address_(address), data_(data) {}

std::uint32_t SimRegisterWriteFuture::getAddress() const {
    return address_;
}
std::uint32_t SimRegisterWriteFuture::getData() const {
    return data_;
}

SimMatrixViewFuture::SimMatrixViewFuture(SimTarget& simTarget) : SimFuture(simTarget) {}

SimMatrixViewReadFuture::SimMatrixViewReadFuture(SimTarget& simTarget, const std::shared_ptr<MatrixView>& view)
    : SimMatrixViewFuture(simTarget), view_(view) {
}

const std::shared_ptr<MatrixView>& SimMatrixViewReadFuture::getMatrixView() {
    return view_;
}

SimMatrixViewWriteFuture::SimMatrixViewWriteFuture(SimTarget& simTarget, const std::shared_ptr<const MatrixView>& view)
    : SimMatrixViewFuture(simTarget), view_(view) {}

const std::shared_ptr<const MatrixView>& SimMatrixViewWriteFuture::getMatrixView() {
    return view_;
}
