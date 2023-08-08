#pragma once

#include <chrono>
#include <string>

using namespace std::chrono;

class Timer {
    static uint64_t msStart;
    static Timer* pInstance;

    Timer() {
        msStart = getMsTimeAbs();
    }

  public:
    static Timer* getInstance() {
        if (pInstance == NULL)
            pInstance = new Timer();
        return pInstance;
    }

    static uint64_t getMsTimeAbs() {
        return duration_cast<milliseconds>(system_clock::now().time_since_epoch())
            .count();
    }

    static std::string getMsTimeRelStr() {
        return "[" + std::to_string(getMsTimeAbs() - msStart) + "]";
    }
};
