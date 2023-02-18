#!/bin/bash

# to figure out the impact of different vector length
OUTPUT_DIR=$1
VEC=$2
NTHREADS=$3
M5OUT_DIR="m5out"
BASH_OUT=$M5OUT_DIR/$OUTPUT_DIR/output.txt
CONFIG_DIR="configs"
MODEL_NAME="se"
BIN="../stencil-3d/bin/stencil-3domp"

# cache size for 4/8 threads (L2 is shared)
L1DS=64kB
L1IS=32kB
L2S=2048kB
GEM5_BIN="build/ARM/gem5.opt"

export OMP_NUM_THREADS=$NTHREADS

if [ $# -lt 3 ]
then
    >&2 echo "usage: srcipts/sim.sh OUTPUT_DIR VEC NTHREADS"
    >&2 echo "VEC should be 1(128)/2(256)/4(512)/8(1024)/16(2048)"
    exit 1
else
    echo "------------------------------------"
    echo "GEM5 ARM syscall emulation"
    echo
    echo "L1I: $L1IS"
    echo "L1D: $L1DS"
    echo "L2:  $L2S"
    echo
    echo "sve_vl_se: $VEC"
    echo "nthreads:  $NTHREADS"
    echo
    echo "model: $CONFIG_DIR/$MODEL_NAME.py"
    echo "m5out: $M5OUT_DIR/$OUTPUT_DIR"
    echo "bin:  $BIN"
    echo "------------------------------------"
fi

# syscall emulation, also save bash output
mkdir $M5OUT_DIR/$OUTPUT_DIR
$GEM5_BIN -d "$M5OUT_DIR/$OUTPUT_DIR" "$CONFIG_DIR/$MODEL_NAME.py" \
--param "system.cpu[:].isa[:].sve_vl_se = $VEC" --num-cpus=$NTHREADS --cpu-type=ex5_big \
--caches --l1d_size=$L1DS --l1i_size=$L1IS --l2cache --l2_size=$L2S -c "$BIN" \
| tee $BASH_OUT
# copy the configuration py file too
cp "$CONFIG_DIR/$MODEL_NAME.py" "$M5OUT_DIR/$OUTPUT_DIR/"