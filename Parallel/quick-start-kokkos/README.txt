# Instructions for quick start #
from https://kokkos.org/kokkos-core-wiki/get-started/quick-start.html

Should work on home system, TN Tech Warp, or Delta clusters so long as you have proper modules loaded.

TN Tech: spack load kokkos@4.7~cuda gcc@14 openmpi cmake
Delta: none

mkdir build
cd build

cmake -DCMAKE_INSTALL_PREFIX=. ..
make -j4 install
./HelloKokkos 
