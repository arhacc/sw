#include "targets/sim/ClockProducerThread.hpp"

#include <algorithm>
#include <cstdint>

ClockProducerThread::ClockProducerThread(
    DUT* dut, Sync& wp, Sync& wd, Sync& rd, Sync& other, Semaphore& exitTick)
    : Thread("clockprodthread"),
      pDUT(dut),
      mWP(wp),
      mWD(wd),
      mRD(rd),
      mOther(other),
      mExitTick(exitTick) {}

void ClockProducerThread::run() {
    std::cout << std::dec << "[" << pDUT->getTime() << "]"
              << " Started: " << getCurrentThreadName() << std::endl;

    std::map<unsigned int, unsigned int> map;

    while (mExitTick.available() == 0) {
        Thread::sleepm(1);
        uint64_t minRequestdTicks;

        minRequestdTicks = UINT64_MAX;
        minRequestdTicks = std::min(
            minRequestdTicks, static_cast<uint64_t>(mWP.getRequestedTicks().available()));
        minRequestdTicks = std::min(
            minRequestdTicks, static_cast<uint64_t>(mWD.getRequestedTicks().available()));
        minRequestdTicks = std::min(
            minRequestdTicks, static_cast<uint64_t>(mRD.getRequestedTicks().available()));

        while (minRequestdTicks-- > 0) {
            mWP.getRequestedTicks().acquire();
            mWD.getRequestedTicks().acquire();
            mRD.getRequestedTicks().acquire();
            map = pDUT->get64Value("m00_axis_tdata");

            for (auto& val : map)
                if (val.first != 0xFFFF'FFFF && val.second != 0xFFFF'FFFF) {
                    // std::cout << "WP WD RD Time: " << pDUT->getTime() << " values: " <<
                    // val.first << ' ' << val.second << std::endl;
                    masterValues.push_back(val.second);
                    masterValues.push_back(val.first);
                }

            pDUT->_wait_half_period_cycle();
            mWP.getGeneratedTicks().release();
            mWD.getGeneratedTicks().release();
            mRD.getGeneratedTicks().release();
        }

        minRequestdTicks = UINT64_MAX;
        minRequestdTicks = std::min(
            minRequestdTicks, static_cast<uint64_t>(mWP.getRequestedTicks().available()));
        minRequestdTicks = std::min(
            minRequestdTicks, static_cast<uint64_t>(mRD.getRequestedTicks().available()));

        while (minRequestdTicks-- > 0) {
            mWP.getRequestedTicks().acquire();
            mRD.getRequestedTicks().acquire();
            map = pDUT->get64Value("m00_axis_tdata");

            for (auto& val : map)
                if (val.first != 0xFFFF'FFFF && val.second != 0xFFFF'FFFF) {
                    // std::cout << "WP RD Time: " << pDUT->getTime() << " values: " <<
                    // val.first << ' ' << val.second << std::endl;

                    masterValues.push_back(val.second);
                    masterValues.push_back(val.first);
                }
            pDUT->_wait_half_period_cycle();
            mWP.getGeneratedTicks().release();
            mRD.getGeneratedTicks().release();
        }

        minRequestdTicks = UINT64_MAX;
        minRequestdTicks = std::min(
            minRequestdTicks, static_cast<uint64_t>(mWP.getRequestedTicks().available()));
        minRequestdTicks = std::min(
            minRequestdTicks, static_cast<uint64_t>(mWD.getRequestedTicks().available()));

        while (minRequestdTicks-- > 0) {
            mWP.getRequestedTicks().acquire();
            mWD.getRequestedTicks().acquire();
            map = pDUT->get64Value("m00_axis_tdata");

            for (auto& val : map)
                if (val.first != 0xFFFF'FFFF && val.second != 0xFFFF'FFFF) {
                    // std::cout << "WP WD Time: " << pDUT->getTime() << " values: " <<
                    // val.first << ' ' << val.second << std::endl;

                    masterValues.push_back(val.second);
                    masterValues.push_back(val.first);
                }
            pDUT->_wait_half_period_cycle();
            mWP.getGeneratedTicks().release();
            mWD.getGeneratedTicks().release();
        }

        while (mWP.getRequestedTicks().available() > 0) {
            mWP.getRequestedTicks().acquire();
            map = pDUT->get64Value("m00_axis_tdata");

            for (auto& val : map) {
                if (val.first != 0xFFFF'FFFF && val.second != 0xFFFF'FFFF) {
                    // std::cout << "WP Time: " << pDUT->getTime() << " values: " <<
                    // val.first << ' ' << val.second << std::endl;

                    masterValues.push_back(val.second);
                    masterValues.push_back(val.first);
                }
            }
            pDUT->_wait_half_period_cycle();
            mWP.getGeneratedTicks().release();
        }

        while (mWD.getRequestedTicks().available() > 0) {
            mWD.getRequestedTicks().acquire();
            map = pDUT->get64Value("m00_axis_tdata");

            for (auto& val : map)
                if (val.first != 0xFFFF'FFFF && val.second != 0xFFFF'FFFF) {
                    // std::cout << "WD Time: " << pDUT->getTime() << " values: " <<
                    // val.first << ' ' << val.second << std::endl;

                    masterValues.push_back(val.second);
                    masterValues.push_back(val.first);
                }
            pDUT->_wait_half_period_cycle();
            mWD.getGeneratedTicks().release();
        }

        while (mRD.getRequestedTicks().available() > 0) {
            mRD.getRequestedTicks().acquire();
            map = pDUT->get64Value("m00_axis_tdata");

            for (auto& val : map)
                if (val.first != 0xFFFF'FFFF && val.second != 0xFFFF'FFFF) {
                    // std::cout << "RD Time: " << pDUT->getTime() << " values: " <<
                    // val.first << ' ' << val.second << std::endl;

                    masterValues.push_back(val.second);
                    masterValues.push_back(val.first);
                }
            pDUT->_wait_half_period_cycle();
            mRD.getGeneratedTicks().release();
        }

        while (mOther.getRequestedTicks().available() > 0) {
            mOther.getRequestedTicks().acquire();
            map = pDUT->get64Value("m00_axis_tdata");

            for (auto& val : map)
                if (val.first != 0xFFFF'FFFF && val.second != 0xFFFF'FFFF) {
                    // std::cout << "Other Time: " << pDUT->getTime() << " values: " <<
                    // val.first << ' ' << val.second << std::endl;

                    masterValues.push_back(val.second);
                    masterValues.push_back(val.first);
                }
            pDUT->_wait_half_period_cycle();
            mOther.getGeneratedTicks().release();
        }
    }
    std::cout << std::dec << "[" << pDUT->getTime() << "]"
              << " Ended: " << getCurrentThreadName() << std::endl;
}
