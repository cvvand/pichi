
add_executable(bench1 bench1.cc)

target_link_libraries(bench1 pichi ${ARMADILLO_LIBRARIES})