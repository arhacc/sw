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
    while (!done()) {
        ctx->runClockCycle();
    }
}

//-------------------------------------------------------------------------------------
MuxFuture::MuxFuture(Manager* _ctx, std::span<Future*> _futures, bool _owning) : Future(_ctx), owning(_owning) {
    for (Future* _future : _futures) {
        futures.push_back(_future);
    }
}

//-------------------------------------------------------------------------------------
MuxFuture::MuxFuture(Manager* _ctx, std::vector<Future*>&& _futures, bool _owning) : Future(_ctx), owning(_owning) {
    futures = std::move(_futures);
}

//-------------------------------------------------------------------------------------
MuxFuture::~MuxFuture() {
    if (owning) {
        for (Future* _future : futures) {
            delete _future;
        }
    }
}

//-------------------------------------------------------------------------------------
bool MuxFuture::done() const {
    for (Future* _future : futures) {
        if (!_future->done()) {
            return false;
        }
    }

    return true;
}

//-------------------------------------------------------------------------------------
