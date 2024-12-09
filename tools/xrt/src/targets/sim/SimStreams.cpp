//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#include <targets/common/Future.hpp>
#include <targets/sim/SimStream.hpp>
#include <targets/sim/SimStreams.hpp>
#include <common/arch/Arch.hpp>

SimStreams::SimStreams(const Arch &_arch, Tb* _tb, uint32_t _wstrb) {
    registerStream        = new AXILiteSimStream(_arch, _tb, _wstrb);
    matrixViewReadStream  = new AXIStreamReadSimStream(_arch, _tb);
    matrixViewWriteStream = new AXIStreamWriteSimStream(_arch, _tb);
}

SimStreams::~SimStreams() {
    delete registerStream;
    delete matrixViewReadStream;
    delete matrixViewWriteStream;
}

void SimStreams::step() {
    if (!registerFutures.empty() && registerStream->status() == SimStreamStatus::Idle) {
        registerStream->process(registerFutures.front());
        registerFutures.pop();
    }
    if (!matrixViewReadFutures.empty() && matrixViewReadStream->status() == SimStreamStatus::Idle) {
        matrixViewReadStream->process(matrixViewReadFutures.front());
        matrixViewReadFutures.pop();
    }
    if (!matrixViewWriteFutures.empty() && matrixViewWriteStream->status() == SimStreamStatus::Idle) {
        matrixViewWriteStream->process(matrixViewWriteFutures.front());
        matrixViewWriteFutures.pop();
    }

    registerStream->step();
    matrixViewReadStream->step();
    matrixViewWriteStream->step();
}

void SimStreams::process(std::shared_ptr<Future> _future) {
    auto _registerReadFuture    = std::dynamic_pointer_cast<RegisterReadFuture>(_future);
    auto _registerWriteFuture   = std::dynamic_pointer_cast<RegisterWriteFuture>(_future);
    auto _matrixViewReadFuture  = std::dynamic_pointer_cast<MatrixViewReadFuture>(_future);
    auto _matrixViewWriteFuture = std::dynamic_pointer_cast<MatrixViewWriteFuture>(_future);

    if (_registerReadFuture != nullptr) {
        registerFutures.push(_registerReadFuture);
    }

    if (_registerWriteFuture != nullptr) {
        registerFutures.push(_registerWriteFuture);
    }

    if (_matrixViewReadFuture != nullptr) {
        matrixViewReadFutures.push(_matrixViewReadFuture);
    }

    if (_matrixViewWriteFuture != nullptr) {
        matrixViewWriteFutures.push(_matrixViewWriteFuture);
    }
}
