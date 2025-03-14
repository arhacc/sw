//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#include <targets/common/Future.hpp>
#include <targets/sim/SimStream.hpp>
#include <targets/sim/SimStreams.hpp>
#include <targets/sim/SimFuture.hpp>
#include <common/arch/Arch.hpp>

SimStreams::SimStreams(SimTarget& simTarget, const Arch &arch, Tb* tb, uint32_t wstrb) : simTarget_(simTarget) {
    registerStream_        = new AXILiteSimStream(arch, tb, wstrb);
    matrixViewReadStream_  = new AXIStreamReadSimStream(arch, tb);
    matrixViewWriteStream_ = new AXIStreamWriteSimStream(arch, tb);
}

SimStreams::~SimStreams() {
    delete registerStream_;
    delete matrixViewReadStream_;
    delete matrixViewWriteStream_;
}

void SimStreams::step() {
    if (!registerFutures_.empty() && registerStream_->status() == SimStreamStatus::Idle) {
        registerStream_->process(registerFutures_.front());
        registerFutures_.pop();
    }
    if (!matrixViewReadFutures_.empty() && matrixViewReadStream_->status() == SimStreamStatus::Idle) {
        matrixViewReadStream_->process(matrixViewReadFutures_.front());
        matrixViewReadFutures_.pop();
    }
    if (!matrixViewWriteFutures_.empty() && matrixViewWriteStream_->status() == SimStreamStatus::Idle) {
        matrixViewWriteStream_->process(matrixViewWriteFutures_.front());
        matrixViewWriteFutures_.pop();
    }

    registerStream_->step();
    matrixViewReadStream_->step();
    matrixViewWriteStream_->step();
}

std::shared_ptr<Future> SimStreams::createReadRegisterFuture(std::uint32_t address, std::uint32_t* dataLocation) {
    std::shared_ptr<Future> future = std::make_shared<SimRegisterReadFuture>(simTarget_, address, dataLocation);

    registerStream_->process(future);

    return future;
}

std::shared_ptr<Future> SimStreams::createWriteRegisterFuture(std::uint32_t address, std::uint32_t data) {
    std::shared_ptr<Future> future = std::make_shared<SimRegisterWriteFuture>(simTarget_, address, data);

    registerStream_->process(future);

    return future;
}

std::shared_ptr<Future> SimStreams::createReadMatrixViewFuture(const std::shared_ptr<MatrixView>& view) {
    std::shared_ptr<Future> future = std::make_shared<SimMatrixViewReadFuture>(simTarget_, view);

    matrixViewReadStream_->process(future);

    return future;
}

std::shared_ptr<Future> SimStreams::createWriteMatrixViewFuture(const std::shared_ptr<const MatrixView>& view) {
    std::shared_ptr<Future> future = std::make_shared<SimMatrixViewWriteFuture>(simTarget_, view);

    matrixViewWriteStream_->process(future);

    return future;
}
