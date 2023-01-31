#!/bin/bash
echo "Generate XPU asm gramar ..."
java -jar $XPU_HOME/lib/antlr-*.jar -visitor Asm.g4 