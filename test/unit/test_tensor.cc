#include "gtest/gtest.h"
#include "tensor.h"

/*
 * Unit tests of the tensor class, implemented in TENSOR.CC
 */

using namespace pichi;
using namespace std;

namespace {


TEST(TensorDefault, DefaultDimension2x2) {
  Tensor t;
  ASSERT_EQ(t.rank(), 2);
  EXPECT_EQ(t.size(), 2);
}

TEST(TensorDefault, DefaultInitialData) {
  Tensor t;
  cdouble data[4];
  t.getSlice({-1,-1}, data);
  EXPECT_EQ(data[0] , 0.0);
  EXPECT_EQ(data[1] , 0.0);
  EXPECT_EQ(data[2] , 0.0);
  EXPECT_EQ(data[3] , 0.0);
}

TEST(TensorConstructor, DimensionsOfTensor) {
  Tensor t(3,4);
  ASSERT_EQ(3, t.rank());
  EXPECT_EQ(4, t.size());
}

TEST(TensorConstructor, InitialDataOfTensor) {
  Tensor t(2,3);
  cdouble data[9];
  t.getSlice({-1,-1}, data);
  for (int i = 0; i < 9; ++i)
    EXPECT_EQ(data[i],0.0);
}




TEST(TensorCopyConstructor, CopyTensorDimension) {
  Tensor t(3,5);
  Tensor t2(t);
  ASSERT_EQ(t2.rank(), 3);
  EXPECT_EQ(t2.size(), 5);
}

TEST(TensorCopyConstructor, Copy2DModifiedTensorData) {
  Tensor t(2,2);
  cdouble data[4];
  data[0] = 1.5; data[1] = 3.0; data[2] = -1.2; data[3] = 0.6;
  t.setSlice({-1,-1},data);
  Tensor t2(t);
  cdouble data2[4];
  t2.getSlice({-1,-1},data2);
  EXPECT_EQ(1.5, data2[0]);
  EXPECT_EQ(3.0, data2[1]);
  EXPECT_EQ(-1.2, data2[2]);
  EXPECT_EQ(0.6, data2[3]);
}

TEST(TensorCopyConstructor, Copy3DModifiedTensorData) {
  Tensor t(3,2);
  cdouble data[8];
  data[0] = 1.5; data[1] = 3.0; data[2] = -1.2;
  data[3] = 0.6; data[4] = -3.3; data[5] = 4.1;
  data[6] = 1.4; data[7] = 3.4;
  t.setSlice({-1,-1,0},data);
  t.setSlice({-1,-1,1},data+4);
  Tensor t2(t);
  cdouble data2[4];
  t2.getSlice({-1,-1,0},data2);
  EXPECT_EQ(1.5, data2[0]);
  EXPECT_EQ(3.0, data2[1]);
  EXPECT_EQ(-1.2, data2[2]);
  EXPECT_EQ(0.6, data2[3]);
  t2.getSlice({-1,-1,1},data2);
  EXPECT_EQ(-3.3, data2[0]);
  EXPECT_EQ(4.1, data2[1]);
  EXPECT_EQ(1.4, data2[2]);
  EXPECT_EQ(3.4, data2[3]);
}


TEST(TensorMoveConstructor, Move2DModifiedTensor) {
  Tensor t(2,2);
  cdouble data[4];
  data[0] = 1.5; data[1] = 3.0; data[2] = -1.2; data[3] = 0.6;
  t.setSlice({-1,-1},data);
  Tensor t2 = move(t);

  ASSERT_EQ(2, t2.rank());
  EXPECT_EQ(2, t2.size());

  cdouble data2[4];
  t2.getSlice({-1,-1},data2);
  EXPECT_EQ(1.5, data2[0]);
  EXPECT_EQ(3.0, data2[1]);
  EXPECT_EQ(-1.2, data2[2]);
  EXPECT_EQ(0.6, data2[3]);

  ASSERT_EQ(0, t.rank());
}

TEST(TensorMoveConstructor, Move3DModifiedTensor) {
  Tensor t(3,2);
  cdouble data[8];
  data[0] = 1.5; data[1] = 3.0; data[2] = -1.2;
  data[3] = 0.6; data[4] = -3.3; data[5] = 4.1;
  data[6] = 1.4; data[7] = 3.4;
  t.setSlice({-1,-1,0},data);
  t.setSlice({-1,-1,1},data+4);
  Tensor t2 = move(t);

  ASSERT_EQ(3, t2.rank());
  EXPECT_EQ(2, t2.size());

  cdouble data2[4];
  t2.getSlice({-1,-1,0},data2);
  EXPECT_EQ(1.5, data2[0]);
  EXPECT_EQ(3.0, data2[1]);
  EXPECT_EQ(-1.2, data2[2]);
  EXPECT_EQ(0.6, data2[3]);
  t2.getSlice({-1,-1,1},data2);
  EXPECT_EQ(-3.3, data2[0]);
  EXPECT_EQ(4.1, data2[1]);
  EXPECT_EQ(1.4, data2[2]);
  EXPECT_EQ(3.4, data2[3]);

  ASSERT_EQ(0, t.rank());

}



TEST(TensorMoveAssignment, MoveAssign2DModifiedTensor) {
  Tensor t(2,2);
  Tensor t2(2,4);
  cdouble data[4];
  data[0] = 1.5; data[1] = 3.0; data[2] = -1.2; data[3] = 0.6;
  t.setSlice({-1,-1},data);
  t2 = move(t);

  ASSERT_EQ(2, t2.rank());
  EXPECT_EQ(2, t2.size());

  cdouble data2[4];
  t2.getSlice({-1,-1},data2);
  EXPECT_EQ(1.5, data2[0]);
  EXPECT_EQ(3.0, data2[1]);
  EXPECT_EQ(-1.2, data2[2]);
  EXPECT_EQ(0.6, data2[3]);

  ASSERT_EQ(0, t.rank());

}

TEST(TensorMoveAssignment, MoveAssign3DModifiedTensor) {
  Tensor t(3,2);
  Tensor t2(2,4);
  cdouble data[8];
  data[0] = 1.5; data[1] = 3.0; data[2] = -1.2;
  data[3] = 0.6; data[4] = -3.3; data[5] = 4.1;
  data[6] = 1.4; data[7] = 3.4;
  t.setSlice({-1,-1,0},data);
  t.setSlice({-1,-1,1},data+4);
  t2 = move(t);

  ASSERT_EQ(3, t2.rank());
  EXPECT_EQ(2, t2.size());

  cdouble data2[4];
  t2.getSlice({-1,-1,0},data2);
  EXPECT_EQ(1.5, data2[0]);
  EXPECT_EQ(3.0, data2[1]);
  EXPECT_EQ(-1.2, data2[2]);
  EXPECT_EQ(0.6, data2[3]);
  t2.getSlice({-1,-1,1},data2);
  EXPECT_EQ(-3.3, data2[0]);
  EXPECT_EQ(4.1, data2[1]);
  EXPECT_EQ(1.4, data2[2]);
  EXPECT_EQ(3.4, data2[3]);

  ASSERT_EQ(0, t.rank());

}

TEST(TensorMoveAssignment, HandleMoveToSelf) {
  Tensor t(2,2);
  cdouble data[4];
  data[0] = 1.5; data[1] = 3.0; data[2] = -1.2; data[3] = 0.6;
  t.setSlice({-1,-1},data);
  t = move(t);

  ASSERT_EQ(2, t.rank());
  EXPECT_EQ(2, t.size());

  cdouble data2[4];
  t.getSlice({-1,-1},data2);
  EXPECT_EQ(1.5, data2[0]);
  EXPECT_EQ(3.0, data2[1]);
  EXPECT_EQ(-1.2, data2[2]);
  EXPECT_EQ(0.6, data2[3]);

}



TEST(TensorCopyAssignment, CopyAssign2DModifiedData) {
  Tensor t(2,2);
  cdouble data[4];
  data[0] = 1.5; data[1] = 3.0; data[2] = -1.2; data[3] = 0.6;
  t.setSlice({-1,-1},data);
  Tensor t2(2,5);

  t2 = t;

  ASSERT_EQ(2, t2.rank());
  EXPECT_EQ(2, t2.size());

  cdouble data2[4];
  t2.getSlice({-1,-1},data2);
  EXPECT_EQ(1.5, data2[0]);
  EXPECT_EQ(3.0, data2[1]);
  EXPECT_EQ(-1.2, data2[2]);
  EXPECT_EQ(0.6, data2[3]);

  ASSERT_EQ(2, t.rank());
  EXPECT_EQ(2, t.size());

  t.getSlice({-1,-1},data2);
  EXPECT_EQ(1.5, data2[0]);
  EXPECT_EQ(3.0, data2[1]);
  EXPECT_EQ(-1.2, data2[2]);
  EXPECT_EQ(0.6, data2[3]);

}

TEST(TensorCopyAssignment, CopyAssign3DModifiedData) {
  Tensor t(3,2);
  cdouble data[8];
  data[0] = 1.5; data[1] = 3.0; data[2] = -1.2;
  data[3] = 0.6; data[4] = -3.3; data[5] = 4.1;
  data[6] = 1.4; data[7] = 3.4;
  t.setSlice({-1,-1,0},data);
  t.setSlice({-1,-1,1},data+4);
  Tensor t2(2,7);
  t2 = t;

  ASSERT_EQ(3, t2.rank());
  EXPECT_EQ(2, t2.size());

  cdouble data2[4];
  t2.getSlice({-1,-1,0},data2);
  EXPECT_EQ(1.5, data2[0]);
  EXPECT_EQ(3.0, data2[1]);
  EXPECT_EQ(-1.2, data2[2]);
  EXPECT_EQ(0.6, data2[3]);
  t2.getSlice({-1,-1,1},data2);
  EXPECT_EQ(-3.3, data2[0]);
  EXPECT_EQ(4.1, data2[1]);
  EXPECT_EQ(1.4, data2[2]);
  EXPECT_EQ(3.4, data2[3]);

  ASSERT_EQ(3, t.rank());
  EXPECT_EQ(2, t.size());

  t.getSlice({-1,-1,0},data2);
  EXPECT_EQ(1.5, data2[0]);
  EXPECT_EQ(3.0, data2[1]);
  EXPECT_EQ(-1.2, data2[2]);
  EXPECT_EQ(0.6, data2[3]);
  t.getSlice({-1,-1,1},data2);
  EXPECT_EQ(-3.3, data2[0]);
  EXPECT_EQ(4.1, data2[1]);
  EXPECT_EQ(1.4, data2[2]);
  EXPECT_EQ(3.4, data2[3]);

}



TEST(TensorGetSetSlice, OverlappingSlicesAreCompatible) {
  Tensor t(3,2);
  cdouble data[4];
  data[0] = 0.0; data[1] = 1.0; data[2] = 2.0; data[3] = 3.0;
  t.setSlice({-1,-1,0}, data);
  data[0] = 4.0; data[1] = 5.0; data[2] = 6.0; data[3] = 7.0;
  t.setSlice({-1,-1,1}, data);

  cdouble data2[4];
  t.getSlice({-1,0,-1},data2);
  EXPECT_EQ(0.0, data2[0]);
  EXPECT_EQ(1.0, data2[1]);
  EXPECT_EQ(4.0, data2[2]);
  EXPECT_EQ(5.0, data2[3]);

  t.getSlice({-1,1,-1},data2);
  EXPECT_EQ(2.0, data2[0]);
  EXPECT_EQ(3.0, data2[1]);
  EXPECT_EQ(6.0, data2[2]);
  EXPECT_EQ(7.0, data2[3]);

  t.getSlice({0,-1,-1},data2);
  EXPECT_EQ(0.0, data2[0]);
  EXPECT_EQ(2.0, data2[1]);
  EXPECT_EQ(4.0, data2[2]);
  EXPECT_EQ(6.0, data2[3]);

  t.getSlice({1,-1,-1},data2);
  EXPECT_EQ(1.0, data2[0]);
  EXPECT_EQ(3.0, data2[1]);
  EXPECT_EQ(5.0, data2[2]);
  EXPECT_EQ(7.0, data2[3]);

}



}


