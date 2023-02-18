#!/bin/bash

# to figure out the impact of different vector length
OUTPUT_DIR=$1
VEC=$2
M5OUT_DIR="m5out"
BASH_OUT=$M5OUT_DIR/$OUTPUT_DIR/output.txt
CONFIG_DIR="configs"
MODEL_NAME="se"
BIN="../stencil-3d/bin/stencil-3daO3"

# cache size
#L1S=64kB
L1S=32kB
#L2S=256kB
L2S=64kB
GEM5_BIN="build/ARM/gem5.opt"

if [ $# -lt 2 ]
then
    >&2 echo "usage: srcipts/sim.sh OUTPUT_DIR VEC"
    >&2 echo "VEC should be 1(128)/2(256)/4(512)/8(1024)/16(2048)"
    exit 1
else
    echo "------------------------------------"
    echo "GEM5 ARM syscall emulation"
    echo
    echo "L1D: $L1S"
    echo "L2:  $L2S"
    echo "sve_vl_se: $VEC"
    echo
    echo "model: $CONFIG_DIR/$MODEL_NAME.py"
    echo "m5out: $M5OUT_DIR/$OUTPUT_DIR"
    echo "bin:  $BIN"
    echo "------------------------------------"
fi

# syscall emulation, also save bash output
mkdir $M5OUT_DIR/$OUTPUT_DIR
$GEM5_BIN -d "$M5OUT_DIR/$OUTPUT_DIR" "$CONFIG_DIR/$MODEL_NAME.py" \
--param "system.cpu[:].isa[:].sve_vl_se = $VEC" --cpu-type=ex5_big \
--caches --l1d_size=$L1S --l1i_size=32kB --l2cache --l2_size=$L2S -c "$BIN" \
| tee $BASH_OUT
# copy the configuration py file too
cp "$CONFIG_DIR/$MODEL_NAME.py" "$M5OUT_DIR/$OUTPUT_DIR/"