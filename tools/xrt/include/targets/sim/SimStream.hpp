//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <cstddef>
#include <cstdint>

class Future;
class RegisterFuture;
class RegisterReadFuture;
class RegisterWriteFuture;
class MatrixViewReadFuture;
class MatrixViewWriteFuture;
class Tb;

enum class SimStreamStatus {
    Idle,
    Active,
};

class SimStream {
  protected:
    Tb* tb;

#ifndef NDEBUG
    unsigned timeoutClock = 0;
#endif

  public:
    explicit SimStream(Tb* tb);
    virtual ~SimStream();

    virtual SimStreamStatus status() const = 0;
    virtual void step()                    = 0;
    virtual void process(Future*)          = 0;
};

enum class AXILiteSimStreamStatus {
    Idle,
    WriteAwaitingAwready,
    WriteGotAwready,
    WriteAwaitingWready,
    WriteGotWready,
    ReadAwaitingArready,
    ReadGotArready,
    ReadAwaitingRvalid,
    ReadGotRvalid,
};

class AXILiteSimStream : public SimStream {
    AXILiteSimStreamStatus status_;
    RegisterFuture* future;

    uint32_t wstrb;

  public:
    AXILiteSimStream(Tb* tb, uint32_t wstrb);
    ~AXILiteSimStream() override = default;

    void step() override;
    void process(Future*) override;
    SimStreamStatus status() const override;
};

enum class AXIStreamWriteSimStreamStatus {
    Idle,
    Writing,
    Finishing,
};

class AXIStreamWriteSimStream : public SimStream {
    AXIStreamWriteSimStreamStatus status_;
    MatrixViewWriteFuture* future;

    size_t i;
    size_t j;

    uint64_t nextData();
    bool isLastData() const;

  public:
    AXIStreamWriteSimStream(Tb* tb);
    ~AXIStreamWriteSimStream() override = default;

    void step() override;
    void process(Future*) override;
    SimStreamStatus status() const override;
};

enum class AXIStreamReadSimStreamStatus {
    Idle,
    Reading,
    Finishing,
};

class AXIStreamReadSimStream : public SimStream {
    AXIStreamReadSimStreamStatus status_;
    MatrixViewReadFuture* future;

    size_t i;
    size_t j;

    void putNextData(uint64_t _data);
    bool isLastData() const;

  public:
    AXIStreamReadSimStream(Tb* _tb);
    ~AXIStreamReadSimStream() override = default;

    void step() override;
    void process(Future*) override;
    SimStreamStatus status() const override;
};