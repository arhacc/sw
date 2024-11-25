//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <chrono>
#include <cstdint>
#include <memory>
#include <span>
#include <variant>
#include <vector>

#include "common/types/Matrix.hpp"

#include <manager/Manager.hpp>

class Manager;

using namespace std::literals::chrono_literals;

inline uint64_t operator""_cycles(unsigned long long int x) {
  return x;
}

class Future {
    Manager* ctx;
    bool done_;

  public:
    inline explicit Future(Manager* _ctx) : ctx(_ctx), done_(false) {}

    virtual ~Future() = default;

    void wait();
    template<typename T>
    bool wait(std::chrono::duration<T> _d) {
        auto _begin = std::chrono::steady_clock::now();

        while (!isDone()) {
            if (std::chrono::duration_cast<std::chrono::duration<T>>(std::chrono::steady_clock::now() - _begin) > _d) {
                return false;
            }

            ctx->runClockCycle();
        }

        return true;
    }
    bool wait(uint64_t _cycles);

    virtual bool isDone() const {
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
    friend class FpgaTarget;

    uint32_t address;
    uint32_t* dataLocation;

  public: // TODO: fix friend class
    inline RegisterReadFuture(Manager* _ctx, uint32_t _address, uint32_t* _dataLocation)
        : RegisterFuture(_ctx), address(_address), dataLocation(_dataLocation) {}
};

class RegisterWriteFuture : public RegisterFuture {
    friend class AXILiteSimStream;
    friend class FpgaTarget;

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

    std::shared_ptr<MatrixView> view;

  public:
    inline MatrixViewReadFuture(Manager* _ctx, std::shared_ptr<MatrixView> _view)
        : MatrixViewFuture(_ctx), view(_view) {}
};

class MatrixViewWriteFuture : public MatrixViewFuture {
    friend class AXIStreamWriteSimStream;

    std::shared_ptr<const MatrixView> view;

  public:
    inline MatrixViewWriteFuture(Manager* _ctx, std::shared_ptr<const MatrixView> _view)
        : MatrixViewFuture(_ctx), view(_view) {}
};

class AndFuture : public Future {
    std::vector<std::shared_ptr<Future>> futures;

  public:
    AndFuture(Manager* _ctx, std::span<std::shared_ptr<Future>> _futures);
    AndFuture(Manager* _ctx, std::vector<std::shared_ptr<Future>>&& _futures);

    ~AndFuture() override = default;

    bool isDone() const override;
};

class OrFuture : public Future {
    std::vector<std::shared_ptr<Future>> futures;

  public:
    OrFuture(Manager* _ctx, std::span<std::shared_ptr<Future>> _futures);
    OrFuture(Manager* _ctx, std::vector<std::shared_ptr<Future>>&& _futures);

    ~OrFuture() override = default;

    bool isDone() const override;
};
