//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <cstdint>
#include <memory>
#include <queue>

class Manager;
class Future;
class MatrixView;
class SimFuture;
class SimRegisterFuture;
class SimMatrixViewReadFuture;
class SimMatrixViewWriteFuture;
class SimStream;
class SimTarget;
class Tb;
struct Arch;

class SimStreams {
    SimTarget& simTarget_;

    SimStream* registerStream_;
    SimStream* matrixViewReadStream_;
    SimStream* matrixViewWriteStream_;

    std::queue<std::shared_ptr<Future>> registerFutures_;
    std::queue<std::shared_ptr<Future>> matrixViewReadFutures_;
    std::queue<std::shared_ptr<Future>> matrixViewWriteFutures_;

  public:
    SimStreams(SimTarget& simTarget, const Arch &arch, Tb* tb, std::uint32_t wstrb);
    ~SimStreams();

    void step();

    std::shared_ptr<Future> createReadRegisterFuture(std::uint32_t address, std::uint32_t* dataLocation);
    std::shared_ptr<Future> createWriteRegisterFuture(std::uint32_t address, std::uint32_t data);
    std::shared_ptr<Future> createReadMatrixViewFuture(const std::shared_ptr<MatrixView>& view);
    std::shared_ptr<Future> createWriteMatrixViewFuture(const std::shared_ptr<const MatrixView>& view);
};
