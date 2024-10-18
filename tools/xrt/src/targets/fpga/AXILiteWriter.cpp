//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <targets/fpga/AXILiteWriter.hpp>

#include <fcntl.h>

AXILiteWriter::AXILiteWriter() {
 /* fd = open(cDeviceName);
  if (fd < 0) {
    throw std::runtime_error(fmt::format("Unable to open device {}: {}", cDeviceName, strerror(errno)));
  }

  regmap = (volatile uint32_t *) mmap(nullptr, cMapSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (regmap == NULL) {
    throw std::runtime_error(fmt::format("Could not mmap device {}: {}", cDeviceName, strerror(errno)));
  }*/
}

uint32_t AXILiteWriter::readRegister(uint32_t _address) {
  return regmap[_address];
}

void AXILiteWriter::writeRegister(uint32_t _address, uint32_t _value) {
  regmap[_address] = _value;
}
