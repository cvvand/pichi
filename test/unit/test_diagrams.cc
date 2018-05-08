#include "diagrams.h"
#include "gtest/gtest.h"

/*
 * Unit tests of the contraction logic defined in CONTRACTION.CC
 */

using namespace pichi;
using namespace std;

namespace {

TEST(Diagram, Diagram0) {
  Tensor t1(2,2);
  cdouble data[4];
  data[0] = 1.0; data[1] = 2.0; data[2] = 3.0; data[3] = 4.0;
  t1.setSlice({-1,-1},data);

  EXPECT_EQ(5.0, diagram0(t1));
}

TEST(Diagram, Diagram1_ab_ba) {
  Tensor t1(2,3);
  cdouble data[9];
  data[0] = 2.0; data[1] = 3.0; data[2] = 3.0;
  data[3] = -1.0; data[4] = 2.0; data[5] = -4.0;
  data[6] = 2.0; data[7] = -5.0; data[8] = -2.0;
  t1.setSlice({-1,-1},data);

  Tensor t2(2,3);
  data[0] = 2.0; data[1] = -2.0; data[2] = 5.0;
  data[3] = -5.0; data[4] = -1.0; data[5] = -1.0;
  data[6] = 3.0; data[7] = 3.0; data[8] = -3.0;
  t2.setSlice({-1,-1},data);

  EXPECT_EQ(7.0, diagram1(t1, "ab", t2, "ba"));
}

TEST(Diagram, Diagram1_ab_ab) {
  Tensor t1(2,3);
  cdouble data[9];
  data[0] = 2.0; data[1] = 3.0; data[2] = 3.0;
  data[3] = -1.0; data[4] = 2.0; data[5] = -4.0;
  data[6] = 2.0; data[7] = -5.0; data[8] = -2.0;
  t1.setSlice({-1,-1},data);

  Tensor t2(2,3);
  data[0] = 2.0; data[1] = -2.0; data[2] = 5.0;
  data[3] = -5.0; data[4] = -1.0; data[5] = -1.0;
  data[6] = 3.0; data[7] = 3.0; data[8] = -3.0;
  t2.setSlice({-1,-1},data);

  EXPECT_EQ(17.0, diagram1(t1, "ab", t2, "ab"));
}

TEST(Diagram, Diagram2_ab_bc_ca) {
  Tensor t1(2,2);
  cdouble data[4];
  data[0] = 2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1.setSlice({-1,-1},data);

  Tensor t2(2,2);
  data[0] = 2.0; data[1] = -2.0;
  data[2] = 5.0; data[3] = -5.0;
  t2.setSlice({-1,-1},data);

  Tensor t3(2,2);
  data[0] = -1.0; data[1] = 0.0;
  data[2] = 1.0; data[3] = 3.0;
  t3.setSlice({-1,-1},data);

  EXPECT_EQ(70.0, diagram2(t1,"ab",t2,"bc",t3,"ca"));
}

TEST(Diagram, Diagram2_ab_cb_ca) {
  Tensor t1(2,2);
  cdouble data[4];
  data[0] = 2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1.setSlice({-1,-1},data);

  Tensor t2(2,2);
  data[0] = 2.0; data[1] = -2.0;
  data[2] = 5.0; data[3] = -5.0;
  t2.setSlice({-1,-1},data);

  Tensor t3(2,2);
  data[0] = -1.0; data[1] = 0.0;
  data[2] = 1.0; data[3] = 3.0;
  t3.setSlice({-1,-1},data);

  EXPECT_EQ(-21.0, diagram2(t1,"ab",t2,"cb",t3,"ca"));
}

TEST(Diagram, Diagram2_ba_cb_ca) {
  Tensor t1(2,2);
  cdouble data[4];
  data[0] = 2.0; data[1] = -3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1.setSlice({-1,-1},data);

  Tensor t2(2,2);
  data[0] = 2.0; data[1] = -2.0;
  data[2] = 5.0; data[3] = -5.0;
  t2.setSlice({-1,-1},data);

  Tensor t3(2,2);
  data[0] = -1.0; data[1] = 0.0;
  data[2] = 1.0; data[3] = 3.0;
  t3.setSlice({-1,-1},data);

  EXPECT_EQ(9.0, diagram2(t1,"ba",t2,"cb",t3,"ca"));
}

TEST(Diagram, Diagram2_ba_bc_ca) {
  Tensor t1(2,2);
  cdouble data[4];
  data[0] = 2.0; data[1] = -3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1.setSlice({-1,-1},data);

  Tensor t2(2,2);
  data[0] = 2.0; data[1] = -2.0;
  data[2] = 5.0; data[3] = -5.0;
  t2.setSlice({-1,-1},data);

  Tensor t3(2,2);
  data[0] = -1.0; data[1] = 0.0;
  data[2] = 1.0; data[3] = 3.0;
  t3.setSlice({-1,-1},data);

  EXPECT_EQ(58.0, diagram2(t1,"ba",t2,"bc",t3,"ca"));
}

TEST(Diagram, Diagram2_ab_bc_ac) {
  Tensor t1(2,2);
  cdouble data[4];
  data[0] = 2.0; data[1] = -3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1.setSlice({-1,-1},data);

  Tensor t2(2,2);
  data[0] = 2.0; data[1] = -2.0;
  data[2] = 5.0; data[3] = -5.0;
  t2.setSlice({-1,-1},data);

  Tensor t3(2,2);
  data[0] = -1.0; data[1] = 0.0;
  data[2] = 1.0; data[3] = 3.0;
  t3.setSlice({-1,-1},data);

  EXPECT_EQ(-33.0, diagram2(t1,"ab",t2,"bc",t3,"ac"));
}

TEST(Diagram, Diagram3_ab_bc_cd_da) {
  Tensor t1(2,2);
  cdouble data[4];
  data[0] = 2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1.setSlice({-1,-1},data);

  Tensor t2(2,2);
  data[0] = 2.0; data[1] = -2.0;
  data[2] = 5.0; data[3] = -5.0;
  t2.setSlice({-1,-1},data);

  Tensor t3(2,2);
  data[0] = -1.0; data[1] = 0.0;
  data[2] = 1.0; data[3] = 3.0;
  t3.setSlice({-1,-1},data);

  Tensor t4(2,2);
  data[0] = -3.0; data[1] = 2.0;
  data[2] = 1.0; data[3] = -2.0;
  t4.setSlice({-1,-1},data);

  EXPECT_EQ(-184.0, diagram3(t1,"ab",t2,"bc",t3,"dc",t4,"da"));
}

TEST(Diagram, Diagram3_ab_cb_dc_da) {
  Tensor t1(2,2);
  cdouble data[4];
  data[0] = 2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1.setSlice({-1,-1},data);

  Tensor t2(2,2);
  data[0] = 2.0; data[1] = -2.0;
  data[2] = 5.0; data[3] = -5.0;
  t2.setSlice({-1,-1},data);

  Tensor t3(2,2);
  data[0] = -1.0; data[1] = 0.0;
  data[2] = 1.0; data[3] = 3.0;
  t3.setSlice({-1,-1},data);

  Tensor t4(2,2);
  data[0] = -3.0; data[1] = 2.0;
  data[2] = 1.0; data[3] = -2.0;
  t4.setSlice({-1,-1},data);

  EXPECT_EQ(4.0, diagram3(t1,"ab",t2,"cb",t3,"dc",t4,"da"));
}

}