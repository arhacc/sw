//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>

struct Arch;
class Future;
class SimFuture;
class SimRegisterFuture;
class SimRegisterReadFuture;
class SimRegisterWriteFuture;
class SimMatrixViewReadFuture;
class SimMatrixViewWriteFuture;
class Tb;

enum class SimStreamStatus {
    Idle,
    Active,
};

class SimStream {
  protected:
    Tb* tb;
    const Arch& arch_;

#ifndef NDEBUG
    unsigned timeoutClock = 0;
#endif

  public:
    explicit SimStream(const Arch& arch, Tb* tb);
    virtual ~SimStream();

    virtual SimStreamStatus status() const        = 0;
    virtual void step()                           = 0;
    virtual void process(std::shared_ptr<Future>) = 0;
};

enum class AXILiteSimStreamStatus {
    Idle,
    WriteAwaitingAwready,
    WriteAwaitingWready,
    ReadAwaitingArready,
    ReadAwaitingRvalid,
};

class AXILiteSimStream : public SimStream {
    AXILiteSimStreamStatus status_;
    std::shared_ptr<SimRegisterFuture> future;

    uint32_t wstrb;

  public:
    AXILiteSimStream(const Arch& arch, Tb* tb, uint32_t wstrb);
    ~AXILiteSimStream() override = default;

    void step() override;
    void process(std::shared_ptr<Future>) override;
    SimStreamStatus status() const override;
};

enum class AXIStreamWriteSimStreamStatus {
    Idle,
    Writing,
    Finishing,
};

class AXIStreamWriteSimStream : public SimStream {
    AXIStreamWriteSimStreamStatus status_;
    std::shared_ptr<SimMatrixViewWriteFuture> future;

    size_t i;
    size_t j;

    uint64_t nextData();
    bool isLastData() const;

  public:
    AXIStreamWriteSimStream(const Arch& arch, Tb* tb);
    ~AXIStreamWriteSimStream() override = default;

    void step() override;
    void process(std::shared_ptr<Future>) override;
    SimStreamStatus status() const override;
};

enum class AXIStreamReadSimStreamStatus {
    Idle,
    Reading,
};

class AXIStreamReadSimStream : public SimStream {
    AXIStreamReadSimStreamStatus status_;
    std::shared_ptr<SimMatrixViewReadFuture> future;

    size_t i;
    size_t j;

    void putNextData(uint64_t _data);
    bool isLastData() const;

  public:
    AXIStreamReadSimStream(const Arch& arch, Tb* _tb);
    ~AXIStreamReadSimStream() override = default;

    void step() override;
    void process(std::shared_ptr<Future>) override;
    SimStreamStatus status() const override;
};
