cmake_minimum_required(VERSION 2.8.2)
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



# --- PICHI --------------------------------------------------------

include_directories(include)
include_directories(include/laph)
add_subdirectory(lib)


# --- Testing -------------------------------------------------------

enable_testing()
add_subdirectory(test/unit)
add_subdirectory(test/bench)
