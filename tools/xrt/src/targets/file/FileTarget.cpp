//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------

#include <common/CodeGen.hpp>
#include <common/Utils.hpp>
#include <common/arch/Arch.hpp>
#include <common/arch/generated/ArchConstants.hpp>
#include <common/types/Matrix.hpp>
#include <targets/file/FileTarget.hpp>

#include <cinttypes>
#include <cstdint>
#include <cstdio>
#include <iomanip>
#include <stdexcept>

#include <fmt/os.h>
#include <fmt/printf.h>
#include <sys/types.h>

//-------------------------------------------------------------------------------------
FileTarget::FileTarget(std::string_view _path, const Arch& _arch)
    : arch(_arch),
      controllerFile(fmt::output_file(std::string(_path) + ".ctrl")),
      dataFile(fmt::output_file(std::string(_path) + ".data")) {}

//-------------------------------------------------------------------------------------
// UNIMPLEMENTED
//-------------------------------------------------------------------------------------
void FileTarget::reset() {}

//-------------------------------------------------------------------------------------
void FileTarget::process(Future* _future) {
    throw std::runtime_error("Unimplemented FileTarget::process");
}

//-------------------------------------------------------------------------------------
