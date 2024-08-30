//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------

#include <manager/Manager.hpp>
#include <targets/common/Future.hpp>

//-------------------------------------------------------------------------------------
void Future::wait() {
    while (!isDone()) {
        ctx->runClockCycle();
    }
}


//-------------------------------------------------------------------------------------
bool Future::wait(uint64_t _cycles) {
    uint64_t _i{0};

    while (!isDone()) {
        if (_i >= _cycles) {
            return false;
        }

        ctx->runClockCycle();
        _i++;
    }

    return true;
}

//-------------------------------------------------------------------------------------
AndFuture::AndFuture(Manager* _ctx, std::span<std::shared_ptr<Future>> _futures) : Future(_ctx) {
    for (std::shared_ptr<Future> _future : _futures) {
        futures.push_back(_future);
    }
}

//-------------------------------------------------------------------------------------
AndFuture::AndFuture(Manager* _ctx, std::vector<std::shared_ptr<Future>>&& _futures) : Future(_ctx) {
    futures = std::move(_futures);
}

//-------------------------------------------------------------------------------------
bool AndFuture::isDone() const {
    for (std::shared_ptr<Future> _future : futures) {
        if (!_future->isDone()) {
            return false;
        }
    }

    return true;
}

//-------------------------------------------------------------------------------------
OrFuture::OrFuture(Manager* _ctx, std::span<std::shared_ptr<Future>> _futures) : Future(_ctx) {
    for (std::shared_ptr<Future> _future : _futures) {
        futures.push_back(_future);
    }
}

//-------------------------------------------------------------------------------------
OrFuture::OrFuture(Manager* _ctx, std::vector<std::shared_ptr<Future>>&& _futures) : Future(_ctx) {
    futures = std::move(_futures);
}

//-------------------------------------------------------------------------------------
bool OrFuture::isDone() const {
    for (std::shared_ptr<Future> _future : futures) {
        if (_future->isDone()) {
            return true;
        }
    }

    return false;
}

//-------------------------------------------------------------------------------------
