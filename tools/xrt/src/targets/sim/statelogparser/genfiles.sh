#!/bin/bash

SRC="${XPU_SW_PATH}/tools/xrt/src/targets/sim/statelogparser"
DST_C="${XPU_SW_PATH}/tools/xrt/src/targets/sim/statelogparser"
DST_H="${XPU_SW_PATH}/tools/xrt/include/targets/sim/statelogparser"

flex -o "${DST_C}/Lexer.gen.cpp" --header-file="${DST_H}/Lexer.gen.hpp" "${SRC}/Lexer.l"
bison -o "${DST_C}/Parser.gen.cpp" -H"${DST_H}/Parser.gen.hpp" "${SRC}/Parser.y"
