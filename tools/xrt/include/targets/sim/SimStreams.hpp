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

class Future;
class RegisterFuture;
class MatrixViewReadFuture;
class MatrixViewWriteFuture;
class SimStream;
class Tb;

class SimStreams {
    SimStream* registerStream;
    SimStream* matrixViewReadStream;
    SimStream* matrixViewWriteStream;

    std::queue<std::shared_ptr<RegisterFuture>> registerFutures;
    std::queue<std::shared_ptr<MatrixViewReadFuture>> matrixViewReadFutures;
    std::queue<std::shared_ptr<MatrixViewWriteFuture>> matrixViewWriteFutures;

  public:
    SimStreams(Tb* _tb, uint32_t _wstrb);
    ~SimStreams();

    void step();
    void process(std::shared_ptr<Future>);
};
