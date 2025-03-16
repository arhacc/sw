//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------

#include <manager/Manager.hpp>
#include <targets/common/Future.hpp>

#include <algorithm>

Future::~Future() = default;

bool Future::wait(const std::chrono::microseconds d) {
    return wait(std::chrono::duration_cast<std::chrono::nanoseconds>(d));
}

bool Future::wait(const std::chrono::milliseconds d) {
    return wait(std::chrono::duration_cast<std::chrono::nanoseconds>(d));
}

bool Future::wait(const std::chrono::seconds d) {
    return wait(std::chrono::duration_cast<std::chrono::nanoseconds>(d));
}

bool Future::wait(const std::chrono::minutes d) {
    return wait(std::chrono::duration_cast<std::chrono::nanoseconds>(d));
}

bool Future::wait(const std::chrono::hours d) {
    return wait(std::chrono::duration_cast<std::chrono::nanoseconds>(d));
}

bool Future::wait(const std::chrono::days d) {
    return wait(std::chrono::duration_cast<std::chrono::nanoseconds>(d));
}

bool Future::wait(const std::chrono::weeks d) {
    return wait(std::chrono::duration_cast<std::chrono::nanoseconds>(d));
}

bool Future::wait(const std::chrono::months d) {
    return wait(std::chrono::duration_cast<std::chrono::nanoseconds>(d));
}

bool Future::wait(const std::chrono::years d) {
    return wait(std::chrono::duration_cast<std::chrono::nanoseconds>(d));
}

AndFuture::AndFuture(std::span<std::shared_ptr<Future>> futures) {
    for (const std::shared_ptr<Future>& future : futures) {
        futures_.push_back(future);
    }
}

AndFuture::AndFuture(std::vector<std::shared_ptr<Future>>&& futures) {
    futures_ = std::move(futures);
}

AndFuture::~AndFuture() = default;


bool AndFuture::isDone() const {
    return std::ranges::all_of(futures_.begin(), futures_.end(), [](const std::shared_ptr<Future>& future) {
        return future->isDone();
    });
}

void AndFuture::wait() {
    for (const auto& future : futures_) {
        future->wait();
    }
}

bool AndFuture::wait(std::chrono::nanoseconds d) {
    for (const auto& future : futures_) {
        if (d < std::chrono::nanoseconds::zero()) {
            return false;
        }

        const auto start = std::chrono::high_resolution_clock::now();
        const auto ok    = future->wait(d);
        const auto stop  = std::chrono::high_resolution_clock::now();

        if (!ok) {
            return false;
        }

        const auto elapsedTime = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);

        d -= elapsedTime;
    }

    return true;
}

bool AndFuture::wait(const std::uint64_t cycles) {
    return std::ranges::all_of(futures_.begin(), futures_.end(), [cycles](auto& future) {
        return future->wait(cycles);
    });
}

NopFuture::NopFuture() = default;

NopFuture::~NopFuture() = default;

bool NopFuture::isDone() const {
    return true;
}

void NopFuture::wait() {}

bool NopFuture::wait(std::chrono::nanoseconds d) {
    return true;
}
