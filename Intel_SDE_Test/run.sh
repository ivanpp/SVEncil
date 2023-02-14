export OMP_NUM_THREADS=4
gcc -O -DNTIMES=2 -DSTREAM_ARRAY_SIZE=100000000 -fopenmp stencil-3d.c -o stencil-3d.100M
../../sde-external-9.14.0-2022-10-25-lin/sde64 -skx -i -global_region -omix output -- ./stencil-3d.100M
