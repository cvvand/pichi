cmake_minimum_required(VERSION 2.8.2)
project(pichi)

set(CMAKE_CXX_STANDARD 11)


# --- Armadillo ----------------------------------------------------


find_package(Armadillo REQUIRED)
include_directories(${ARMADILLO_INCLUDE_DIRS})


# --- BLAS ---------------------------------------------------------

# find_package(BLAS REQUIRED)


# --- PICHI --------------------------------------------------------

include_directories(include lib)

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

target_link_libraries(pichi
        ${ARMADILLO_LIBRARIES})

#export(TARGETS pichi FILE pichiConfig.cmake)

# --- Benchmarks ----------------------------------------------------

add_executable(bench1 test/bench/bench1.cc)

target_link_libraries(bench1 pichi)


# --- Unit testing ---------------------------------------------------

enable_testing()

# Google test
# For information about Googletest:
#   https://github.com/google/googletest

# The project is downloaded as a part of the configuration process and built
# alongside PICHI.

# Download and unpack googletest at configure time
configure_file(${CMAKE_SOURCE_DIR}/CMakeLists.txt.in
        ${CMAKE_BINARY_DIR}/googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
        ${CMAKE_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

add_executable(all_ut
        test/unit/test_compute.cc
        test/unit/test_contract.cc
        test/unit/test_contract_errors.cc
        test/unit/test_contraction_storage_rules.cc
        test/unit/test_double_slice_iterator.cc
        test/unit/test_extract.cc
        test/unit/test_graph.cc
        test/unit/test_graph_split.cc
        test/unit/test_identify.cc
        test/unit/test_single_slice_iterator.cc
        test/unit/test_string_utils.cc
        test/unit/test_tensor.cc
        test/unit/test_tensor_getsetslice.cc
        test/unit/test_tensor_storage.cc
        )

target_link_libraries(all_ut gtest_main pichi)
add_test(NAME unit_tests COMMAND all_ut)
