#include "gtest/gtest.h"
#include "pichi/op_tensor.h"

/*
 * Unit tests of the outer product tensor class,
 * implemented in OP_TENSOR.CC
 */

using namespace pichi;
using namespace std;

namespace {


TEST(OPTensor, GetInputTensorsBack) {
  Tensor t1(2,2);
  cdouble r1[4] = {1.0,2.0,3.0,4.0};
  t1.setSlice({-1,-1},r1);
  Tensor t2(2,2);
  cdouble r2[4] = {8.0,7.0,6.0,5.0};
  t2.setSlice({-1,-1},r2);

  OPTensor opt(t1,t2);
  Tensor tt = opt.getTensor1(); cdouble rr[4];
  tt.getSlice({-1,-1},rr);
  EXPECT_EQ(1.0, rr[0]); EXPECT_EQ(2.0, rr[1]);
  EXPECT_EQ(3.0, rr[2]); EXPECT_EQ(4.0, rr[3]);

  tt = opt.getTensor2();
  tt.getSlice({-1,-1},rr);
  EXPECT_EQ(8.0, rr[0]); EXPECT_EQ(7.0, rr[1]);
  EXPECT_EQ(6.0, rr[2]); EXPECT_EQ(5.0, rr[3]);
}

TEST(OPTensor, TensorsAreShallowCopies) {
  Tensor t1(2,2);
  cdouble r1[4] = {1.0,2.0,3.0,4.0};
  t1.setSlice({-1,-1},r1);
  Tensor t2(2,2);
  cdouble r2[4] = {8.0,7.0,6.0,5.0};
  t2.setSlice({-1,-1},r2);

  OPTensor opt(t1,t2);
  r1[0] = -4.0;
  t1.setSlice({-1,-1},r1);
  Tensor tt = opt.getTensor1(); cdouble rr[4];
  tt.getSlice({-1,-1},rr);
  EXPECT_EQ(1.0, rr[0]); EXPECT_EQ(2.0, rr[1]);
  EXPECT_EQ(3.0, rr[2]); EXPECT_EQ(4.0, rr[3]);

}

TEST(OPTensor, TensorsCanBeModified) {
  Tensor t1(2,2);
  cdouble r1[4] = {1.0,2.0,3.0,4.0};
  t1.setSlice({-1,-1},r1);
  Tensor t2(2,2);
  cdouble r2[4] = {8.0,7.0,6.0,5.0};
  t2.setSlice({-1,-1},r2);

  OPTensor opt(t1,t2);
  cdouble rr[4] = {-1.0,-2.0,-3.0,-4.0};
  opt.getTensor1().setSlice({-1,-1},rr);
  opt.getTensor1().getSlice({-1,-1},rr);
  EXPECT_EQ(-1.0, rr[0]); EXPECT_EQ(-2.0, rr[1]);
  EXPECT_EQ(-3.0, rr[2]); EXPECT_EQ(-4.0, rr[3]);

}

}
