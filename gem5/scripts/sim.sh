#!/bin/bash

M5OUT_DIR="m5out"
OUTPUT_DIR=$1
BASH_OUT=$M5OUT_DIR/$OUTPUT_DIR/output.txt
CONFIG_DIR="configs"
MODEL_NAME="2level-arm"

GEM5_BIN="build/ARM/gem5.opt"

if [ $# -eq 0 ]
then
    >&2 echo "usage: srcipts/sim.sh OUTPUT_DIR"
    exit 1
else
    echo "------------------------------------"
    echo "GEM5 ARM syscall emulation"
    echo
    echo "model: $CONFIG_DIR/$MODEL_NAME.py"
    echo "m5out: $M5OUT_DIR/$OUTPUT_DIR"
    echo "------------------------------------"
fi

# syscall emulation, also save bash output
$GEM5_BIN -d "$M5OUT_DIR/$OUTPUT_DIR" "$CONFIG_DIR/$MODEL_NAME.py" | tee $BASH_OUT
# copy the configuration py file too
cp "$CONFIG_DIR/$MODEL_NAME.py" "$M5OUT_DIR/$OUTPUT_DIR/"
