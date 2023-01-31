//
// Created by alex13dumi on 26.12.2022.
//

#ifndef XPU_SIMULATOR_CONSTANTS_HPP
#define XPU_SIMULATOR_CONSTANTS_HPP

#include "xsi_loader.h"

namespace constants
{
    /* Verilog logic values are stored bit packed encoded in two bits
     * one bit put into aVal and the other into bVal. The four logic
     * values are encoded as {aVal, bVal}: 0=00, 1=10, X=11, Z=01
     * So, a 8 bit logic value 0Z11011X is stored as below :
     * (in hexadecimal) aVal = 00000037 bVal=00000041 */
    inline constexpr s_xsi_vlog_logicval zero_val = {0X00000000, 0X00000000};
    inline constexpr s_xsi_vlog_logicval one_val  = {0X00000001, 0X00000000};
}

#endif //XPU_SIMULATOR_CONSTANTS_HPP
