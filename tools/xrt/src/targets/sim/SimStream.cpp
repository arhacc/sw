//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#include <common/log/Logger.hpp>
#include <common/types/Matrix.hpp>
#include <targets/common/Future.hpp>
#include <targets/sim/SimStream.hpp>
#include <targets/sim/Tb.hpp>

#include <cstdint>
#include <stdexcept>

constexpr unsigned cMaxTimeoutClock = 10000;

SimStream::SimStream(Tb* _tb) : tb(_tb) {}

SimStream::~SimStream() {}

AXILiteSimStream::AXILiteSimStream(Tb* _tb, uint32_t _wstrb)
    : SimStream(_tb), status_(AXILiteSimStreamStatus::Idle), future(nullptr), wstrb(_wstrb) {}

SimStreamStatus AXILiteSimStream::status() const {
    return (future == nullptr) ? SimStreamStatus::Idle : SimStreamStatus::Active;
}

void AXILiteSimStream::process(Future* _future) {
    if (future != nullptr) {
        throw std::runtime_error("AXILiteSimStream::process called when not idle");
    }

    RegisterFuture* _futureRead  = dynamic_cast<RegisterReadFuture*>(_future);
    RegisterFuture* _futureWrite = dynamic_cast<RegisterWriteFuture*>(_future);
    if (_futureRead != nullptr) {
        future = _futureRead;
    } else if (_futureWrite != nullptr) {
        future = _futureWrite;
    } else {
        throw std::runtime_error("Incompatible future sent to AXILiteSimStream");
    }
}

void AXILiteSimStream::step() {
#ifndef NDEBUG
    if (status_ == AXILiteSimStreamStatus::Idle) {
        timeoutClock = 0;
    } else if (timeoutClock++ > cMaxTimeoutClock) {
        throw std::runtime_error("AXILiteSimStream timed out");
    }
#endif

    RegisterReadFuture* readFuture   = dynamic_cast<RegisterReadFuture*>(future);
    RegisterWriteFuture* writeFuture = dynamic_cast<RegisterWriteFuture*>(future);

    switch (status_) {
        case AXILiteSimStreamStatus::Idle: {
            if (readFuture != nullptr) {
                tb->write("s00_axi_araddr", readFuture->address);
                tb->write("s00_axi_arvalid", 1);
                tb->write("s00_axi_rready", 1);
                tb->write("s00_axi_arprot", 0);

                status_ = AXILiteSimStreamStatus::ReadAwaitingArready;

                goto LabelAXILiteStartRead;
            }

            if (writeFuture != nullptr) {
                tb->write("s00_axi_awprot", 0);
                tb->write("s00_axi_awvalid", 1);
                tb->write("s00_axi_awaddr", writeFuture->address);
                tb->write("s00_axi_wdata", writeFuture->data);
                tb->write("s00_axi_wstrb", wstrb);
                tb->write("s00_axi_bready", 1);

                status_ = AXILiteSimStreamStatus::WriteAwaitingAwready;

                goto LabelAXILiteStartWrite;
            }

            break;
        }

        LabelAXILiteStartRead:
        case AXILiteSimStreamStatus::ReadAwaitingArready: {
            if (tb->read("s00_axi_arready") == 1) {
                status_ = AXILiteSimStreamStatus::ReadGotArready;
            }

            break;
        }

        case AXILiteSimStreamStatus::ReadGotArready: {
            tb->write("s00_axi_arvalid", 0);

            status_ = AXILiteSimStreamStatus::ReadAwaitingRvalid;

            // falltrough
        }

        case AXILiteSimStreamStatus::ReadAwaitingRvalid: {
            if (tb->read("s00_axi_rvalid") == 1) {
                status_ = AXILiteSimStreamStatus::ReadGotRvalid;
            }

            break;
        }

        case AXILiteSimStreamStatus::ReadGotRvalid: {
            tb->write("s00_axi_rready", 0);

            *(readFuture->dataLocation) = tb->read("s00_axi_rdata");

            future->setDone();
            future  = nullptr;
            status_ = AXILiteSimStreamStatus::Idle;

            break;
        }

        LabelAXILiteStartWrite:
        case AXILiteSimStreamStatus::WriteAwaitingAwready: {
            if (tb->read("s00_axi_awready") == 1) {
                status_ = AXILiteSimStreamStatus::WriteGotAwready;
            }

            break;
        }

        case AXILiteSimStreamStatus::WriteGotAwready: {
            tb->write("s00_axi_awvalid", 0);
            tb->write("s00_axi_wvalid", 1);

            status_ = AXILiteSimStreamStatus::WriteAwaitingWready;

            // falltrough
        }

        case AXILiteSimStreamStatus::WriteAwaitingWready: {
            if (tb->read("s00_axi_wready") == 1) {
                status_ = AXILiteSimStreamStatus::WriteGotWready;
            }

            break;
        }

        case AXILiteSimStreamStatus::WriteGotWready: {
            tb->write("s00_axi_wvalid", 0);

            future->setDone();
            future  = nullptr;
            status_ = AXILiteSimStreamStatus::Idle;

            break;
        }
    }
}

AXIStreamWriteSimStream::AXIStreamWriteSimStream(Tb* _tb)
    : SimStream(_tb), status_(AXIStreamWriteSimStreamStatus::Idle), future(nullptr) {}

SimStreamStatus AXIStreamWriteSimStream::status() const {
    return (future == nullptr) ? SimStreamStatus::Idle : SimStreamStatus::Active;
}

void AXIStreamWriteSimStream::process(Future* _future) {
    if (future != nullptr) {
        throw std::runtime_error("AXILiteSimStream::process called when not idle");
    }

    future = dynamic_cast<MatrixViewWriteFuture*>(_future);
    if (future == nullptr) {
        throw std::runtime_error("Incompatible future sent to AXIStreamWriteSimStream");
    }
}

uint64_t AXIStreamWriteSimStream::nextData() {
    uint64_t _dataWord = 0;

    assert(i < future->view->numRows());

    _dataWord |= (static_cast<uint64_t>(future->view->at(i, j)) << 32);
    _dataWord |= (static_cast<uint64_t>(future->view->at(i, j + 1)));

    j += 2;

    if (j >= future->view->numColumns()) {
        j = 0;
        i++;
    }

    return _dataWord;
}

bool AXIStreamWriteSimStream::isLastData() const {
    return i >= future->view->numRows();
}

void AXIStreamWriteSimStream::step() {
#ifndef NDEBUG
    if (status_ == AXIStreamWriteSimStreamStatus::Idle) {
        timeoutClock = 0;
    } else if (timeoutClock++ > cMaxTimeoutClock) {
        throw std::runtime_error("AXIStreamWriteSimStream timed out");
    }
#endif

    switch (status_) {
        case AXIStreamWriteSimStreamStatus::Idle: {
            if (future != nullptr) {
                i = 0;
                j = 0;

                status_ = AXIStreamWriteSimStreamStatus::Writing;
            } else {
                break;
            }

            tb->write("s00_axis_tvalid", 1);

            // falltrough
        }

        case AXIStreamWriteSimStreamStatus::Writing: {
            if (tb->read("s00_axis_tready") == 1) {
                tb->write("s00_axis_tvalid", 1);
                tb->write64("s00_axis_tdata", nextData());
                tb->write("s00_axis_tlast", isLastData() ? 1 : 0);

                if (isLastData()) {
                    status_ = AXIStreamWriteSimStreamStatus::Finishing;
                }
            }

            break;
        }

        case AXIStreamWriteSimStreamStatus::Finishing: {
            tb->write("s00_axis_tvalid", 0);
            tb->write("s00_axis_tlast", 0);

            future->setDone();
            future  = nullptr;
            status_ = AXIStreamWriteSimStreamStatus::Idle;

            break;
        }
    }
}

AXIStreamReadSimStream::AXIStreamReadSimStream(Tb* _tb)
    : SimStream(_tb), status_(AXIStreamReadSimStreamStatus::Idle), future(nullptr) {}

SimStreamStatus AXIStreamReadSimStream::status() const {
    return (future == nullptr) ? SimStreamStatus::Idle : SimStreamStatus::Active;
}

void AXIStreamReadSimStream::process(Future* _future) {
    if (future != nullptr) {
        throw std::runtime_error("AXILiteSimStream::process called when not idle");
    }

    future = dynamic_cast<MatrixViewReadFuture*>(_future);
    if (future == nullptr) {
        throw std::runtime_error("Incompatible future sent to AXIStreamReadSimStream");
    }
}

void AXIStreamReadSimStream::putNextData(uint64_t _data) {
    assert(i < future->view->numRows());

    future->view->at(i, j)     = static_cast<uint32_t>(_data >> 32);
    future->view->at(i, j + 1) = static_cast<uint32_t>(_data);

    j += 2;

    if (j >= future->view->numColumns()) {
        j = 0;
        i++;
    }
}

bool AXIStreamReadSimStream::isLastData() const {
    return i >= future->view->numRows();
}

void AXIStreamReadSimStream::step() {
#ifndef NDEBUG
    if (status_ == AXIStreamReadSimStreamStatus::Idle) {
        timeoutClock = 0;
    } else if (timeoutClock++ > cMaxTimeoutClock) {
        throw std::runtime_error("AXIStreamReadSimStream timed out");
    }
#endif

    switch (status_) {
        case AXIStreamReadSimStreamStatus::Idle: {
            i = 0;
            j = 0;

            if (future != nullptr) {
                status_ = AXIStreamReadSimStreamStatus::Reading;
                tb->write("m00_axis_tready", 1);
            } else {
                break;
            }

            // falltrough
        }

        case AXIStreamReadSimStreamStatus::Reading: {
            if (tb->read("m00_axis_tvalid") == 1) {
                tb->write("m00_axis_tready", 1);

                putNextData(tb->read64("m00_axis_tdata"));

                if (isLastData()) {
                    status_ = AXIStreamReadSimStreamStatus::Finishing;
                }
            }

            break;
        }

        case AXIStreamReadSimStreamStatus::Finishing: {
            tb->write("m00_axis_tready", 0);

            future->setDone();
            future  = nullptr;
            status_ = AXIStreamReadSimStreamStatus::Idle;

            break;
        }
    }
}