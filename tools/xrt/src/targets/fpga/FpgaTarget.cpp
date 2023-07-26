//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include "common/arch/Arch.hpp"
#include <cstddef>
#include <cstdint>
#include <cinttypes>
#include <cstdlib>
#include <fmt/core.h>
#include <targets/fpga/FpgaTarget.h>
#include <common/CodeGen.h>
#include <chrono>
#include <thread>
#include <unistd.h>

using namespace std::chrono_literals;

//-------------------------------------------------------------------------------------
FpgaTarget::FpgaTarget(Arch& _arch)
    : arch(_arch) {
    fmt::println("Starting FpgaTarget...");


    unsigned int xpu_status_reg;

    memory_file_descriptor = open("/dev/mem", O_RDWR | O_SYNC);

    if (memory_file_descriptor == -1) {
        fmt::println("Error opening /dev/mem, target:fpga requires root.");
        std::exit(1);
    }

    XPU_POINTER_CONSTANT = (uint32_t *) mmap(nullptr, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, memory_file_descriptor,
            XPU_BASE_ADDR);
    DMA_POINTER_CONSTANT = (uint32_t *) mmap(nullptr, 65535, PROT_READ | PROT_WRITE, MAP_SHARED, memory_file_descriptor,
            DMA_BASE_ADDR);

    std::string _hwArch = fmt::format("xpu_{:08X}{:08X}{:08X}{:08X}",
        readRegister(Arch::IO_INTF_AXILITE_READ_REGS_MD5_word3_REG_ADDR),
        readRegister(Arch::IO_INTF_AXILITE_READ_REGS_MD5_word2_REG_ADDR),
        readRegister(Arch::IO_INTF_AXILITE_READ_REGS_MD5_word1_REG_ADDR),
        readRegister(Arch::IO_INTF_AXILITE_READ_REGS_MD5_word0_REG_ADDR));

    fmt::println("Detected HW architecture {} will overwrite specified or default architecture", _hwArch);

    parseArchFile(_arch, _hwArch);
    
    reset();

    xpu_status_reg = AXI_LITE_read(XPU_POINTER_CONSTANT + _arch.IO_INTF_AXILITE_READ_REGS_STATUS_REG_ADDR);    // write program file
    printf("before loading program file : %x\n", xpu_status_reg);

    io_matrix_max_size = 16 * 1024 * sizeof(uint32_t);

    io_matrix_raw_position = 0x19000000;

    io_matrix = (uint32_t *) mmap(nullptr, io_matrix_max_size, PROT_READ | PROT_WRITE, MAP_SHARED,
            memory_file_descriptor, io_matrix_raw_position);
}

//-------------------------------------------------------------------------------------
FpgaTarget::~FpgaTarget() {
    munmap(DMA_POINTER_CONSTANT, 65535);
    munmap(XPU_POINTER_CONSTANT, 4096);

    close(memory_file_descriptor);
}

//-------------------------------------------------------------------------------------
void FpgaTarget::writeInstruction(uint32_t _instruction)
{
    AXI_LITE_write(XPU_POINTER_CONSTANT, _instruction);
}

//-------------------------------------------------------------------------------------
void FpgaTarget::writeInstruction(uint8_t _instructionByte, uint32_t _argument)
{
    writeInstruction(makeInstruction(arch, _instructionByte, _argument));
}

//-------------------------------------------------------------------------------------
void FpgaTarget::reset() {
    dma_reset(DMA_POINTER_CONSTANT);

    writeRegister(arch.IO_INTF_AXILITE_WRITE_REGS_SOFT_RESET_ADDR, 1);
    usleep(200 * 1000);
    writeRegister(arch.IO_INTF_AXILITE_WRITE_REGS_SOFT_RESET_ADDR, 0);
    usleep(200 * 1000);
}
//-------------------------------------------------------------------------------------
void FpgaTarget::runRuntime(uint32_t _address, uint32_t _argc, uint32_t *_args) {
    printf("Running code at 0x%016" PRIx32 "\n", _address);

    writeInstruction(arch.INSTRB_prun, _address);
    writeInstruction(arch.INSTR_nop);

    for (uint32_t _i = 0; _i < _argc; _i++) {
        writeInstruction(_args[_i]);
        writeInstruction(arch.INSTR_nop);
    }
}

//-------------------------------------------------------------------------------------
void FpgaTarget::runDebug(uint32_t _address, uint32_t *_args, uint32_t _breakpointAddress) {

}

//-------------------------------------------------------------------------------------
uint32_t FpgaTarget::readRegister(uint32_t _address) {

#ifndef NDEBUG
    if (_address % 4 != 0) {
        throw std::runtime_error("Register address must be a multiple of 4");
    }
#endif

    return AXI_LITE_read(XPU_POINTER_CONSTANT + (_address / 4));
}

//-------------------------------------------------------------------------------------
void FpgaTarget::writeRegister(uint32_t _address, uint32_t _value) {
    
#ifndef NDEBUG
    if (_address % 4 != 0) {
        throw std::runtime_error("Register address must be a multiple of 4");
    }
#endif

    AXI_LITE_write(XPU_POINTER_CONSTANT + (_address / 4), _value);
}

//-------------------------------------------------------------------------------------
void FpgaTarget::writeCode(uint32_t _address, uint32_t *_code, uint32_t _length) {
    printf("Writing code at 0x%08" PRIx32 " ", _address);
    printf("length = %5" PRId32 " (0x%016" PRIx32 ")\n", _length, _length);

    writeInstruction(arch.INSTRB_pload, _address);
    writeInstruction(arch.INSTR_nop);

    for (uint32_t _i = 0; _i < _length; ++_i) {
        writeInstruction(_code[_i]);
    }

    writeInstruction(arch.INSTRB_prun, 0);
    writeInstruction(arch.INSTR_nop);
}

//-------------------------------------------------------------------------------------
void FpgaTarget::readControllerData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop,
        uint32_t _columnStart, uint32_t _columnStop) {
    // unimplemented in hardware on current machine on pynq board
}

//-------------------------------------------------------------------------------------
void FpgaTarget::writeControllerData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop,
        uint32_t _columnStart, uint32_t _columnStop) {
    // unimplemented in hardware on current machine on pynq board
}

//-------------------------------------------------------------------------------------
void FpgaTarget::readMatrixArray(uint32_t _accMemStart,
                                 uint32_t *_ramMatrix,
                                 uint32_t _ramTotalLines, uint32_t _ramTotalColumns,
                                 uint32_t _ramStartLine, uint32_t _ramStartColumn,
                                 uint32_t _numLines, uint32_t _numColumns,
                                 bool     _accRequireResultReady) {

    assert(_ramStartLine + _numLines <= _ramTotalLines);
    assert(_ramStartColumn + _numColumns <= _ramTotalColumns);

    if (_numLines * _numColumns * sizeof(uint32_t) > io_matrix_max_size) {
        throw std::runtime_error("Matrix too large");
    }

    getMatrixArray(_accMemStart, io_matrix_raw_position, _numLines, _numColumns, _accRequireResultReady);

    uint32_t io_matrix_i = 0;

    for (uint32_t i = _ramStartLine; i < _ramStartLine + _numLines; i++ ) {
        for (uint32_t j = _ramStartColumn; j < _ramStartColumn + _numColumns; j++) {
            _ramMatrix[i * _ramTotalColumns + j] = io_matrix[io_matrix_i++];
        }
    }
}

//-------------------------------------------------------------------------------------
void FpgaTarget::getMatrixArray(uint32_t _accAddress, uint32_t _rawRamAddress, uint32_t _numLines, uint32_t _numColumns, bool _waitResult) {
    fmt::print("Getting matrix array from 0x{:08x} of dimension {:4}x{:<4} into ram address 0x{:08x}",
            _accAddress, _numLines, _numColumns, _rawRamAddress);

    if (_waitResult) {
        fmt::println(" (waiting for result)");
    } else {
        fmt::println(" (not waiting for result)");
    }

    writeInstruction(_waitResult 
                    ? arch.INSTR_get_matrix_array_w_result_ready
                    : arch.INSTR_get_matrix_array_wo_result_ready);
    writeInstruction(arch.INSTR_nop);
    writeInstruction(0, _accAddress);
    writeInstruction(arch.INSTR_nop);
    writeInstruction(0, _numLines);
    writeInstruction(arch.INSTR_nop);
    writeInstruction(_numColumns);
    writeInstruction(arch.INSTR_nop);

    printf("Status reg: %d\n", readRegister(0x10));
    printf("FIFO data in count: %d\n", readRegister(36));
    printf("FIFO data out count: %d\n", readRegister(40));

    uint32_t _transferLength = _numLines * _numColumns;

    DMA_read(DMA_POINTER_CONSTANT, _rawRamAddress, _transferLength * sizeof(uint32_t));
}

//-------------------------------------------------------------------------------------
void FpgaTarget::writeMatrixArray(uint32_t _accMemStart,
                                  uint32_t *_ramMatrix,
                                  uint32_t _ramTotalLines, uint32_t _ramTotalColumns,
                                  uint32_t _ramStartLine, uint32_t _ramStartColumn,
                                  uint32_t _numLines, uint32_t _numColumns) {
    
    assert(_ramStartLine + _numLines <= _ramTotalLines);
    assert(_ramStartColumn + _numColumns <= _ramTotalColumns);

    // TODO: test performance of liniarization vs sending each part individually on FIFO
    if (_numLines * _numColumns * sizeof(uint32_t) > io_matrix_max_size) {
        throw std::runtime_error("Matrix too large");
    }

    uint32_t io_matrix_i = 0;

    for (uint32_t i = _ramStartLine; i < _ramStartLine + _numLines; i++ ) {
        for (uint32_t j = _ramStartColumn; j < _ramStartColumn + _numColumns; j++) {
            io_matrix[io_matrix_i++] = _ramMatrix[i * _ramTotalColumns + j];
        }
    }

    sendMatrixArray(io_matrix_raw_position, _accMemStart, _numLines, _numColumns);

#ifndef NDEBUG
    for (uint32_t i = 0; i < io_matrix_i; i++) {
        io_matrix[i] = 0;
    }
#endif
}

//-------------------------------------------------------------------------------------
void FpgaTarget::sendMatrixArray(uint32_t _rawRamAddress, uint32_t _accAddress, uint32_t _numLines, uint32_t _numColumns) {
    uint32_t _transferLength = _numLines * _numColumns;

    fmt::println("Sending matrix array from ram address 0x{:08x} of dimension {:4}x{:<4} to 0x{:08x}",
            _rawRamAddress, _numLines, _numColumns, _accAddress);


    DMA_write(DMA_POINTER_CONSTANT, _rawRamAddress, _transferLength * sizeof(uint32_t));

    printf("Status reg: %d\n", readRegister(0x10));
    printf("FIFO data in count: %d\n", readRegister(36));
    printf("FIFO data out count: %d\n", readRegister(40));

    writeInstruction(arch.INSTR_send_matrix_array);
    writeInstruction(arch.INSTR_nop);
    writeInstruction(0, _accAddress);
    writeInstruction(arch.INSTR_nop);
    writeInstruction(0, _numLines);
    writeInstruction(arch.INSTR_nop);
    writeInstruction(_numColumns);
    writeInstruction(arch.INSTR_nop);
}

//-------------------------------------------------------------------------------------
void FpgaTarget::dump(const std::string &_addressString) {
    unsigned int _address = std::stoul(_addressString, nullptr, 16);
    printf("FpgaTarget.dump @%x:\n", _address);

}

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
void FpgaTarget::AXI_LITE_write(uint32_t *addr, uint32_t _value) {
    fmt::println("AXI_LITE_write: 0x{:08x}", _value);
    *((volatile unsigned *) (addr)) = _value;
}

//-------------------------------------------------------------------------------------
uint32_t FpgaTarget::AXI_LITE_read(const uint32_t *addr) {
    uint32_t return_value = 0;
    return_value = *((volatile unsigned *) (addr));
    return return_value;
}

//-------------------------------------------------------------------------------------
void FpgaTarget::AXI_LITE_set_bits(uint32_t *_addr, uint32_t _mask) {
    uint32_t _value = AXI_LITE_read(_addr);
    _value |= _mask;
    AXI_LITE_write(_addr, _value);
}

//-------------------------------------------------------------------------------------
void FpgaTarget::AXI_LITE_clear_bits(uint32_t *_addr, uint32_t _mask) {
    uint32_t _value = AXI_LITE_read(_addr);
    _value &= ~_mask;
    AXI_LITE_write(_addr, _value);
}

//-------------------------------------------------------------------------------------
void FpgaTarget::dma_mm2s_status(uint32_t *DMA_POINTER_CONSTANT) {
    uint32_t status_reg = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_MM2S_DMASR_OFFSET >> 2));
    printf("MM2S status (addr offset: 0x%x status:0x%x): ", DMA_MM2S_DMASR_OFFSET, status_reg);

    if (status_reg & 0x00000001) {
        printf("halted ");
    } else {
        printf("running ");
    }

    if (status_reg & 0x00000002) {
        printf("idle ");
    }
    if (status_reg & 0x00000008) {
        printf("SG enabled ");
    }
    if (status_reg & 0x00000010) {
        printf("DMA interrupt error ");
    }
    if (status_reg & 0x00000020) {
        printf("DMA slave error ");
    }
    if (status_reg & 0x00000040) {
        printf("DMA decode error ");
    }
    if (status_reg & 0x00001000) {
        printf("IOC interrupt request ");
    }
    if (status_reg & 0x00002000) {
        printf("Delay interrupt request ");
    }
    if (status_reg & 0x00004000) {
        printf("Error interrupt request ");
    }

    printf("\n");
}

//-------------------------------------------------------------------------------------
void FpgaTarget::dma_s2mm_status(uint32_t *DMA_POINTER_CONSTANT) {
    uint32_t status_reg = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_S2MM_DMASR_OFFSET >> 2));
    printf("S2MM status (addr offset: 0x%x status: 0x%x): ", DMA_S2MM_DMASR_OFFSET, status_reg);

    if (status_reg & 0x00000001) {
        printf("halted ");
    } else {
        printf("running ");
    }

    if (status_reg & 0x00000002) {
        printf("idle ");
    }
    if (status_reg & 0x00000008) {
        printf("SG enabled ");
    }
    if (status_reg & 0x00000010) {
        printf("DMA interrupt error ");
    }
    if (status_reg & 0x00000020) {
        printf("DMA slave error ");
    }
    if (status_reg & 0x00000040) {
        printf("DMA decode error ");
    }
    if (status_reg & 0x00001000) {
        printf("IOC interrupt request ");
    }
    if (status_reg & 0x00002000) {
        printf("Delay interrupt request ");
    }
    if (status_reg & 0x00004000) {
        printf("Error interrupt request ");
    }

    printf("\n");
}

//-------------------------------------------------------------------------------------
void FpgaTarget::print_all_registers_mm2s(uint32_t *DMA_POINTER_CONSTANT, int tag) {
    uint32_t register_read_value;

    printf("Printing all DMA mm2s registers: tag: %d \n", tag);

    register_read_value = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_MM2S_DMACR_OFFSET >> 2));
    printf("DMA mm2s: control register: %x\n", register_read_value);

    register_read_value = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_MM2S_DMASR_OFFSET >> 2));
    printf("DMA mm2s: status register: %x\n", register_read_value);

    register_read_value = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_MM2S_SA_LSB_OFFSET >> 2));
    printf("DMA mm2s: source addr lsb: %x\n", register_read_value);

    register_read_value = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_MM2S_SA_MSB_OFFSET >> 2));
    printf("DMA mm2s: source addr msb: %x\n", register_read_value);

    register_read_value = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_MM2S_LENGTH_OFFSET >> 2));
    printf("DMA mm2s: transfer length: %x\n", register_read_value);

    printf("Finished printing all DMA mm2s registers: \n");
}

//-------------------------------------------------------------------------------------
void FpgaTarget::print_all_registers_s2mm(uint32_t *DMA_POINTER_CONSTANT, int tag) {
    uint32_t register_read_value;

    printf("Printing all DMA s2mm registers: tag: %d\n", tag);

    register_read_value = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_S2MM_DMACR_OFFSET >> 2));
    printf("DMA s2mm: control register: %x\n", register_read_value);

    register_read_value = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_S2MM_DMASR_OFFSET >> 2));
    printf("DMA s2mm: status register: %x\n", register_read_value);

    register_read_value = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_S2MM_DA_LSB_OFFSET >> 2));
    printf("DMA s2mm: source addr lsb: %x\n", register_read_value);

    register_read_value = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_S2MM_DA_MSB_OFFSET >> 2));
    printf("DMA s2mm: source addr msb: %x\n", register_read_value);

    register_read_value = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_S2MM_LENGTH_OFFSET >> 2));
    printf("DMA s2mm: transfer length: %x\n", register_read_value);

    printf("Finished printing all DMA s2mm registers: \n");
}

//-------------------------------------------------------------------------------------
void FpgaTarget::dma_mm2s_wait_transfers_complete(uint32_t *DMA_POINTER_CONSTANT) {
    auto start_time = std::chrono::steady_clock::now();

    uint32_t mm2s_status = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_MM2S_DMASR_OFFSET >> 2));

    while ((mm2s_status & (1 << DMA_MM2S_DMASR_X_Idle_LOC)) == DMA_MM2S_DMASR_X_Idle_X_NOT_IDLE) {
        dma_mm2s_status(DMA_POINTER_CONSTANT);
        mm2s_status = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_MM2S_DMASR_OFFSET >> 2));

        auto current_time = std::chrono::steady_clock::now();

        if (std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count() > DMA_TRANSFER_TIMEOUT) {
            printf("Timeout: s2mm transfer not finished\n");
            exit(EXIT_FAILURE);
        }

        std::this_thread::sleep_for(30ms);
    }
}

//-------------------------------------------------------------------------------------
void FpgaTarget::dma_s2mm_wait_transfers_complete(uint32_t *DMA_POINTER_CONSTANT) {
    auto start_time = std::chrono::steady_clock::now();

    uint32_t s2mm_status = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_S2MM_DMASR_OFFSET >> 2));

    while ((s2mm_status & (1 << DMA_S2MM_DMASR_X_Idle_LOC)) == DMA_S2MM_DMASR_X_Idle_X_NOT_IDLE) {
        dma_s2mm_status(DMA_POINTER_CONSTANT);
        s2mm_status = AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_S2MM_DMASR_OFFSET >> 2));

        auto current_time = std::chrono::steady_clock::now();

        if (std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count() > DMA_TRANSFER_TIMEOUT) {
            printf("Timeout: s2mm transfer not finished\n");
            exit(EXIT_FAILURE);
        }

        std::this_thread::sleep_for(30ms);
    }
}

//-------------------------------------------------------------------------------------
void FpgaTarget::DMA_write(uint32_t *DMA_POINTER_CONSTANT, uint32_t ddr_start_addr, uint32_t transfer_length) {
    printf("Start MM2S function\n");
    AXI_LITE_write(DMA_POINTER_CONSTANT + (DMA_MM2S_DMACR_OFFSET >> 2), 0);
    dma_mm2s_status(DMA_POINTER_CONSTANT);

    AXI_LITE_write(DMA_POINTER_CONSTANT + (DMA_MM2S_DMASR_OFFSET >> 2), 0);
    dma_mm2s_status(DMA_POINTER_CONSTANT);

    printf("Writing source address\n");
    AXI_LITE_write(DMA_POINTER_CONSTANT + (DMA_MM2S_SA_LSB_OFFSET >> 2), ddr_start_addr);
    AXI_LITE_write(DMA_POINTER_CONSTANT + (DMA_MM2S_SA_MSB_OFFSET >> 2), 0x00000000);
    dma_mm2s_status(DMA_POINTER_CONSTANT);
    printf("Starting MM2S channel\n");
    AXI_LITE_write(DMA_POINTER_CONSTANT + (DMA_MM2S_DMACR_OFFSET >> 2), 0x0001);
    dma_mm2s_status(DMA_POINTER_CONSTANT);
    printf("Writing MM2S transfer length\n");
    AXI_LITE_write(DMA_POINTER_CONSTANT + (DMA_MM2S_LENGTH_OFFSET >> 2), transfer_length);
    dma_mm2s_status(DMA_POINTER_CONSTANT);
    printf("Waiting for MM2S to be done\n");
    dma_mm2s_wait_transfers_complete(DMA_POINTER_CONSTANT);
    dma_mm2s_status(DMA_POINTER_CONSTANT);
    printf("End MM2S function\n");
}

//-------------------------------------------------------------------------------------
void FpgaTarget::DMA_read(uint32_t *DMA_POINTER_CONSTANT, uint32_t ddr_start_addr, uint32_t transfer_length) {
    printf("Start S2MM function\n");
    printf("Writing destination address\n");
    AXI_LITE_write(DMA_POINTER_CONSTANT + (DMA_S2MM_DA_LSB_OFFSET >> 2), ddr_start_addr);
    AXI_LITE_write(DMA_POINTER_CONSTANT + (DMA_S2MM_DA_MSB_OFFSET >> 2), 0x00000000);
    dma_s2mm_status(DMA_POINTER_CONSTANT);
    printf("Starting S2MM channel\n");
    AXI_LITE_write(DMA_POINTER_CONSTANT + (DMA_S2MM_DMACR_OFFSET >> 2), 0x0001);
    dma_s2mm_status(DMA_POINTER_CONSTANT);
    printf("Writing S2MM transfer length\n");
    AXI_LITE_write(DMA_POINTER_CONSTANT + (DMA_S2MM_LENGTH_OFFSET >> 2), transfer_length);
    dma_s2mm_status(DMA_POINTER_CONSTANT);
    printf("Waiting for S2MM to be done\n");
    dma_s2mm_wait_transfers_complete(DMA_POINTER_CONSTANT);
    dma_s2mm_status(DMA_POINTER_CONSTANT);
    printf("S2MM received: %" PRIu32 " of %" PRIu32 "\n", AXI_LITE_read(DMA_POINTER_CONSTANT + (DMA_S2MM_LENGTH_OFFSET >> 2)), transfer_length);
    printf("End S2MM function\n");
}

//-------------------------------------------------------------------------------------
void FpgaTarget::dma_reset(uint32_t *DMA_POINTER_CONSTANT) {
    printf("Resetting DMA\n");

    AXI_LITE_set_bits(DMA_POINTER_CONSTANT + (DMA_MM2S_DMACR_OFFSET >> 2), 1 << DMA_MM2S_DMACR_X_RESET_LOC);
    usleep(200 * 1000);
    AXI_LITE_clear_bits(DMA_POINTER_CONSTANT + (DMA_MM2S_DMACR_OFFSET >> 2), 1 << DMA_MM2S_DMACR_X_RESET_LOC);
    usleep(200 * 1000);

    AXI_LITE_set_bits(DMA_POINTER_CONSTANT + (DMA_S2MM_DMACR_OFFSET >> 2), 1 << DMA_S2MM_DMACR_X_RESET_LOC);
    usleep(200 * 1000);
    AXI_LITE_clear_bits(DMA_POINTER_CONSTANT + (DMA_S2MM_DMACR_OFFSET >> 2), 1 << DMA_S2MM_DMACR_X_RESET_LOC);
    usleep(200 * 1000);

    dma_mm2s_status(DMA_POINTER_CONSTANT);
    dma_s2mm_status(DMA_POINTER_CONSTANT);
}

//-------------------------------------------------------------------------------------
void FpgaTarget::print_main_mem(uint32_t *address, int32_t nr_bytes, uint32_t word_size) {
    char *p = (char *) address;

    for (int i = 0; i < nr_bytes; i++) {
        if ((i + 1) % word_size == 0) {
            for (uint32_t j = 0; j < word_size; j++) {
                printf("%02x", p[i - j]);
            }
            printf(" ");
        }

        if ((i + 1) % (word_size * 8) == 0) {
            printf("\n");
        }
    }

    printf("\n");
}

//#endif
