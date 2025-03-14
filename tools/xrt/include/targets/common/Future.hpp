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

inline std::uint64_t operator""_cycles(unsigned long long x) {
  return x;
}

class Future {
public:
    virtual ~Future() = default;

    [[nodiscard]] virtual bool isDone() const = 0;

    virtual void wait() = 0;
    virtual bool wait(std::chrono::nanoseconds d) = 0;
    virtual bool wait(std::uint64_t cycles) {
        return wait(10ns * cycles);
    }

    virtual bool wait(std::chrono::microseconds d) {
        return wait(std::chrono::duration_cast<std::chrono::nanoseconds>(d));
    }
    virtual bool wait(std::chrono::milliseconds d) {
        return wait(std::chrono::duration_cast<std::chrono::nanoseconds>(d));
    }
    virtual bool wait(std::chrono::seconds d) {
        return wait(std::chrono::duration_cast<std::chrono::nanoseconds>(d));
    }
    virtual bool wait(std::chrono::minutes d) {
        return wait(std::chrono::duration_cast<std::chrono::nanoseconds>(d));
    }
    virtual bool wait(std::chrono::hours d) {
        return wait(std::chrono::duration_cast<std::chrono::nanoseconds>(d));
    }
    virtual bool wait(std::chrono::days d) {
        return wait(std::chrono::duration_cast<std::chrono::nanoseconds>(d));
    }
    virtual bool wait(std::chrono::weeks d) {
        return wait(std::chrono::duration_cast<std::chrono::nanoseconds>(d));
    }
    virtual bool wait(std::chrono::months d) {
        return wait(std::chrono::duration_cast<std::chrono::nanoseconds>(d));
    }
    virtual bool wait(std::chrono::years d) {
        return wait(std::chrono::duration_cast<std::chrono::nanoseconds>(d));
    }
};


class AndFuture final : public Future {
    std::vector<std::shared_ptr<Future>> futures_;

  public:
    explicit AndFuture(std::span<std::shared_ptr<Future>> futures);
    explicit AndFuture(std::vector<std::shared_ptr<Future>>&& futures);

    ~AndFuture() override = default;

    [[nodiscard]] bool isDone() const override;

    void wait() override;
    bool wait(std::chrono::nanoseconds d) override;
    bool wait(std::uint64_t cycles) override;
};

class OrFuture final : public Future {
    std::vector<std::shared_ptr<Future>> futures_;

  public:
    explicit OrFuture(std::span<std::shared_ptr<Future>> futures);
    explicit OrFuture(std::vector<std::shared_ptr<Future>>&& futures);

    ~OrFuture() override = default;

    [[nodiscard]] bool isDone() const override;

    void wait() override;
    bool wait(std::chrono::nanoseconds d) override;
    bool wait(std::uint64_t cycles) override;
};

class NopFuture final : public Future {
  public:
    NopFuture() = default;
    ~NopFuture() override = default;

    [[nodiscard]] bool isDone() const override;

    void wait() override;
    bool wait(std::chrono::nanoseconds d) override;
    bool wait(std::uint64_t cycles) override;
};
