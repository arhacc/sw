//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <cstddef>
#include <cstdint>

#include <gsl/gsl>

class MatrixView;
class Tb;

enum class SimStreamStatus : std::uint_fast8_t {
    Idle,
    Active,
};

class AXILiteSimStream {
private:
    enum class Status : std::uint_fast8_t {
        Idle,
        BeginRead,
        ReadAwaitingArready,
        ReadAwaitingRvalid,
        BeginWrite,
        WriteAwaitingAwready,
        WriteAwaitingWready,
    };

    Tb& tb_;
    const Arch& arch_;

    Status status_{Status::Idle};

    std::uint32_t address_{};
    union {
        std::uint32_t data_;
        std::uint32_t* dataLocation_{};
    };

    const std::uint32_t wstrb_;

    std::uint32_t timeoutClock_{};

  public:
    AXILiteSimStream(const Arch& arch, Tb& tb, std::uint32_t wstrb);
    ~AXILiteSimStream();
    AXILiteSimStream(const AXILiteSimStream& other) = delete;
    AXILiteSimStream(AXILiteSimStream&& other) noexcept = delete;
    auto operator=(const AXILiteSimStream& other) -> AXILiteSimStream& = delete;
    auto operator=(AXILiteSimStream&& other) noexcept -> AXILiteSimStream& = delete;

    void step();
    void read(std::uint32_t address, std::uint32_t& data);
    void write(std::uint32_t address, std::uint32_t data);

    [[nodiscard]] auto status() const -> SimStreamStatus;
};

class AXIStreamWriteSimStream {
    enum class Status : std::uint_fast8_t {
        Idle,
        Begin,
        Writing,
        Finishing,
    };

    Tb& tb_;
    const Arch& arch_;

    Status status_{Status::Idle};

    const MatrixView* view_{};
    std::size_t i_{};
    std::size_t j_{};

    auto nextData() -> std::uint64_t;
    [[nodiscard]] auto isLastData() const -> bool;

    std::uint32_t timeoutClock_{};

  public:
    AXIStreamWriteSimStream(const Arch& arch, Tb& tb);
    ~AXIStreamWriteSimStream();

    void step();
    void write(const MatrixView& view);
    [[nodiscard]] auto status() const -> SimStreamStatus;
};

class AXIStreamReadSimStream {
    enum class Status {
        Idle,
        Begin,
        Reading,
    };

    Tb& tb_;
    const Arch& arch_;

    Status status_{Status::Idle};

    MatrixView* view_{};

    size_t i_{};
    size_t j_{};

    std::uint32_t timeoutClock_{};

    void putNextData(std::uint64_t data);
    [[nodiscard]] auto isLastData() const -> bool;

  public:
    AXIStreamReadSimStream(const Arch& arch, Tb& tb);
    ~AXIStreamReadSimStream();

    void step();
    void read(MatrixView& view);
    [[nodiscard]] auto status() const -> SimStreamStatus;
};
