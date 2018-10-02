cmake_minimum_required(VERSION 3.0.0)
project(pichi)

set(CMAKE_CXX_STANDARD 11)


# --- Armadillo ----------------------------------------------------


set(ARMA_PATH "default" CACHE STRING "Path to Armadillo")
if(ARMA_PATH STREQUAL "default")
  # If Armadillo is in standard position, just load it
  find_package(Armadillo REQUIRED)
  include_directories(${ARMADILLO_INCLUDE_DIRS})
else()
  # Set custom include and link directories
  include_directories(${ARMA_PATH}/include)
  link_directories(${ARMA_PATH}/lib)
  # Set the correct value of the Armadillo libs
  set(ARMADILLO_LIBRARIES armadillo)
endif()


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

target_include_directories(pichi PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE lib)

target_link_libraries(pichi
        ${ARMADILLO_LIBRARIES})

install(TARGETS pichi EXPORT pichiConfig
        ARCHIVE DESTINATION lib
        )
install(DIRECTORY include/ DESTINATION include)
install(EXPORT pichiConfig DESTINATION share/cmake)

# --- Benchmarks ----------------------------------------------------
option(MAKE_BENCHMARKS
        "Build benchmark tests" OFF )
if(MAKE_BENCHMARKS)
  add_executable(bench1 test/bench/bench1.cc)
  target_link_libraries(bench1 pichi)
endif()


# --- Unit testing ---------------------------------------------------

option(MAKE_UNIT_TESTS
        "Build unit tests (downloads googletest from the web" OFF )
if(MAKE_UNIT_TESTS)

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
          test/unit/test_tensor_algebra.cc
          test/unit/test_tensor_getsetslice.cc
          test/unit/test_tensor_storage.cc
          )

  target_link_libraries(all_ut gtest_main pichi)
  add_test(NAME unit_tests COMMAND all_ut)

endif()
