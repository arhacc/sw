//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <cstdint>
#include <memory>
#include <span>
#include <variant>
#include <vector>

#include "common/types/Matrix.hpp"

class MatrixView;

class Manager;

class Future {
    Manager* ctx;
    bool done_;

  public:
    inline explicit Future(Manager* _ctx) : ctx(_ctx), done_(false) {}

    virtual ~Future() = default;

    void wait();

    virtual bool done() const {
        return done_;
    }

    void setDone() {
        done_ = true;
    }
};

class RegisterFuture : public Future {
  public:
    inline explicit RegisterFuture(Manager* _ctx) : Future(_ctx) {}
};

class RegisterReadFuture : public RegisterFuture {
    friend class AXILiteSimStream;

    uint32_t address;
    uint32_t* dataLocation;

  public: // TODO: fix friend class
    inline RegisterReadFuture(Manager* _ctx, uint32_t _address, uint32_t* _dataLocation)
        : RegisterFuture(_ctx), address(_address), dataLocation(_dataLocation) {}
};

class RegisterWriteFuture : public RegisterFuture {
    friend class AXILiteSimStream;
    uint32_t address;
    uint32_t data;

  public:
    inline RegisterWriteFuture(Manager* _ctx, uint32_t _address, uint32_t _data)
        : RegisterFuture(_ctx), address(_address), data(_data){};
};

class MatrixViewFuture : public Future {
  public:
    inline explicit MatrixViewFuture(Manager* _ctx) : Future(_ctx) {}
};

class MatrixViewReadFuture : public MatrixViewFuture {
    friend class AXIStreamReadSimStream;

    MatrixView* view;

  public:
    inline MatrixViewReadFuture(Manager* _ctx, MatrixView* _view) : MatrixViewFuture(_ctx), view(_view) {}
};

class MatrixViewWriteFuture : public MatrixViewFuture {
    friend class AXIStreamWriteSimStream;

    const MatrixView* view;

  public:
    inline MatrixViewWriteFuture(Manager* _ctx, const MatrixView* _view) : MatrixViewFuture(_ctx), view(_view) {}
};

class MuxFuture : public Future {
    std::vector<Future*> futures;
    bool owning;

  public:
    MuxFuture(Manager* _ctx, std::span<Future*> _futures, bool _owning = true);
    MuxFuture(Manager* _ctx, std::vector<Future*>&& _futures, bool _owning = true);

    ~MuxFuture() override;

    bool done() const override;
};
