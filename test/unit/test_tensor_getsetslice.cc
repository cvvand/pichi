#include "pichi/tensor.h"

#include "gtest/gtest.h"

/*
 * Unit tests of the getSlice and setSlice methods in the tensor class,
 * implemented in TENSOR.CC
 */

using namespace pichi;
using namespace std;

namespace {

TEST(TensorGetSetSlice, SetScalarValue) {
  Tensor t;
  cdouble data[1] = {2.5};
  t.setSlice({0},data);
  cdouble data2[1];
  EXPECT_FALSE(t.getSlice({0}, data2));
  EXPECT_EQ(2.5, data2[0]);
}

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
  EXPECT_FALSE(t.getSlice({-1, 0, -1}, data2));
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

TEST(TensorGetSetSlice, ReturnTransposeSliceOfRank2Tensor) {
  Tensor t(2,2);
  cdouble data[4] = {0.0,1.0,2.0,3.0};
  t.setSlice({-1,-1}, data);
  t.setStorage({1,0});
  cdouble data2[4];
  EXPECT_TRUE(t.getSlice({-1,-1},data2));
  EXPECT_EQ(0.0, data2[0]);
  EXPECT_EQ(2.0, data2[1]);
  EXPECT_EQ(1.0, data2[2]);
  EXPECT_EQ(3.0, data2[3]);
}

TEST(TensorGetSetSlice, ReturnTransposeSliceOfRank3Tensor) {
  Tensor t(3,2);
  cdouble data[8] = {0.0,1.0,2.0,3.0,4.0,5.0,6.0,7.0};
  t.setSlice({-1,-1,0}, data);
  t.setSlice({-1,-1,1}, data+4);
  t.setStorage({2,0,1});
  cdouble data2[4];
  EXPECT_TRUE(t.getSlice({-1,0,-1},data2));
  EXPECT_EQ(0.0, data2[0]);
  EXPECT_EQ(4.0, data2[1]);
  EXPECT_EQ(1.0, data2[2]);
  EXPECT_EQ(5.0, data2[3]);

  EXPECT_TRUE(t.getSlice({-1,1,-1},data2));
  EXPECT_EQ(2.0, data2[0]);
  EXPECT_EQ(6.0, data2[1]);
  EXPECT_EQ(3.0, data2[2]);
  EXPECT_EQ(7.0, data2[3]);
}

TEST(TensorGetSetSlice, SetNonTransposeSliceOfDecreasingStorageInRank2Tensor) {
  Tensor t(2,2,{1,0});
  cdouble data[4] {0.0,1.0,2.0,3.0};
  t.setSlice({-1,-1}, data);
  t.setStorage({0,1});
  cdouble data2[4];
  EXPECT_FALSE(t.getSlice({-1,-1},data2));
  EXPECT_EQ(data2[0], 0.0);
  EXPECT_EQ(data2[1], 1.0);
  EXPECT_EQ(data2[2], 2.0);
  EXPECT_EQ(data2[3], 3.0);
}

TEST(TensorGetSetSlice, SetTransposeSliceOfDecreasingStorageInRank2Tensor) {
  Tensor t(2,2,{1,0});
  cdouble data[4] {0.0,2.0,1.0,3.0};
  t.setSlice({-1,-1}, data, true);
  t.setStorage({0,1});
  cdouble data2[4];
  EXPECT_FALSE(t.getSlice({-1,-1},data2));
  EXPECT_EQ(data2[0], 0.0);
  EXPECT_EQ(data2[1], 1.0);
  EXPECT_EQ(data2[2], 2.0);
  EXPECT_EQ(data2[3], 3.0);
}

TEST(TensorGetSetSlice, SetTransposeSliceOfIncreasingStorageInRank2Tensor) {
  Tensor t(2,2);
  cdouble data[4] {0.0,2.0,1.0,3.0};
  t.setSlice({-1,-1}, data, true);
  cdouble data2[4];
  EXPECT_FALSE(t.getSlice({-1,-1},data2));
  EXPECT_EQ(data2[0], 0.0);
  EXPECT_EQ(data2[1], 1.0);
  EXPECT_EQ(data2[2], 2.0);
  EXPECT_EQ(data2[3], 3.0);
}

TEST(TensorGetSetSlice, SetNonTransposeSliceOfDecreasingStorageInRank3Tensor) {
  Tensor t(3,2,{2,0,1});
  cdouble data[8] {0.0,1.0,4.0,5.0,2.0,3.0,6.0,7.0};
  t.setSlice({-1,0,-1}, data);
  t.setSlice({-1,1,-1}, data+4);
  t.setStorage({0,1,2});
  cdouble data2[4];
  EXPECT_FALSE(t.getSlice({-1,0,-1},data2));
  EXPECT_EQ(data2[0], 0.0);
  EXPECT_EQ(data2[1], 1.0);
  EXPECT_EQ(data2[2], 4.0);
  EXPECT_EQ(data2[3], 5.0);

  EXPECT_FALSE(t.getSlice({-1,1,-1},data2));
  EXPECT_EQ(data2[0], 2.0);
  EXPECT_EQ(data2[1], 3.0);
  EXPECT_EQ(data2[2], 6.0);
  EXPECT_EQ(data2[3], 7.0);
}

TEST(TensorGetSetSlice, SetTransposeSliceOfDecreasingStorageInRank3Tensor) {
  Tensor t(3,2,{2,0,1});
  cdouble data[8] {0.0,4.0,1.0,5.0,2.0,6.0,3.0,7.0};
  t.setSlice({-1,0,-1}, data, true);
  t.setSlice({-1,1,-1}, data+4, true);
  t.setStorage({0,1,2});
  cdouble data2[4];
  EXPECT_FALSE(t.getSlice({-1,0,-1},data2));
  EXPECT_EQ(data2[0], 0.0);
  EXPECT_EQ(data2[1], 1.0);
  EXPECT_EQ(data2[2], 4.0);
  EXPECT_EQ(data2[3], 5.0);

  EXPECT_FALSE(t.getSlice({-1,1,-1},data2));
  EXPECT_EQ(data2[0], 2.0);
  EXPECT_EQ(data2[1], 3.0);
  EXPECT_EQ(data2[2], 6.0);
  EXPECT_EQ(data2[3], 7.0);
}

TEST(TensorGetSetSlice, SetTransposeSliceOfIncreasingStorageInRank3Tensor) {
  Tensor t(3,2);
  cdouble data[8] {0.0,4.0,1.0,5.0,2.0,6.0,3.0,7.0};
  t.setSlice({-1,0,-1}, data, true);
  t.setSlice({-1,1,-1}, data+4, true);
  cdouble data2[4];
  EXPECT_FALSE(t.getSlice({-1,0,-1},data2));
  EXPECT_EQ(data2[0], 0.0);
  EXPECT_EQ(data2[1], 1.0);
  EXPECT_EQ(data2[2], 4.0);
  EXPECT_EQ(data2[3], 5.0);

  EXPECT_FALSE(t.getSlice({-1,1,-1},data2));
  EXPECT_EQ(data2[0], 2.0);
  EXPECT_EQ(data2[1], 3.0);
  EXPECT_EQ(data2[2], 6.0);
  EXPECT_EQ(data2[3], 7.0);
}

TEST(TensorGetSetSlice, GetSliceErrorOnInvalidIndex) {
  Tensor t(3,64);
  cdouble data[64*64];
  EXPECT_THROW(t.getSlice({-1,-1,-1}, data), invalid_argument);
  EXPECT_THROW(t.getSlice({-1,-1,64}, data), invalid_argument);
}

TEST(TensorGetSetSlice, GetSliceErrorOnWrongNumberOfEntries) {
  Tensor t(3,64);
  cdouble data[64*64];
  EXPECT_THROW(t.getSlice({-1,-1,0,1}, data), invalid_argument);
}

TEST(TensorGetSetSlice, GetSliceErrorOnTooFewRunningIndices) {
  Tensor t(3,64);
  cdouble data[64*64];
  EXPECT_THROW(t.getSlice({-1,0,1}, data), invalid_argument);
}

TEST(TensorGetSetSlice, SetSliceErrorOnInvalidIndex) {
  Tensor t(3,64);
  cdouble data[64*64];
  EXPECT_THROW(t.setSlice({-1,-1,-1}, data), invalid_argument);
  EXPECT_THROW(t.setSlice({-1,-1,64}, data), invalid_argument);
}

TEST(TensorGetSetSlice, SetSliceErrorOnTooFewRunningIndices) {
  Tensor t(3,64);
  cdouble data[64*64];
  EXPECT_THROW(t.setSlice({-1,0,1}, data), invalid_argument);
}

TEST(TensorGetSetSlice, SetSliceErrorOnWrongNumberOfEntries) {
  Tensor t(3,64);
  cdouble data[64*64];
  EXPECT_THROW(t.setSlice({-1,-1,0,1}, data), invalid_argument);
}

}
