//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <common/arch/Arch.hpp>
#include <common/log/Logger.hpp>
#include <common/types/Matrix.hpp>
#include <manager/Manager.hpp>

#include <span>
#include <string_view>

typedef Manager Accelerator;

Accelerator* newAccelerator(std::span<std::string_view> _args);
Accelerator* newAccelerator(std::span<const char* const> _args);
inline Accelerator* newAccelerator(int _argc, const char* const* _argv) {
    return newAccelerator({_argv + 1, std::size_t(_argc - 1)});
}