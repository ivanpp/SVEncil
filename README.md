# SVEncil

Co-design of SVE Enabled Hardware and Stencil Algorithm

## Use gem5

```bash
# sim.sh: 2level-arm.py with different binaries
#   bin is defined in 2level-arm.py, modify it first
sh scripts/sim.sh OUT_DIR

# sim-sve.sh: se.py with different binary, vec_len
sh scripts/sim-sve.sh OUT_DIR BIN VEC

# sim-veclen.sh: se.py with different vec_len[, binary, cache_size]
#   bin, cache size: defiend in sim-veclen.sh
sh scripts/sim-veclen.sh OUT_DIR VEC

# sim-ompvec.sh: se.py with different #cores(omp threads), vec_len
#   bin, cache size: defiend in sim-ompvec.sh
sh scripts/sim-ompvec.sh OUT_DIR VEC NTHREADS
```

## Techinique explain

* [loop unrolling](https://en.wikipedia.org/wiki/Loop_unrolling)
* [Loop fission and fusion](https://en.wikipedia.org/wiki/Loop_fission_and_fusion)
* [Blocking (Loop nest optimization)](https://en.wikipedia.org/wiki/Loop_nest_optimization)