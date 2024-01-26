//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <cstdint>
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

    std::queue<RegisterFuture*> registerFutures;
    std::queue<MatrixViewReadFuture*> matrixViewReadFutures;
    std::queue<MatrixViewWriteFuture*> matrixViewWriteFutures;

  public:
    SimStreams(Tb* _tb, uint32_t _wstrb);
    ~SimStreams();

    void step();
    void process(Future*);
};
