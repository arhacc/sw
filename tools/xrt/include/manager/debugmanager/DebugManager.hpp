//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <common/debug/Debug.hpp>
#include <memory>
#include <string_view>
#include <unordered_map>
#include <vector>
#include "common/arch/Arch.hpp"
#include <sys/types.h>
#include <manager/debugmanager/ComplexBreakpoint.hpp>


class DebugManager {
    const Arch &arch;
    std::mutex acceleratorImageMutex;
    std::shared_ptr<AcceleratorImage> acceleratorImage;
    bool inBreakpoint = false;
    std::string activeBreakpointSet;
    uint32_t activeBreakpointIndex;

    std::unordered_map<std::string, std::vector<std::unique_ptr<ComplexBreakpoint>>> breakpointSets;

    bool breakpointHitCallback(std::shared_ptr<AcceleratorImage>, std::string, uint32_t);
    BreakpointCallback createBreakpointHitCallback(std::string_view);
public:
    DebugManager(const Arch &asrch);

    unsigned addBreakpointToSet(std::string_view, std::unique_ptr<ComplexBreakpoint>);
    void clearSet(std::string_view);
    const std::vector<std::unique_ptr<ComplexBreakpoint>> *getSet(std::string_view);
    std::vector<Breakpoint> getSetAsHW(std::string_view, uint32_t);

    std::shared_ptr<AcceleratorImage> getAcceleratorImage();

    void continueAfterBreakpoint();

    bool isInBreakpoint() const;

    unsigned getActiveBreakpointIndex() const;
};
