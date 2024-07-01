//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <common/debug/Debug.hpp>
#include <cstdint>

// Data needed to load a breakpoint set to the machine
struct BreakpointSetLoadData {
    BreakpointCallback breakpointCallback;
    const Arch& arch;
    uint32_t baseAddress;
};

class ComplexBreakpoint {
protected:
    ComplexBreakpoint() = default;
public:
    virtual ~ComplexBreakpoint() = default;
    virtual Breakpoint toHwBreakpoint(const BreakpointSetLoadData&) const = 0;
};

class ComplexBreakpointLineNumber final : public ComplexBreakpoint {
private:
    uint32_t lineNumber;
public:
    ComplexBreakpointLineNumber(uint32_t _lineNumber);
    ~ComplexBreakpointLineNumber() override = default;

    Breakpoint toHwBreakpoint(const BreakpointSetLoadData&) const override;
};

class ComplexBreakpointAlwaysBreak final : public ComplexBreakpoint {
public:
    ComplexBreakpointAlwaysBreak() = default;
    ~ComplexBreakpointAlwaysBreak() override = default;

    Breakpoint toHwBreakpoint(const BreakpointSetLoadData&) const override;
};
