//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <targets/common/Future.hpp>

#include <atomic>

class Dma;

template<typename T>
class DmaTFuture final : public Future {
    Dma& dma_;
    std::shared_ptr<T> view_;
    std::atomic_bool done_{false};

  public:
    DmaTFuture(Dma& dma, const std::shared_ptr<T>& view) : dma_(dma), view_(view) {}

    ~DmaTFuture() override = default;

    [[nodiscard]] bool isDone() const override {
        return done_;
    }

    void wait() override {
        // TODO: implement wait using a condition variable
        while (!done_) {
        }
    }

    bool wait(std::chrono::nanoseconds d) override {
        const auto start = std::chrono::high_resolution_clock::now();

        while (!done_) {
            const auto now = std::chrono::high_resolution_clock::now();

            if (now - start < d) {
                return false;
            }
        }

        return true;
    }

    std::shared_ptr<T>& getMatrixView() {
        return view_;
    }

    void setDone() {
        done_ = true;
    }
};

typedef DmaTFuture<MatrixView> DmaReadFuture;
typedef DmaTFuture<const MatrixView> DmaWriteFuture;
