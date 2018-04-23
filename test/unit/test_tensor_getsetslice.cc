#include "tensor.h"

#include "gtest/gtest.h"

/*
 * Unit tests of the getSlice and setSlice methods in the tensor class,
 * implemented in TENSOR.CC
 */

using namespace pichi;
using namespace std;

namespace {

TEST(TensorGetSetSlice, OverlappingSlicesAreCompatible) {
  Tensor t(3, 2);
  cdouble data[4];
  data[0] = 0.0;
  data[1] = 1.0;
  data[2] = 2.0;
  data[3] = 3.0;
  t.setSlice({-1, -1, 0}, data);
  data[0] = 4.0;
  data[1] = 5.0;
  data[2] = 6.0;
  data[3] = 7.0;
  t.setSlice({-1, -1, 1}, data);

  cdouble data2[4];
  t.getSlice({-1, 0, -1}, data2);
  EXPECT_EQ(0.0, data2[0]);
  EXPECT_EQ(1.0, data2[1]);
  EXPECT_EQ(4.0, data2[2]);
  EXPECT_EQ(5.0, data2[3]);

  t.getSlice({-1, 1, -1}, data2);
  EXPECT_EQ(2.0, data2[0]);
  EXPECT_EQ(3.0, data2[1]);
  EXPECT_EQ(6.0, data2[2]);
  EXPECT_EQ(7.0, data2[3]);

  t.getSlice({0, -1, -1}, data2);
  EXPECT_EQ(0.0, data2[0]);
  EXPECT_EQ(2.0, data2[1]);
  EXPECT_EQ(4.0, data2[2]);
  EXPECT_EQ(6.0, data2[3]);

  t.getSlice({1, -1, -1}, data2);
  EXPECT_EQ(1.0, data2[0]);
  EXPECT_EQ(3.0, data2[1]);
  EXPECT_EQ(5.0, data2[2]);
  EXPECT_EQ(7.0, data2[3]);

}

}
