#include "targets/sim/Semaphore.hpp"

/** Default constructor. Default semaphore is a binary semaphore **/
Semaphore::Semaphore(const size_t& num_permissions) : num_permissions(num_permissions), avail(0) { }

/** Copy constructor. Does not copy state of mutex or condition variable,
    only the number of permissions and number of available permissions **/
Semaphore::Semaphore(const Semaphore& s) : num_permissions(s.num_permissions), avail(s.avail) { }

void Semaphore::acquire() {
    std::unique_lock<std::mutex> lk(m);
    cv.wait(lk, [this] { return avail > 0; });
    avail--;
    lk.unlock();
}

void Semaphore::release() {
    m.lock();
    avail++;
    m.unlock();
    cv.notify_one();
}

size_t Semaphore::available() const {
    return avail;
}