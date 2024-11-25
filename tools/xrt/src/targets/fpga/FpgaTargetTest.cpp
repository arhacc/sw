#include <targets/fpga/FpgaTarget.hpp>
#include <common/allocator/UDmaSAllocator.hpp>

#include "common/arch/Arch.hpp"
#include "common/arch/generated/ArchConstants.hpp"
#include <gtest/gtest.h>
#include <fmt/format.h>
#include <random>

class FpgaTargetTest : public testing::Test {
 protected:
  Arch arch_;
  FpgaTarget target_;
  bool skip_ = false;
  std::mt19937 randomGenerator_;

  FpgaTargetTest() : target_(arch_) {
    if (!UDmaSAllocator::haveUDma()) {
      std::cerr << "             No u-dma-bug-mgr. Assuming no FPGA. Skipping test." << std::endl;
      skip_ = true;
    }

    // TODO: seed this by reading /dev/urandom
    srand(time(NULL));
    randomGenerator_.seed(rand());
  }
};

TEST_F(FpgaTargetTest, SaneArchitectureHash) {
  if (skip_) {
    SUCCEED();
    return;
  }

  std::string hwArch = fmt::format(
      "xpu_{:08X}{:08X}{:08X}{:08X}",
      target_.readRegister(Arch::IO_INTF_AXILITE_READ_REGS_MD5_word3_REG_ADDR),
      target_.readRegister(Arch::IO_INTF_AXILITE_READ_REGS_MD5_word2_REG_ADDR),
      target_.readRegister(Arch::IO_INTF_AXILITE_READ_REGS_MD5_word1_REG_ADDR),
      target_.readRegister(Arch::IO_INTF_AXILITE_READ_REGS_MD5_word0_REG_ADDR)
  );

  parseArchFile(arch_, hwArch);
}

TEST_F(FpgaTargetTest, WriteID) {
  if (skip_) {
    SUCCEED();
    return;
  }

  std::string hwArch = fmt::format(
      "xpu_{:08X}{:08X}{:08X}{:08X}",
      target_.readRegister(Arch::IO_INTF_AXILITE_READ_REGS_MD5_word3_REG_ADDR),
      target_.readRegister(Arch::IO_INTF_AXILITE_READ_REGS_MD5_word2_REG_ADDR),
      target_.readRegister(Arch::IO_INTF_AXILITE_READ_REGS_MD5_word1_REG_ADDR),
      target_.readRegister(Arch::IO_INTF_AXILITE_READ_REGS_MD5_word0_REG_ADDR)
  );

  parseArchFile(arch_, hwArch);

  uint32_t idValue = randomGenerator_();

  target_.writeRegister(arch_.get(ArchConstant::IO_INTF_AXILITE_WRITE_READ_REGS_ACCELERATOR_ID_REG_ADDR), idValue);

  ASSERT_EQ(target_.readRegister(arch_.get(ArchConstant::IO_INTF_AXILITE_WRITE_READ_REGS_ACCELERATOR_ID_REG_ADDR)), idValue);
}
