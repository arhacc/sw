//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------

#include <filesystem>
#include <common/arch/Arch.hpp>
#include <common/XrtException.h>
#include <string>

enum class LibLevel {
    ANY_LEVEL,
    LOW_LEVEL,
    MID_LEVEL,
    HIGH_LEVEL,
};

class LibNotFoundError : public XrtException {
public:
    LibNotFoundError(const std::string &_name);
    ~LibNotFoundError() override = default;
};

class LibraryResolver {
    static const std::filesystem::path cLibPath;

    const Arch &arch;

public:
    LibraryResolver(const Arch &_arch);
    ~LibraryResolver() = default;

    std::filesystem::path resolve(const std::string &_name, LibLevel _level);
};

