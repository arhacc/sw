//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------

#include <common/Utils.hpp>
#include <common/log/Logger.hpp>
#include <targets/sim/XSimFS.hpp>

#include <filesystem>

#include <fmt/core.h>

#ifndef _WIN32

#include <fcntl.h>
#include <sys/file.h>
#include <unistd.h>

void XSimFS::setup(const Arch& _arch) {
    // 1. Create xsim fs

    const auto _xsimFSPath = getPath(ResourceDirectory::XSimFS);
    std::filesystem::create_directories(_xsimFSPath);

    // 2. Create and lock xsim.lock
    const auto _xsimLockPath = _xsimFSPath / "tmp" / "xsim.lock";

    const auto _xsimLockFd = open(_xsimLockPath.c_str(), O_RDWR | O_CREAT | O_TRUNC | O_CLOEXEC, 0600);
    if (_xsimLockFd < 0) {
        throw std::runtime_error(
            fmt::format("Could not open xsimfs lock file at {}: {}", _xsimLockPath.string(), strerror(errno)));
    }

    if (flock(_xsimLockFd, LOCK_EX) < 0) {
        throw std::runtime_error(
            fmt::format("Could not lock xsimfs lock file at {}: {}", _xsimLockPath.string(), strerror(errno)));
    }

    // 3. Create directory for this process

    auto _pid    = getpid();
    auto _pidStr = fmt::format("{}", _pid);

    const auto _xsimMyDir = _xsimFSPath / fmt::format("tmp_xsim{}", _pidStr);

    remove_all(_xsimMyDir);
    create_directories(_xsimMyDir);

    const auto _designPath = getPath(ResourceDirectory::DesignsDir) / _arch.IDString / "xsim.dir";
    copy(
        _designPath,
        _xsimMyDir / "xsim.dir",
        std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive);

    // 4. Create alive file for this process

    const auto _xsimAlivePath = _xsimMyDir / "xsim.alive";

    auto _xsimAliveFd = open(_xsimAlivePath.c_str(), O_RDWR | O_CREAT | O_TRUNC | O_CLOEXEC, 0600);
    if (_xsimLockFd < 0) {
        throw std::runtime_error(
            fmt::format("Could not open xsimfs alive file at {}: {}", _xsimAlivePath.string(), strerror(errno)));
    }

    if (flock(_xsimAliveFd, LOCK_EX | LOCK_NB) < 0) {
        throw std::runtime_error(
            fmt::format("Could not lock xsimfs alive file at {}: {}", _xsimAlivePath.string(), strerror(errno)));
    }

    // 5. Collect stale directories

    logInit.println<InfoMedium>("Cleaning xsimfs directory");
    for (const auto& _xsimEntry : std::filesystem::directory_iterator{_xsimFSPath}) {
        if (_xsimEntry.is_directory() && _xsimEntry.path().filename() != fmt::format("tmp_xsim{}", _pidStr)) {
            auto _xsimOtherAlivePath     = _xsimEntry.path() / "xsim.alive";
            const auto _xsimOtherAliveFd   = open(_xsimOtherAlivePath.c_str(), O_RDWR | O_CLOEXEC);
            if (_xsimOtherAliveFd < 0) {
                continue;
            }

            if (flock(_xsimOtherAliveFd, LOCK_EX | LOCK_NB) < 0) {
                if (errno == EWOULDBLOCK) {
                    logInit.println<InfoMedium>("{} is still alive, skipping", _xsimEntry.path().string());
                    close(_xsimOtherAliveFd);
                    continue;
                } else {
                    logInit.println<Warn>(
                        "WARNING: Could not lock file {} ({}). Skipping directory",
                        _xsimOtherAlivePath.string(),
                        strerror(errno));
                    close(_xsimOtherAliveFd);
                    continue;
                }
            }

            flock(_xsimOtherAliveFd, LOCK_UN);
            close(_xsimOtherAliveFd);

            std::error_code _ec;
            remove_all(_xsimEntry.path(), _ec);
            if (_ec) {
                logInit.println<Warn>(
                    "WARNING: Could not remove stale directory {}: {}", _xsimEntry.path().string(), _ec.message());
            }
        }
    }

    logInit.println<InfoMedium>("xsimfs directory clean");

    // 6. Release xsim.lock

    flock(_xsimLockFd, LOCK_UN);
    close(_xsimLockFd);

    // 7. Change my current directory

    current_path(_xsimMyDir);

    logInit.println<InfoLow>("Changing current directory to {}", _xsimMyDir.string());
}

std::filesystem::path XSimFS::getDesignLibPath(const Arch& arch) {
    setup(arch);

    return std::filesystem::current_path() / "xsim.dir" / "simulator_axi" / "xsimk.so";
}

#else // WINDOWS

#endif
