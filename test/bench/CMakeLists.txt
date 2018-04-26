
add_executable(bench1 bench1.cc)

add_executable(bench2 bench2.cc)

target_link_libraries(bench1 pichi ${ARMADILLO_LIBRARIES})
target_link_libraries(bench2 pichi ${ARMADILLO_LIBRARIES})