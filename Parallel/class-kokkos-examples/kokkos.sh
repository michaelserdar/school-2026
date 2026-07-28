#!/bin/bash
#SBATCH --account=csc4760-001-2026s
#SBATCH --cpus-per-task=2

# load library
spack load kokkos@4.7~cuda gcc@14.3.0 openmpi cmake
mkdir -p build && cd build
cmake -DCMAKE_C_COMPILER=mpicc -DCMAKE_CXX_COMPILER=mpicxx ..
make
#srun --account=csc4760-001-2026s  --cpus-per-task=2 ./HelloKokkos
./HelloKokkos
cd ..
