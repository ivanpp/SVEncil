# arm4stencil

## Use gem5

```bash
# with 2level-arm.py
#   bin is defined in 2level-arm.py, modify it first
sh scripts/sim.sh OUT_DIR

# with se.py
sh scripts/sim-sve.sh OUT_DIR BIN VEC
```

## Techinique explain

* [loop unrolling](https://en.wikipedia.org/wiki/Loop_unrolling)
* [Loop fission and fusion](https://en.wikipedia.org/wiki/Loop_fission_and_fusion)
* [Blocking (Loop nest optimization)](https://en.wikipedia.org/wiki/Loop_nest_optimization)