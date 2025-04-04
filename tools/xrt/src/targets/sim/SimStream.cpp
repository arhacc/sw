#include <common/arch/Arch.hpp>
#include <common/types/Matrix.hpp>
#include <targets/sim/SimStream.hpp>
#include <targets/sim/Tb.hpp>

#include <cstdint>
#include <stdexcept>

#include <magic_enum.hpp>

constexpr std::uint32_t cMaxTimeoutClock = 100000;

AXILiteSimStream::AXILiteSimStream(
    const Arch& arch, Tb& tb, const std::uint32_t wstrb)
    : tb_{tb}, arch_{arch}, wstrb_{wstrb} {}

AXILiteSimStream::~AXILiteSimStream() = default;

auto AXILiteSimStream::status() const -> SimStreamStatus {
    return (status_ == Status::Idle) ? SimStreamStatus::Idle : SimStreamStatus::Active;
}

void AXILiteSimStream::read(const std::uint32_t address, std::uint32_t& data) {
    if (status_ != Status::Idle) {
        throw std::runtime_error("AXILiteSimStream::read called when not idle");
    }

    address_      = address;
    dataLocation_ = &data;
    status_       = Status::BeginRead;
}

void AXILiteSimStream::write(const std::uint32_t address, const std::uint32_t data) {
    if (status_ != Status::Idle) {
        throw std::runtime_error("AXILiteSimStream::read called when not idle");
    }

    address_ = address;
    data_    = data;
    status_  = Status::BeginWrite;
}

void AXILiteSimStream::step() {
#ifndef NDEBUG
    if (status_ == Status::Idle) {
        timeoutClock_ = 0;
    } else if (timeoutClock_++ > cMaxTimeoutClock) {
        throw std::runtime_error("AXILiteSimStream timed out");
    }
#endif

    switch (status_) {
        case Status::Idle: {
            break;
        }

        case Status::BeginRead: {
            tb_.write("s00_axi_araddr", address_);
            tb_.write("s00_axi_arvalid", 1);
            tb_.write("s00_axi_rready", 1);
            tb_.write("s00_axi_arprot", 0);

            status_ = Status::ReadAwaitingArready;

            break;
        }

        case Status::ReadAwaitingArready: {
            if (tb_.read("s00_axi_arready") == 1 && tb_.read("s00_axi_arvalid") == 1) {
                tb_.write("s00_axi_arvalid", 0);
                status_ = Status::ReadAwaitingRvalid;
            }

            break;
        }

        case Status::ReadAwaitingRvalid: {
            if (tb_.read("s00_axi_rvalid") == 1 && tb_.read("s00_axi_rready") == 1) {
                tb_.write("s00_axi_rready", 0);

                *dataLocation_ = tb_.read("s00_axi_rdata");

                status_ = Status::Idle;
            }

            break;
        }

        case Status::BeginWrite: {
            tb_.write("s00_axi_awprot", 0);
            tb_.write("s00_axi_awvalid", 1);
            tb_.write("s00_axi_awaddr", address_);
            tb_.write("s00_axi_wdata", data_);
            tb_.write("s00_axi_wstrb", wstrb_);
            tb_.write("s00_axi_bready", 1);

            status_ = Status::WriteAwaitingAwready;

            break;
        }

        case Status::WriteAwaitingAwready: {
            if (tb_.read("s00_axi_awready") == 1 && tb_.read("s00_axi_awvalid") == 1) {
                tb_.write("s00_axi_awvalid", 0);
                tb_.write("s00_axi_wvalid", 1);

                status_ = Status::WriteAwaitingWready;
            }

            break;
        }

        case Status::WriteAwaitingWready: {
            if (tb_.read("s00_axi_wready") == 1 && tb_.read("s00_axi_wvalid") == 1) {
                tb_.write("s00_axi_wvalid", 0);

                status_ = Status::Idle;
            }

            break;
        }
    }
}

AXIStreamWriteSimStream::AXIStreamWriteSimStream(const Arch& arch, Tb& tb) : tb_(tb), arch_(arch) {}

AXIStreamWriteSimStream::~AXIStreamWriteSimStream() = default;

auto AXIStreamWriteSimStream::status() const -> SimStreamStatus {
    return (status_ == Status::Idle) ? SimStreamStatus::Idle : SimStreamStatus::Active;
}

void AXIStreamWriteSimStream::write(const MatrixView& view) {
    view_   = &view;
    status_ = Status::Begin;
}

auto AXIStreamWriteSimStream::nextData() -> std::uint64_t {
    std::uint64_t dataWord = 0;

    assert(i_ < view_->numRows());

    if (arch_.get(ArchConstant::IO_INTF_AXISTREAM_DATA_IN_ENDIANNESS_words_per_packet_EXTERNAL)
        == arch_.get(ArchConstant::ENDIANNESS_BIG_ENDIAN)) {
        dataWord |= ((static_cast<uint64_t>(static_cast<uint32_t>(view_->at(i_, j_)))) << 32);
        if (j_ + 1 < view_->numColumns()) {
            dataWord |= (static_cast<uint64_t>(static_cast<uint32_t>(view_->at(i_, j_ + 1))));
        } // else remains 0 (don't care)
    } else {
        dataWord |= ((static_cast<uint64_t>(static_cast<uint32_t>(view_->at(i_, j_)))));
        if (j_ + 1 < view_->numColumns()) {
            dataWord |= (static_cast<uint64_t>(static_cast<uint32_t>(view_->at(i_, j_ + 1))) << 32);
        } // else remains 0 (don't care)
    }

    j_ += 2;

    if (j_ >= view_->numColumns()) {
        j_ = 0;
        i_++;
    }

    return dataWord;
}

auto AXIStreamWriteSimStream::isLastData() const -> bool {
    return i_ >= view_->numRows();
}

void AXIStreamWriteSimStream::step() {
#ifndef NDEBUG
    if (status_ == Status::Idle) {
        timeoutClock_ = 0;
    } else if (timeoutClock_++ > cMaxTimeoutClock) {
        throw std::runtime_error("AXIStreamWriteSimStream timed out");
    }
#endif
    switch (status_) {
        case Status::Idle: {
            break;
        }

        case Status::Begin: {
            i_ = 0;
            j_ = 0;

            tb_.write("s00_axis_tvalid", 1);
            tb_.write64("s00_axis_tdata", nextData());
            tb_.write("s00_axis_tlast", isLastData() ? 1 : 0);

            if (isLastData()) {
                status_ = Status::Finishing;
            } else {
                status_ = Status::Writing;
            }

            break;
        }

        case Status::Writing: {
            if (tb_.read("s00_axis_tready") == 1) {
                tb_.write("s00_axis_tvalid", 1);
                tb_.write64("s00_axis_tdata", nextData());
                tb_.write("s00_axis_tlast", isLastData() ? 1 : 0);

                if (isLastData()) {
                    status_ = Status::Finishing;
                }
            }

            break;
        }

        case Status::Finishing: {
            if (tb_.read("s00_axis_tready") == 1) {
                tb_.write("s00_axis_tvalid", 0);
                tb_.write64("s00_axis_tdata", 0);
                tb_.write("s00_axis_tlast", isLastData() ? 1 : 0);

                status_ = Status::Idle;
            }

            break;
        }
    }
}
AXIStreamReadSimStream::AXIStreamReadSimStream(const Arch& arch, Tb& tb) : tb_(tb), arch_(arch) {}
AXIStreamReadSimStream::~AXIStreamReadSimStream() = default;

auto AXIStreamReadSimStream::status() const -> SimStreamStatus {
    return (status_ == Status::Idle) ? SimStreamStatus::Idle : SimStreamStatus::Active;
}

void AXIStreamReadSimStream::read(MatrixView& view) {
    view_   = &view;
    status_ = Status::Begin;
}

void AXIStreamReadSimStream::putNextData(const std::uint64_t data) {
    assert(i_ < view_->numRows());

    if (arch_.get(ArchConstant::IO_INTF_AXISTREAM_DATA_OUT_ENDIANNESS_words_per_packet_EXTERNAL)
        == arch_.get(ArchConstant::ENDIANNESS_BIG_ENDIAN)) {
        view_->at(i_, j_) = static_cast<std::int32_t>(data >> 32);
        if (j_ + 1 < view_->numColumns()) {
            view_->at(i_, j_ + 1) = static_cast<std::int32_t>(data);
        } // else remains 0 (don't care)
    } else {
        view_->at(i_, j_) = static_cast<std::int32_t>(data);
        if (j_ + 1 < view_->numColumns()) {
            view_->at(i_, j_ + 1) = static_cast<std::int32_t>(data >> 32);
        } // else remains 0 (don't care)
    }

    j_ += 2;

    if (j_ >= view_->numColumns()) {
        j_ = 0;
        i_++;
    }
}

auto AXIStreamReadSimStream::isLastData() const -> bool {
    return i_ >= view_->numRows();
}

void AXIStreamReadSimStream::step() {
#ifndef NDEBUG
    if (status_ == Status::Idle) {
        timeoutClock_ = 0;
    } else if (timeoutClock_++ > cMaxTimeoutClock) {
        throw std::runtime_error("AXIStreamReadSimStream timed out");
    }
#endif

    switch (status_) {
        case Status::Idle: {
            break;
        }

        case Status::Begin: {
            i_ = 0;
            j_ = 0;

            status_ = Status::Reading;
            tb_.write("m00_axis_tready", 1);

            break;
        }

        case Status::Reading: {
            if (tb_.read("m00_axis_tvalid") == 1) {
                putNextData(tb_.read64("m00_axis_tdata"));

                if (isLastData()) {
                    tb_.write("m00_axis_tready", 0);

                    status_ = Status::Idle;
                } else {
                    tb_.write("m00_axis_tready", 1);
                }
            }

            break;
        }
    }
}
