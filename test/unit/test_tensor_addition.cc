#include "gtest/gtest.h"
#include "pichi/tensor.h"

/*
 * Unit tests of the addition operation in the tensor class, implemented in
 * TENSOR.CC
 */

using namespace pichi;
using namespace std;

namespace {

TEST(TensorAdd, DefaultTensors) {
  Tensor t1,t2;
  Tensor t3 = t1+t2;
  ASSERT_EQ(0, t3.getRank());
  ASSERT_EQ(1, t3.getSize());
  cdouble data[1]; t3.getSlice({0},data);
  EXPECT_EQ(0.0, data[0]);
}

TEST(TensorAdd, NonDefaultScalars) {
  Tensor t1,t2;
  cdouble data[1]; data[0] = 1.0;
  t1.setSlice({0},data);
  data[0] = 2.0;
  t2.setSlice({0},data);
  Tensor t3 = t1+t2;
  ASSERT_EQ(0, t3.getRank());
  ASSERT_EQ(1, t3.getSize());
  t3.getSlice({0},data);
  EXPECT_EQ(3.0, data[0]);
}

TEST(TensorAdd, RankTwoTensors) {
  Tensor t1(2,2);
  Tensor t2(2,2);
  cdouble data[4] = {1.0,2.0,3.0,4.0};
  t1.setSlice({-1,-1},data);
  data[0] = 2.0; data[1] = -1.0; data[2] = -3.0; data[3] = 1.0;
  t2.setSlice({-1,-1},data);
  Tensor t3 = t1+t2;
  ASSERT_EQ(2, t3.getRank());
  ASSERT_EQ(2, t3.getSize());
  t3.getSlice({-1,-1},data);
  EXPECT_EQ(3.0, data[0]);
  EXPECT_EQ(1.0, data[1]);
  EXPECT_EQ(0.0, data[2]);
  EXPECT_EQ(5.0, data[3]);
}

TEST(TensorAdd, RankThreeTensors) {
  Tensor t1(3,2);
  Tensor t2(3,2);

  cdouble data[4] = {1.0,2.0,3.0,4.0};
  t1.setSlice({-1,-1,0},data);
  data[0] = 2.0; data[1] = -1.0; data[2] = -3.0; data[3] = 1.0;
  t1.setSlice({-1,-1,1},data);

  data[0] = 3.0; data[1] = -4.0; data[2] = 1.0; data[3] = 1.0;
  t2.setSlice({-1,-1,0},data);
  data[0] = 1.0; data[1] = 0.0; data[2] = 6.0; data[3] = -2.0;
  t2.setSlice({-1,-1,1},data);

  Tensor t3 = t1+t2;
  ASSERT_EQ(3, t3.getRank());
  ASSERT_EQ(2, t3.getSize());

  t3.getSlice({-1,-1,0},data);
  EXPECT_EQ(4.0, data[0]);
  EXPECT_EQ(-2.0, data[1]);
  EXPECT_EQ(4.0, data[2]);
  EXPECT_EQ(5.0, data[3]);

  t3.getSlice({-1,-1,1},data);
  EXPECT_EQ(3.0, data[0]);
  EXPECT_EQ(-1.0, data[1]);
  EXPECT_EQ(3.0, data[2]);
  EXPECT_EQ(-1.0, data[3]);
}

TEST(TensorAdd, RankTwoTensorsStorageMismatch) {
  Tensor t1(2,2);
  Tensor t2(2,2,{1,0});
  cdouble data[4] = {1.0,2.0,3.0,4.0};
  t1.setSlice({-1,-1},data);
  data[0] = 2.0; data[1] = -1.0; data[2] = -3.0; data[3] = 1.0;
  t2.setSlice({-1,-1},data);
  Tensor t3 = t1+t2;
  ASSERT_EQ(2, t3.getRank());
  ASSERT_EQ(2, t3.getSize());
  t3.getSlice({-1,-1},data);
  EXPECT_EQ(3.0, data[0]);
  EXPECT_EQ(1.0, data[1]);
  EXPECT_EQ(0.0, data[2]);
  EXPECT_EQ(5.0, data[3]);
}

TEST(TensorAdd, ErrorOnNonEqualRank) {
  Tensor t1(3,2);
  Tensor t2(2,2);
  EXPECT_THROW(t1+t2, invalid_argument);
}

TEST(TensorAdd, ErrorOnNonEqualSize) {
  Tensor t1(3,2);
  Tensor t2(3,4);
  EXPECT_THROW(t1+t2, invalid_argument);
}

}
