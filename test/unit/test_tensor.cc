#include "gtest/gtest.h"
#include "pichi/tensor.h"

/*
 * Unit tests of the tensor class, implemented in TENSOR.CC
 */

using namespace pichi;
using namespace std;

namespace {


TEST(TensorDefault, DefaultScalar) {
  Tensor t;
  ASSERT_EQ(t.getRank(), 0);
  EXPECT_EQ(t.getSize(), 1);
}

TEST(TensorDefault, DefaultInitialData) {
  Tensor t;
  cdouble data[1];
  t.getSlice({0}, data);
  EXPECT_EQ(data[0] , 0.0);
}

TEST(TensorConstructor, DimensionsOfTensor) {
  Tensor t(3,4);
  ASSERT_EQ(3, t.getRank());
  EXPECT_EQ(4, t.getSize());
}

TEST(TensorConstructor, InitialDataOfTensor) {
  Tensor t(2,3);
  cdouble data[9];
  t.getSlice({-1,-1}, data);
  for (int i = 0; i < 9; ++i)
    EXPECT_EQ(data[i],0.0);
}

TEST(TensorConstructor, ErrorOnRank1) {
  EXPECT_THROW(Tensor(1,64), invalid_argument);
}

TEST(TensorConstructor, ErrorOnSize1) {
  EXPECT_THROW(Tensor(2,1), invalid_argument);
}




TEST(TensorCopyConstructor, CopyTensorDimension) {
  Tensor t(3,5);
  Tensor t2(t);
  ASSERT_EQ(t2.getRank(), 3);
  EXPECT_EQ(t2.getSize(), 5);
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

  ASSERT_EQ(2, t2.getRank());
  EXPECT_EQ(2, t2.getSize());

  cdouble data2[4];
  t2.getSlice({-1,-1},data2);
  EXPECT_EQ(1.5, data2[0]);
  EXPECT_EQ(3.0, data2[1]);
  EXPECT_EQ(-1.2, data2[2]);
  EXPECT_EQ(0.6, data2[3]);

  ASSERT_EQ(0, t.getRank());
  EXPECT_EQ(1, t.getSize());
  cdouble data3[1];
  t.getSlice({0},data3);
  EXPECT_EQ(0.0, data3[0]);
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

  ASSERT_EQ(3, t2.getRank());
  EXPECT_EQ(2, t2.getSize());

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

  ASSERT_EQ(0, t.getRank());
  EXPECT_EQ(1, t.getSize());
  cdouble data3[1];
  t.getSlice({0},data3);
  EXPECT_EQ(0.0, data3[0]);

}



TEST(TensorMoveAssignment, MoveAssign2DModifiedTensor) {
  Tensor t(2,2);
  Tensor t2(2,4);
  cdouble data[4];
  data[0] = 1.5; data[1] = 3.0; data[2] = -1.2; data[3] = 0.6;
  t.setSlice({-1,-1},data);
  t2 = move(t);

  ASSERT_EQ(2, t2.getRank());
  EXPECT_EQ(2, t2.getSize());

  cdouble data2[4];
  t2.getSlice({-1,-1},data2);
  EXPECT_EQ(1.5, data2[0]);
  EXPECT_EQ(3.0, data2[1]);
  EXPECT_EQ(-1.2, data2[2]);
  EXPECT_EQ(0.6, data2[3]);

  ASSERT_EQ(0, t.getRank());
  EXPECT_EQ(1, t.getSize());
  cdouble data3[1];
  t.getSlice({0},data3);
  EXPECT_EQ(0.0, data3[0]);

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

  ASSERT_EQ(3, t2.getRank());
  EXPECT_EQ(2, t2.getSize());

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

  ASSERT_EQ(0, t.getRank());
  EXPECT_EQ(1, t.getSize());
  cdouble data3[1];
  t.getSlice({0},data3);
  EXPECT_EQ(0.0, data3[0]);

}

TEST(TensorMoveAssignment, HandleMoveToSelf) {
  Tensor t(2,2);
  cdouble data[4];
  data[0] = 1.5; data[1] = 3.0; data[2] = -1.2; data[3] = 0.6;
  t.setSlice({-1,-1},data);
  t = move(t);

  ASSERT_EQ(2, t.getRank());
  EXPECT_EQ(2, t.getSize());

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

  ASSERT_EQ(2, t2.getRank());
  EXPECT_EQ(2, t2.getSize());

  cdouble data2[4];
  t2.getSlice({-1,-1},data2);
  EXPECT_EQ(1.5, data2[0]);
  EXPECT_EQ(3.0, data2[1]);
  EXPECT_EQ(-1.2, data2[2]);
  EXPECT_EQ(0.6, data2[3]);

  ASSERT_EQ(2, t.getRank());
  EXPECT_EQ(2, t.getSize());

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
  Tensor t2;
  t2 = t;

  ASSERT_EQ(3, t2.getRank());
  EXPECT_EQ(2, t2.getSize());

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

  ASSERT_EQ(3, t.getRank());
  EXPECT_EQ(2, t.getSize());

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


TEST(TensorConj, TakeConjugateAligned) {
  Tensor t(2,2);
  cdouble data[4] = {cdouble(1,1),cdouble(1,0),cdouble(1,-1),cdouble(2,3)};
  t.setSlice({-1,-1},data);
  t.conj();
  cdouble data2[4]; t.getSlice({-1,-1},data2);
  EXPECT_EQ(1,real(data2[0])); EXPECT_EQ(-1,imag(data2[0]));
  EXPECT_EQ(1,real(data2[1])); EXPECT_EQ(0,imag(data2[1]));
  EXPECT_EQ(1,real(data2[2])); EXPECT_EQ(1,imag(data2[2]));
  EXPECT_EQ(2,real(data2[3])); EXPECT_EQ(-3,imag(data2[3]));
}

TEST(TensorConj, TakeConjugateNotAligned) {
  Tensor t(3,2);
  cdouble data[4] = {cdouble(1,1),cdouble(1,0),cdouble(1,-1),cdouble(2,3)};
  t.setSlice({-1,-1,0},data);
  cdouble data2[4] = {cdouble(0,0),cdouble(2,2),cdouble(3,0),cdouble(1,-4)};
  t.setSlice({-1,-1,1},data2);
  t.conj();
  cdouble data3[4]; t.getSlice({-1,0,-1},data3);
  EXPECT_EQ(1,real(data3[0])); EXPECT_EQ(-1,imag(data3[0]));
  EXPECT_EQ(1,real(data3[1])); EXPECT_EQ(0,imag(data3[1]));
  EXPECT_EQ(0,real(data3[2])); EXPECT_EQ(0,imag(data3[2]));
  EXPECT_EQ(2,real(data3[3])); EXPECT_EQ(-2,imag(data3[3]));

  t.getSlice({-1,1,-1},data3);
  EXPECT_EQ(1,real(data3[0])); EXPECT_EQ(1,imag(data3[0]));
  EXPECT_EQ(2,real(data3[1])); EXPECT_EQ(-3,imag(data3[1]));
  EXPECT_EQ(3,real(data3[2])); EXPECT_EQ(0,imag(data3[2]));
  EXPECT_EQ(1,real(data3[3])); EXPECT_EQ(4,imag(data3[3]));
}

TEST(TensorConj, TakeConjugateTwice) {
  Tensor t(2,2);
  cdouble data[4] = {cdouble(1,1),cdouble(1,0),cdouble(1,-1),cdouble(2,3)};
  t.setSlice({-1,-1},data);
  t.conj();
  t.conj();
  cdouble data2[4]; t.getSlice({-1,-1},data2);
  EXPECT_EQ(1,real(data2[0])); EXPECT_EQ(1,imag(data2[0]));
  EXPECT_EQ(1,real(data2[1])); EXPECT_EQ(0,imag(data2[1]));
  EXPECT_EQ(1,real(data2[2])); EXPECT_EQ(-1,imag(data2[2]));
  EXPECT_EQ(2,real(data2[3])); EXPECT_EQ(3,imag(data2[3]));
}


}


