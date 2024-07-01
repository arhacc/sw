#include <manager/debugmanager/DebugManager.hpp>
#include <cstdint>
#include <mutex>
#include "fmt/core.h"
#include <common/debug/Debug.hpp>
#include <common/log/Logger.hpp>

DebugManager::DebugManager(const Arch &_arch) : arch(_arch)
{}

//-------------------------------------------------------------------------------------
unsigned DebugManager::addBreakpointToSet(std::string_view _setName, std::unique_ptr<ComplexBreakpoint> _breakpoint) {

    logWork.print(fmt::format("Adding breakpoint to set {}\n", _setName));

    if (getSet(_setName) == nullptr) {
        breakpointSets.insert({std::string(_setName), std::vector<std::unique_ptr<ComplexBreakpoint>>{}});
    }

    breakpointSets.at(std::string(_setName)).push_back(std::move(_breakpoint));

    return static_cast<unsigned>(breakpointSets.at(std::string(_setName)).size());
}

//-------------------------------------------------------------------------------------
void DebugManager::clearSet(std::string_view _setName) {
    for (auto it = breakpointSets.begin(); it != breakpointSets.end();) {
        if (it->first == _setName)
            it = breakpointSets.erase(it);
        else
            ++it;
    }
}

//-------------------------------------------------------------------------------------
const std::vector<std::unique_ptr<ComplexBreakpoint>> *DebugManager::getSet(std::string_view setName) {
    try {
        return &breakpointSets.at(std::string(setName));
    } catch (const std::out_of_range&) {
        return nullptr;
    }
}

//-------------------------------------------------------------------------------------
std::vector<Breakpoint> DebugManager::getSetAsHW(std::string_view setName, uint32_t offset) {
    std::vector<Breakpoint> hwBreakpoints;

    auto complexBreakpoints = getSet(setName);
    if (complexBreakpoints != nullptr) {
        for (auto& complexBreakpoint : *complexBreakpoints) {
            hwBreakpoints.push_back(
                complexBreakpoint->toHwBreakpoint({
                    createBreakpointHitCallback(setName),
                    arch,
                    offset
                })
            );
        }
    }

    return hwBreakpoints;
}

//-------------------------------------------------------------------------------------
bool DebugManager::breakpointHitCallback(std::shared_ptr<AcceleratorImage> acc, std::string setName, uint32_t index) {
    std::unique_lock lock(acceleratorImageMutex);

    inBreakpoint = true;
    acceleratorImage = acc;
    activeBreakpointSet = setName;
    activeBreakpointIndex = index;

    logWork.print(fmt::format("breakpoint {} of set {} hit\n", activeBreakpointIndex, activeBreakpointSet));

    return false;
}

//-------------------------------------------------------------------------------------
BreakpointCallback DebugManager::createBreakpointHitCallback(std::string_view setName) {
    std::string setNameStr(setName);

    return [this, setNameStr] (std::shared_ptr<AcceleratorImage> _image, uint32_t index) -> bool {
        return breakpointHitCallback(std::move(_image), setNameStr, index);
    };
}

//-------------------------------------------------------------------------------------
std::shared_ptr<AcceleratorImage> DebugManager::getAcceleratorImage() {
    return acceleratorImage;
}

//-------------------------------------------------------------------------------------
void DebugManager::continueAfterBreakpoint() {
    std::unique_lock lock(acceleratorImageMutex);

    inBreakpoint = false;
    acceleratorImage = nullptr;

}

//-------------------------------------------------------------------------------------
bool DebugManager::isInBreakpoint() const {
    return inBreakpoint;
}

//-------------------------------------------------------------------------------------
unsigned DebugManager::getActiveBreakpointIndex() const {
    return activeBreakpointIndex;
}

//-------------------------------------------------------------------------------------