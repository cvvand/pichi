cmake_minimum_required(VERSION 2.8.2)
project(pichi)

set(CMAKE_CXX_STANDARD 11)

# --- PICHI --------------------------------------------------------

add_subdirectory(lib)
include_directories(lib)



# --- Armadillo ----------------------------------------------------

find_package(Armadillo REQUIRED)
include_directories(${ARMADILLO_INCLUDE_DIRS})



# --- Testing -------------------------------------------------------

enable_testing()
add_subdirectory(test/unit)
add_subdirectory(test/bench)