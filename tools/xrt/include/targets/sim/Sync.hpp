#pragma once
#include "Semaphore.hpp"

class Sync {
    Semaphore mRequestedTicks;
    Semaphore mGeneratedTicks;

public:
    Semaphore& getRequestedTicks() {return mRequestedTicks;}
    Semaphore& getGeneratedTicks() {return mGeneratedTicks;}
};