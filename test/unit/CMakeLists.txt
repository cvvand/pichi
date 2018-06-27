
# --- Google test --------------------------------------------------------

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




# --- Tests ----------------------------------------------------------------

add_executable(all_ut
        test_compute.cc
        test_contract.cc
        test_contract_errors.cc
        test_contraction_storage_rules.cc
        test_double_slice_iterator.cc
        test_extract.cc
        test_graph.cc
        test_graph_split.cc
        test_identify.cc
        test_single_slice_iterator.cc
        test_string_utils.cc
        test_tensor.cc
        test_tensor_getsetslice.cc
        test_tensor_storage.cc
        )
target_link_libraries(all_ut
        gtest_main
        pichi
        ${ARMADILLO_LIBRARIES}
        #${BLAS_LIBRARIES}
        )
add_test(NAME unit_tests COMMAND all_ut)