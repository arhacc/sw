//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <common/arch/Arch.hpp>

#include <filesystem>

// In theory, xsim generates a directory xsim.dir, which contains a file
// simulator_axi/xsimk.so, which we can load along with an .so from the
// Vivado installation to use the simulator.
//
// In parctice, there are some limitations:
//
// 1. The current working directory must be the parent directory of xsim.dir.
// 2. There can only be one instance of an xsim.dir loaded at a given
// time (no multiple processes). This is an odd restriction, as data
// from xsim.dir is only ever read, however, the simulator locks
// simulator_axi/xsim.mem for some time at startup.
//
// The XSimFS class is meant to overcome these limitations with the
// following approach:
//
// 0. Simulator designs are placed by build scripts in
// $XPU_HOME/lib/designs/<arch_id>/xsim.dir.
// 1. The directory $XPU_HOME/tmp/xsim is created if it does not exist.
// 2. The file $XPU_HOME/tmp/xsim/xsim.lock is create if it does not
// exist and an exclusive lock is obtained on the file.
// 3. The relevant xsim.dir is copied from $XPU_HOME/lib/designs to
// $XPU_HOME/tmp/xsim/<xrt_pid>/xsim.dir.
// 4. The file $XPU_HOME/tmp/xsim/<xrt_pid>/alive is created, and an
// exclusive lock is obtained on this file. This lock will never be
// freed as long as the program is open.
// 5. Garbage from previous xrt runs is collected by iterating over all
// alive files and checking for files we can obtain a lock on (if we
// can obtain a lock, the previous program has died).
// 6. The lock on $XPU_HOME/tmp/xsim/xsim.lock is released.
// 7. The current directory is changed to $XPU_HOME/tmp/xsim/<xrt_pid>. We
// are now ready to load the simulator.

class XSimFS final {
    static void setup(const Arch& arch);

  public:
    XSimFS() = delete;

    static std::filesystem::path getDesignLibPath(const Arch& arch);
};
