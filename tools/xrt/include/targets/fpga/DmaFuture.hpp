///
/// \file DmaFuture.hpp
///
/// \brief Definition of class DmaFuture.
#pragma once

#include <targets/common/Future.hpp>

#include <atomic>

class Dma;
class MatrixView;

///
/// \brief Generic future for Dma Read/Write.
///
/// \tparam T MatrixView type. MatrixView for read. const MatrixView for read.
template<typename T>
class DmaTFuture final : public Future {
    std::shared_ptr<T> view_;
    std::atomic_bool done_{false};

  public:
    ///
    /// \breif Create a DmaTFuture object based on a particular MatrixView.
    ///
    /// \param view Underlying view to write/write to.
    explicit DmaTFuture(const std::shared_ptr<T>& view) : view_(view) {}

    ///
    /// \brief Destroy a DmaTFuture.
    ~DmaTFuture() override = default;

    ///
    /// \brief Checks whether the transfer has finished.
    ///
    /// \return Ture if the transfer has finished. False if the transfer has not finished.
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

    ///
    /// \brief Returns the requested MatrixView to write/write to.
    ///
    /// This is called by Dma.
    ///
    /// \return The requested MatrixView.
    std::shared_ptr<T>& getMatrixView() {
        return view_;
    }

    ///
    /// \brief Marks the transfer as done.
    ///
    /// This is called by Dma.
    void setDone() {
        done_ = true;
    }
};

typedef DmaTFuture<MatrixView> DmaReadFuture;
typedef DmaTFuture<const MatrixView> DmaWriteFuture;
