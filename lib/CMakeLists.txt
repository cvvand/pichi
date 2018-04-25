
# Collect all the code into a library that can be linked to the executable
add_library(pichi
        contraction.cc
        double_slice_iterator.cc
        single_slice_iterator.cc
        tensor.cc
        )