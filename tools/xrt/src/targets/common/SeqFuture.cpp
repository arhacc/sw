#include <targets/common/SeqFuture.hpp>

SeqFuture::Sequencer::~Sequencer() = default;

SeqFuture::SimpleSequencer::SimpleSequencer(std::vector<Function>&& functions) : functions_(std::move(functions))
{}

SeqFuture::SimpleSequencer::~SimpleSequencer() = default;

SeqFuture::SimpleSequencer::SimpleSequencer(SimpleSequencer&& simpleSequencer) noexcept = default;
auto SeqFuture::SimpleSequencer::operator=(SimpleSequencer&& simpleSequencer) noexcept -> SimpleSequencer& = default;

auto SeqFuture::SimpleSequencer::next() -> std::shared_ptr<Future> {
    if (i_ < functions_.size()) {
        return functions_[i_++]();
    }

    return nullptr;
}

SeqFuture::SeqFuture(std::unique_ptr<Sequencer> sequencer) : sequencer_(std::move(sequencer)), currentFuture_(sequencer_->next()) {}

auto SeqFuture::wait() -> void {
    while (currentFuture_) {
        currentFuture_->wait();
        currentFuture_ = sequencer_->next();
    }
}

auto SeqFuture::wait(std::chrono::nanoseconds d) -> bool {
    while (currentFuture_) {
        currentFuture_->wait();
        currentFuture_ = sequencer_->next();
    }

    return true;
}

auto SeqFuture::isDone() const -> bool {
    return !currentFuture_;
}
