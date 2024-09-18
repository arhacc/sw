//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------

#include <targets/sim/XSimFS.hpp>
#include <common/Utils.hpp>

#ifndef _WIN32

#include <fcntl.h>
#include <sys/file.h>
#include <unistd.h>

void XSimFS::setup(const Arch& _arch) {

  // 1. Create xsim fs

  auto _xsimFSPath = getPath(ResourceDirectory::XSimFS);
  std::filesyste::create_directories(_xsimFSPath);

  // 2. Create and lock xsim.lock
  auto _xsimLockPath = _ximFSPath / "xsim.lock";

  auto _xsimLockFd = open(_xsimLockPath.c_str(), O_RDWR | O_CREAT | O_TRUNC | O_CLOEXEC, 0600);
  if (_xsimLockFd < 0) {
    throw std::runtime_error(fmt::format("Could not open xsimfs lock file at {}: {}", _xsimLockPath.str(), strerror(errno)));
  }

  if (flock(_xsimLockFd, LOCK_EX) < 0) {
    throw std::runtime_error(fmt::format("Could not lock xsimfs lock file at {}: {}", _xsimLockPath.str(), strerror(errno)));
  }

  // 3. Create directory for this process

  auto _pid = getpid();
  auto _pidStr = fmt::format("{}", _pid);

  auto _xsimMyDir = _xsimFSPath / _pidStr;

  std::filesystem::remove_all(_xsimMyDir);
  std::filesystem::create_directories(_xsimMyDir);

  auto _designPath = getPath(ResourceDirectory::DesignsDir) / "designs" / _arch.IDString / "xsim.dir";
  std::filesystem::copy(_designPath, _xsimMyDir / "xsim.dir");

  // 4. Create alive file for this process

  auto _xsimAlivePath = _xsimMyDir / "alive";

  auto _xsimAliveFd = open(_xsimAlivePath.c_str(), O_RDWR | O_CREAT | O_TRUNC | O_CLOEXEC, 0600);
  if (_xsimLockFd < 0) {
    throw std::runtime_error(fmt::format("Could not open xsimfs alive file at {}: {}", _ximAlivePath.str(), strerror(errno)));
  }

  if (flock(_xsimAliveFd, LOCK_EX | LOCK_NB) < 0) {
    throw std::runtime_error(fmt::format("Could not lock xsimfs alive file at {}: {}", _ximAlivePath.str(), strerror(errno)));
  }

  // 5. Collect stale directories

  logInit.print("Cleaning xsimfs directory\n");
  for (const auto& _xsimEntry : std::filesystem::directory_iterator{_xsimFSPath}) {
    if (_xsimEntry.is_directory() && _xsimEntry.path.filename() != _pidStr) {
      auto _xsimOtherAlivePath = _xsimEntry / "alive";
      auto _xsimOtherAliveFd = open(_xsimOtherAlivePath.c_str(), O_RDWR | O_CLOEXEC);
      if (_xsimOtherAliveFd < 0) {
        logInfo.print(fmt::format("WARNING: Could not open file {} ({}). Skipping directory\n"), _xsimOtherAlivePath.str(), strerror(errno));
        continue;
      }
      
      if (flock(_xsimOtherAliveFd, LOCK_EX | LOCK_NB) < 0) {
        if (errno == EWOULDBLOCK) {
          logInfo.print(fmt::format("{} is still alive, skipping\n", _xsimEntry));
          close(_xsimOtherAliveFd);
          continue;
        } else {
          logInfo.print(fmt::format("WARNING: Could not lock file {} ({}). Skipping directory\n"), _xsimOtherAlivePath.str(), strerror(errno));
          close(_xsimOtherAliveFd);
          continue;
        }
      }

      flock(_xsimOtherAliveFd, LOCK_UN);
      close(_xsimOtherAliveFd);

      std::error_code _ec;
      std::filesystem::remove_all(_xsimEntry, _ec);
      if (_ec) {
        logInit.print(fmt::format("WARNING: Could remove stale directory {}: {}\n", _xsimEntry, _ec.message()));
      }
    }
  }
  
  logInit.print("xsimfs directory clean\n");

  // 6. Release xsim.lock

  flock(_xsimLockFd, LOCK_UN);
  close(_xsimLockFd);

  // 7. Change my current directory

  std::filesystem::current_path(_xsimMyDir);
}

#else // WINDOWS


#endif
