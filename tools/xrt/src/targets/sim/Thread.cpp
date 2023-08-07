#include "targets/sim/Thread.hpp"

std::map<std::thread::id, std::string> Thread::sThreadIdMap;
std::vector<Thread *> Thread::sThreadList;

void Thread::identify(const std::string &threadName) {
    sThreadIdMap[std::this_thread::get_id()] = threadName;
}

const std::string& Thread::getCurrentThreadName() {
    return sThreadIdMap[std::this_thread::get_id()];
}

void Thread::sleepm(uint32_t milliseconds) {
    usleep(milliseconds * 1000U);
}

void Thread::interruptAll() {
    for (auto entry:sThreadList) {
        entry->interrupt();
    }
}

void Thread::joinAll() {
    for (auto entry:sThreadList) {
        entry->join();
    }
}

Thread::Thread(std::string threadId) : mThreadId(std::move(threadId)) {
    mWorkerThread = nullptr;
    mInterrupted = false;
    mDone = false;
    sThreadList.push_back(this);
}

void Thread::start() {
    mDone = false;
    mInterrupted = false;
    mWorkerThread = new std::thread(&Thread::execute, this);
    sThreadIdMap[mWorkerThread->get_id()] = this->mThreadId;
}

std::thread::id Thread::getId() const {
    return mWorkerThread->get_id();
}

std::string Thread::getStringId() const {
    return mThreadId;
}

void Thread::join() {
    mWorkerThread->join();
}

bool Thread::isDone() const {
    return mDone;
}

bool Thread::isInterrupted() const {
    return mInterrupted;
}

void Thread::interrupt() {
    mInterrupted = true;
}

Thread::~Thread() {
    if (mWorkerThread != nullptr) {
        sThreadIdMap.erase(mWorkerThread->get_id());
        delete mWorkerThread;
    }

    std::remove(sThreadList.begin(), sThreadList.end(), this);
}

void Thread::listAll(bool justactive) {
    for (auto entry:sThreadList) {
        if (justactive) {
            if (!entry->isDone())
                std::cout << entry->getStringId() << " is Done: " << entry->isDone() << std::endl;
        }
        else
            std::cout << entry->getStringId() << " is Done: " << entry->isDone() << std::endl;
    }
}

void Thread::execute() {
    run();
    mDone = true;
}