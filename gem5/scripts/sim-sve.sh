#!/bin/bash

OUTPUT_DIR=$1
BIN=$2
VEC=$3
M5OUT_DIR="m5out"
BASH_OUT=$M5OUT_DIR/$OUTPUT_DIR/output.txt
CONFIG_DIR="configs"
MODEL_NAME="se"
BIN_DIR="../stencil-3d/bin"

GEM5_BIN="build/ARM/gem5.opt"

if [ $# -eq 0 ]
then
    >&2 echo "usage: srcipts/sim.sh OUTPUT_DIR BIN VEC"
    >&2 echo "VEC should be 1(128)/2(256)/4(512)/8(1024)/16(2048)"
    exit 1
else
    echo "------------------------------------"
    echo "GEM5 ARM syscall emulation"
    echo
    echo "model: $CONFIG_DIR/$MODEL_NAME.py"
    echo "m5out: $M5OUT_DIR/$OUTPUT_DIR"
    echo "bin:  $BIN_DIR/$BIN"
    echo "------------------------------------"
fi

# syscall emulation, also save bash output
$GEM5_BIN -d "$M5OUT_DIR/$OUTPUT_DIR" "$CONFIG_DIR/$MODEL_NAME.py" \
--param "system.cpu[:].isa[:].sve_vl_se = $VEC" --cpu-type=ex5_big \
--caches --l1d_size=64kB --l1i_size=32kB --l2cache --l2_size=256kB -c "$BIN_DIR/$BIN" \
| tee $BASH_OUT
# copy the configuration py file too
cp "$CONFIG_DIR/$MODEL_NAME.py" "$M5OUT_DIR/$OUTPUT_DIR/"