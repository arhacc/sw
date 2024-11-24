#!/bin/bash

SRC="${XPU_SW_PATH}/tools/xrt/src/targets/sim/statelogparser"
DST_C="${XPU_SW_PATH}/tools/xrt/src/targets/sim/statelogparser"
DST_H="${XPU_SW_PATH}/tools/xrt/include/targets/sim/statelogparser"

if [[ "${SRC}/Lexer.l" -nt "${DST_C}/Lexer.gen.cpp" || "${SRC}/Lexer.l" -nt "${DST_H}/Lexer.gen.hpp" ]]
then
    flex -o "${DST_C}/Lexer.gen.cpp" --header-file="${DST_H}/Lexer.gen.hpp" "${SRC}/Lexer.l"
fi

if [[ "${SRC}/Parser.y" -nt "${DST_C}/Parser.gen.cpp" || "${SRC}/Parser.y" -nt "${DST_H}/Parser.gen.hpp" ]]
then
  bison -o "${DST_C}/Parser.gen.cpp" --defines="${DST_H}/Parser.gen.hpp" "${SRC}/Parser.y"
  cp "${XPU_SW_PATH}/tools/xrt/include/targets/sim/statelogparser/Parser.gen.hpp" "${XPU_SW_PATH}/tools/xrt/src/targets/sim/statelogparser/Parser.gen.hpp"
fi


