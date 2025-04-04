//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <targets/sim/SimStream.hpp>

#include <cstdint>
#include <functional>
#include <queue>

class MatrixView;
class SimTarget;
class Tb;
struct Arch;

class SimStreams {
    std::function<void()> runClockCycleCallback_;

    AXILiteSimStream registerStream_;
    AXIStreamReadSimStream matrixViewReadStream_;
    AXIStreamWriteSimStream matrixViewWriteStream_;

  public:
    SimStreams(std::function<void()> runClockCycleCallback, const Arch& arch, Tb& tb, std::uint32_t wstrb);
    ~SimStreams();

    SimStreams(const SimStreams& other)            = delete;
    SimStreams(SimStreams&& other)                 = delete;
    auto operator=(const SimStreams& other) -> SimStreams& = delete;
    auto operator=(SimStreams&& other) -> SimStreams&      = delete;

    auto readRegister(std::uint32_t address) -> std::uint32_t;
    void writeRegister(std::uint32_t address, std::uint32_t data);
    void readMatrix(MatrixView& view);
    void writeMatrix(const MatrixView& view);
};
