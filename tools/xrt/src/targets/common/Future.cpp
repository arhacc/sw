//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------

#include <manager/Manager.hpp>
#include <targets/common/Future.hpp>
#include <algorithm>

//-------------------------------------------------------------------------------------
AndFuture::AndFuture(std::span<std::shared_ptr<Future>> futures) {
    for (const std::shared_ptr<Future>& future : futures) {
        futures_.push_back(future);
    }
}

//-------------------------------------------------------------------------------------
AndFuture::AndFuture(std::vector<std::shared_ptr<Future>>&& futures) {
    futures_ = std::move(futures);
}

//-------------------------------------------------------------------------------------
bool AndFuture::isDone() const {
    return std::ranges::all_of(futures_.begin(), futures_.end(), [](const std::shared_ptr<Future>& future) {
        return future->isDone();
    });
}

void AndFuture::wait() {
    for (auto& future : futures_) {
        future->wait();
    }
}

bool AndFuture::wait(std::chrono::nanoseconds d) {
    for (auto& future : futures_) {
        if (d < std::chrono::nanoseconds::zero()) {
            return false;
        }

        const auto start = std::chrono::high_resolution_clock::now();
        const auto ok = future->wait(d);
        const auto stop = std::chrono::high_resolution_clock::now();

        if (!ok) {
            return false;
        }

        const auto elapsedTime = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);

        d -= elapsedTime;
    }

    return true;
}

bool AndFuture::wait(std::uint64_t cycles) {
    for (auto& future : futures_) {
        const auto ok = future->wait(cycles);
        if (!ok) {
            return false;
        }
    }

    return true;
}

//-------------------------------------------------------------------------------------
OrFuture::OrFuture(std::span<std::shared_ptr<Future>> futures) {
    for (const std::shared_ptr<Future>& future : futures) {
        futures_.push_back(future);
    }
}

//-------------------------------------------------------------------------------------
OrFuture::OrFuture(std::vector<std::shared_ptr<Future>>&& futures) {
    futures_ = std::move(futures);
}

//-------------------------------------------------------------------------------------
bool OrFuture::isDone() const {
    return std::ranges::any_of(futures_.begin(), futures_.end(), [](const std::shared_ptr<Future>& future) {
        return future->isDone();
    });
}

void OrFuture::wait() {
    futures_.at(0)->wait();
}

bool OrFuture::wait(const std::chrono::nanoseconds d) {
    return futures_.at(0)->wait(d);
}

bool OrFuture::wait(const std::uint64_t cycles) {
    return futures_.at(0)->wait(cycles);
}

bool NopFuture::isDone() const {
    return true;
}

void NopFuture::wait() {}

bool NopFuture::wait([[maybe_unused]] std::chrono::nanoseconds d) {
    return true;
}

bool NopFuture::wait([[maybe_unused]] std::uint64_t cycles) {
    return true;
}

//-------------------------------------------------------------------------------------
