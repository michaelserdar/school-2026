cmake_minimum_required(VERSION 3.22)
project(MyProject)

#include(FetchContent)
#FetchContent_Declare(
#  Kokkos
#  URL https://github.com/kokkos/kokkos/archive/refs/tags/5.0.0.zip
#)
#FetchContent_MakeAvailable(Kokkos)
find_package(Kokkos 4.2 REQUIRED CONFIG) # Find Kokkos version 4.2 or any newer version
# ...

add_executable(HelloKokkos HelloKokkos.cpp)
target_link_libraries(HelloKokkos PRIVATE Kokkos::kokkos)
#target_link_libraries(HelloKokkos Kokkos::kokkos)
