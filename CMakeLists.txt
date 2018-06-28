cmake_minimum_required(VERSION 2.8.2)
project(pichi)

set(CMAKE_CXX_STANDARD 11)


# --- Armadillo ----------------------------------------------------


find_package(Armadillo REQUIRED)
include_directories(${ARMADILLO_INCLUDE_DIRS})


# --- BLAS ---------------------------------------------------------

# find_package(BLAS REQUIRED)


# --- PICHI --------------------------------------------------------

include_directories(include)

# Collect all the code into a library
add_library(pichi
        lib/contraction.cc
        lib/diagrams.cc
        lib/double_slice_iterator.cc
        lib/graph.cc
        lib/single_slice_iterator.cc
        lib/string_utils.cc
        lib/tensor.cc
        )

target_include_directories(pichi PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE lib)

target_link_libraries(pichi
        ${ARMADILLO_LIBRARIES})

export(TARGETS pichi FILE pichiConfig.cmake)


# --- Testing -------------------------------------------------------

enable_testing()
add_subdirectory(test/unit)
add_subdirectory(test/bench)
