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
AndFuture::AndFuture(Manager* _ctx, std::span<Future*> _futures, bool _owning) : Future(_ctx), owning(_owning) {
    for (Future* _future : _futures) {
        futures.push_back(_future);
    }
}

//-------------------------------------------------------------------------------------
AndFuture::AndFuture(Manager* _ctx, std::vector<Future*>&& _futures, bool _owning) : Future(_ctx), owning(_owning) {
    futures = std::move(_futures);
}

//-------------------------------------------------------------------------------------
AndFuture::~AndFuture() {
    if (owning) {
        for (Future* _future : futures) {
            delete _future;
        }
    }
}

//-------------------------------------------------------------------------------------
bool AndFuture::isDone() const {
    for (Future* _future : futures) {
        if (!_future->isDone()) {
            return false;
        }
    }

    return true;
}

//-------------------------------------------------------------------------------------
OrFuture::OrFuture(Manager* _ctx, std::span<Future*> _futures, bool _owning) : Future(_ctx), owning(_owning) {
    for (Future* _future : _futures) {
        futures.push_back(_future);
    }
}

//-------------------------------------------------------------------------------------
OrFuture::OrFuture(Manager* _ctx, std::vector<Future*>&& _futures, bool _owning) : Future(_ctx), owning(_owning) {
    futures = std::move(_futures);
}

//-------------------------------------------------------------------------------------
OrFuture::~OrFuture() {
    if (owning) {
        for (Future* _future : futures) {
            delete _future;
        }
    }
}

//-------------------------------------------------------------------------------------
bool OrFuture::isDone() const {
    for (Future* _future : futures) {
        if (_future->isDone()) {
            return true;
        }
    }

    return false;
}

//-------------------------------------------------------------------------------------
