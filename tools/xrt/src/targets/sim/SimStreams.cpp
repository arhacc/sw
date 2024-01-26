//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#include <targets/common/Future.hpp>
#include <targets/sim/SimStream.hpp>
#include <targets/sim/SimStreams.hpp>

SimStreams::SimStreams(Tb* _tb, uint32_t _wstrb) {
    registerStream        = new AXILiteSimStream(_tb, _wstrb);
    matrixViewReadStream  = new AXIStreamReadSimStream(_tb);
    matrixViewWriteStream = new AXIStreamWriteSimStream(_tb);
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

void SimStreams::process(Future* _future) {
    RegisterReadFuture* _registerReadFuture       = dynamic_cast<RegisterReadFuture*>(_future);
    RegisterWriteFuture* _registerWriteFuture     = dynamic_cast<RegisterWriteFuture*>(_future);
    MatrixViewReadFuture* _matrixViewReadFuture   = dynamic_cast<MatrixViewReadFuture*>(_future);
    MatrixViewWriteFuture* _matrixViewWriteFuture = dynamic_cast<MatrixViewWriteFuture*>(_future);

    if (_registerReadFuture != nullptr) {
        if (registerStream->status() == SimStreamStatus::Idle) {
            registerStream->process(_registerReadFuture);
        } else {
            registerFutures.push(_registerReadFuture);
        }
    }

    if (_registerWriteFuture != nullptr) {
        if (registerStream->status() == SimStreamStatus::Idle) {
            registerStream->process(_registerWriteFuture);
        } else {
            registerFutures.push(_registerWriteFuture);
        }
    }

    if (_matrixViewReadFuture != nullptr) {
        if (matrixViewReadStream->status() == SimStreamStatus::Idle) {
            matrixViewReadStream->process(_matrixViewReadFuture);
        } else {
            matrixViewReadFutures.push(_matrixViewReadFuture);
        }
    }

    if (_matrixViewWriteFuture != nullptr) {
        if (matrixViewWriteStream->status() == SimStreamStatus::Idle) {
            matrixViewWriteStream->process(_matrixViewWriteFuture);
        } else {
            matrixViewWriteFutures.push(_matrixViewWriteFuture);
        }
    }
}
