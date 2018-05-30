#include "contraction.h"
#include "gtest/gtest.h"

/*
 * Unit tests of the contraction logic defined in CONTRACTION.CC
 */

using namespace pichi;
using namespace std;

namespace {


TEST(Contract, aa) {
  Tensor t1(2,2);
  cdouble data[4];
  data[0] = 3.0; data[1] = 3.0; data[2] = 3.0; data[3] = -1.0;
  t1.setSlice({-1,-1},data);

  Tensor t = contract(t1,{{0,1}});
  cdouble res_data[1]; t.getSlice({0},res_data);
  EXPECT_EQ(2.0, res_data[0]);
}

TEST(Contract, aabb) {
  Tensor t1(4,2);
  cdouble data[4];
  data[0] = 3.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1.setSlice({-1,-1,0,0},data);
  data[0] = 2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = 0.0;
  t1.setSlice({-1,-1,1,0},data);
  data[0] = 3.0; data[1] = -3.0;
  data[2] = 3.0; data[3] = 2.0;
  t1.setSlice({-1,-1,0,1},data);
  data[0] = 1.0; data[1] = 2.0;
  data[2] = 3.0; data[3] = -4.0;
  t1.setSlice({-1,-1,1,1},data);

  Tensor t = contract(t1,{{0,1},{2,3}});
  cdouble rdata[1]; t.getSlice({0}, rdata);

  EXPECT_EQ(-1.0, rdata[0]);
}

TEST(Contract, aabc) {
  Tensor t1(4,2);
  cdouble data[4];
  data[0] = 3.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1.setSlice({-1,-1,0,0},data);
  data[0] = 2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = 0.0;
  t1.setSlice({-1,-1,1,0},data);
  data[0] = 3.0; data[1] = -3.0;
  data[2] = 3.0; data[3] = 2.0;
  t1.setSlice({-1,-1,0,1},data);
  data[0] = 1.0; data[1] = 2.0;
  data[2] = 3.0; data[3] = -4.0;
  t1.setSlice({-1,-1,1,1},data);

  Tensor t3 = contract(t1,{{0,1}});
  t3.getSlice({-1,-1}, data);

  EXPECT_EQ(2.0, data[0]);
  EXPECT_EQ(2.0, data[1]);
  EXPECT_EQ(5.0, data[2]);
  EXPECT_EQ(-3.0, data[3]);

}

TEST(Contract, abcd_ab) {
  Tensor t1(4,2);
  cdouble data[4];
  data[0] = 3.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1.setSlice({-1,-1,0,0},data);
  data[0] = 2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = 0.0;
  t1.setSlice({-1,-1,1,0},data);
  data[0] = 3.0; data[1] = -3.0;
  data[2] = 3.0; data[3] = 2.0;
  t1.setSlice({-1,-1,0,1},data);
  data[0] = 1.0; data[1] = 2.0;
  data[2] = 3.0; data[3] = -4.0;
  t1.setSlice({-1,-1,1,1},data);

  Tensor t2(2,2);
  data[0] = 2.0; data[1] = -2.0;
  data[2] = 5.0; data[3] = -5.0;
  t2.setSlice({-1,-1},data);

  Tensor t4 = contract(t1,t2,{{0,0},{1,1}});
  t4.getSlice({-1,-1}, data);

  EXPECT_EQ(20.0, data[0]);
  EXPECT_EQ(13.0, data[1]);
  EXPECT_EQ(17.0, data[2]);
  EXPECT_EQ(33.0, data[3]);
}

TEST(Contract, ab_ba) {
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

  Tensor tr = contract(t1,t2,{{0,1},{1,0}});
  tr.getSlice({0},data);

  EXPECT_EQ(7.0, data[0]);
}

TEST(Contract, ab_ab) {
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

  Tensor tr = contract(t1,t2,{{0,0},{1,1}});
  tr.getSlice({0},data);

  EXPECT_EQ(17.0, data[0]);
}

TEST(Contract, ab_bc_ca) {
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

  Tensor t4 = contract(t1,t2,{{1,0}});
  Tensor tr = contract(t4,t3,{{0,1},{1,0}});
  tr.getSlice({0},data);

  EXPECT_EQ(70.0, data[0]);
}

TEST(Contract, ab_cb_ac) {
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

  Tensor t4 = contract(t1,t2,{{1,1}});
  Tensor tr = contract(t4,t3,{{0,0},{1,1}});
  tr.getSlice({0},data);

  EXPECT_EQ(-41.0, data[0]);
}

TEST(Contract, ab_ac_cb) {
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

  Tensor t4 = contract(t1,t2,{{0,0}});
  Tensor tr = contract(t4,t3,{{0,1},{1,0}});
  tr.getSlice({0},data);

  EXPECT_EQ(70.0, data[0]);
}

TEST(Contract, ab_ca_cb) {
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

  Tensor t4 = contract(t1,t2,{{0,1}});
  Tensor tr = contract(t4,t3,{{0,1},{1,0}});
  tr.getSlice({0},data);

  EXPECT_EQ(-21.0, data[0]);
}

TEST(Contract, abc_bad_dc) {
  Tensor t1(3,2);
  cdouble data[4];
  data[0] = 2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1.setSlice({-1,-1,0}, data);
  data[0] = -2.0; data[1] = 1.0;
  data[2] = 5.0;  data[3] = 1.0;
  t1.setSlice({-1,-1,1}, data);

  Tensor t2(3,2);
  data[0] = -4.0; data[1] = -1.0;
  data[2] = 2.0;  data[3] = -1.0;
  t2.setSlice({-1,-1,0}, data);
  data[0] = 1.0; data[1] = 1.0;
  data[2] = 2.0; data[3] = 3.0;
  t2.setSlice({-1,-1,1}, data);

  Tensor t3(2,2);
  data[0] = 1.0; data[1] = -3.0;
  data[2] = 2.0; data[3] = -1.0;
  t3.setSlice({-1,-1}, data);

  Tensor t4 = contract(t1,t2,{{0,1},{1,0}});
  Tensor tr = contract(t4,t3,{{0,1},{1,0}});
  tr.getSlice({0},data);

  EXPECT_EQ(-28.0, data[0]);

}

TEST(Contract, abc_abd_cd) {
  Tensor t1(3,2);
  cdouble data[4];
  data[0] = 2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1.setSlice({-1,-1,0}, data);
  data[0] = -2.0; data[1] = 1.0;
  data[2] = 5.0;  data[3] = 1.0;
  t1.setSlice({-1,-1,1}, data);

  Tensor t2(3,2);
  data[0] = -4.0; data[1] = -1.0;
  data[2] = 2.0;  data[3] = -1.0;
  t2.setSlice({-1,-1,0}, data);
  data[0] = 1.0; data[1] = 1.0;
  data[2] = 2.0; data[3] = 3.0;
  t2.setSlice({-1,-1,1}, data);

  Tensor t3(2,2);
  data[0] = 1.0; data[1] = -3.0;
  data[2] = 2.0; data[3] = -1.0;
  t3.setSlice({-1,-1}, data);

  Tensor t4 = contract(t1,t2,{{0,0},{1,1}});
  Tensor tr = contract(t4,t3,{{0,0},{1,1}});
  tr.getSlice({0},data);

  EXPECT_EQ(-48.0, data[0]);

}

TEST(Contract, abc_abd_cd2) {
  Tensor t1(3,2);
  cdouble data[4];
  data[0] = 2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1.setSlice({-1,-1,0}, data);
  data[0] = -2.0; data[1] = 1.0;
  data[2] = 5.0;  data[3] = 1.0;
  t1.setSlice({-1,-1,1}, data);

  Tensor t2(3,2);
  data[0] = -4.0; data[1] = -1.0;
  data[2] = 2.0;  data[3] = -1.0;
  t2.setSlice({-1,-1,0}, data);
  data[0] = 1.0; data[1] = 1.0;
  data[2] = 2.0; data[3] = 3.0;
  t2.setSlice({-1,-1,1}, data);

  Tensor t3(2,2);
  data[0] = 1.0; data[1] = -3.0;
  data[2] = 2.0; data[3] = -1.0;
  t3.setSlice({-1,-1}, data);

  Tensor t4 = contract(t1,t2,{{1,1},{0,0}});
  Tensor tr = contract(t4,t3,{{0,0},{1,1}});
  tr.getSlice({0},data);

  EXPECT_EQ(-48.0, data[0]);

}

TEST(Contract, abcd_dcbe_ea) {
  Tensor t1(4,2);
  cdouble data[4];
  data[0] = 2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1.setSlice({-1,-1,0,0}, data);
  data[0] = -2.0; data[1] = 1.0;
  data[2] = 5.0;  data[3] = 1.0;
  t1.setSlice({-1,-1,1,0}, data);
  data[0] = 1.0; data[1] = -1.0;
  data[2] = 1.0; data[3] = -1.0;
  t1.setSlice({-1,-1,0,1}, data);
  data[0] = -2.0; data[1] = 2.0;
  data[2] = -5.0;  data[3] = 3.0;
  t1.setSlice({-1,-1,1,1}, data);

  Tensor t2(4,2);
  data[0] = -4.0; data[1] = -1.0;
  data[2] = 2.0;  data[3] = -1.0;
  t2.setSlice({-1,-1,0,0}, data);
  data[0] = 1.0; data[1] = 1.0;
  data[2] = 2.0; data[3] = 3.0;
  t2.setSlice({-1,-1,1,0}, data);
  data[0] = -2.0; data[1] = -1.0;
  data[2] = 2.0;  data[3] = -2.0;
  t2.setSlice({-1,-1,0,1}, data);
  data[0] = -1.0; data[1] = -1.0;
  data[2] = 3.0; data[3] = -3.0;
  t2.setSlice({-1,-1,1,1}, data);

  Tensor t3(2,2);
  data[0] = 1.0; data[1] = -3.0;
  data[2] = 2.0; data[3] = -1.0;
  t3.setSlice({-1,-1}, data);

  Tensor t4 = contract(t1,t2,{{1,2},{2,1},{3,0}});
  Tensor t5 = contract(t4,t3, {{0,1},{1,0}});
  cdouble datar[1];
  t5.getSlice({0},datar);
  EXPECT_EQ(-68.0, datar[0]);

}

TEST(Contract, abc_cba) {
  Tensor t1(3,2);
  cdouble data[4];
  data[0] = -2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1.setSlice({-1,-1,0}, data);
  data[0] = -2.0; data[1] = 1.0;
  data[2] = 5.0;  data[3] = 1.0;
  t1.setSlice({-1,-1,1}, data);

  Tensor t2(3,2);
  data[0] = -4.0; data[1] = -1.0;
  data[2] = 2.0;  data[3] = -1.0;
  t2.setSlice({-1,-1,0}, data);
  data[0] = 1.0; data[1] = 1.0;
  data[2] = 2.0; data[3] = 3.0;
  t2.setSlice({-1,-1,1}, data);

  Tensor t3 = contract(t1,t2,{{0,2},{1,1},{2,0}});
  cdouble datar[1];
  t3.getSlice({0},datar);

  EXPECT_EQ(16.0, datar[0]);

}

TEST(Contract, abc_cd_dba) {
  Tensor t1(3,2);
  cdouble data[4];
  data[0] = 2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1.setSlice({-1,-1,0}, data);
  data[0] = -2.0; data[1] = 1.0;
  data[2] = 5.0;  data[3] = 1.0;
  t1.setSlice({-1,-1,1}, data);

  Tensor t2(2,2);
  data[0] = 1.0; data[1] = -3.0;
  data[2] = 2.0; data[3] = -1.0;
  t2.setSlice({-1,-1}, data);

  Tensor t3(3,2);
  data[0] = -4.0; data[1] = -1.0;
  data[2] = 2.0;  data[3] = -1.0;
  t3.setSlice({-1,-1,0}, data);
  data[0] = 1.0; data[1] = 1.0;
  data[2] = 2.0; data[3] = 3.0;
  t3.setSlice({-1,-1,1}, data);

  Tensor t4 = contract(t1,t2,{{2,0}});
  Tensor t5 = contract(t4,t3,{{0,2},{1,1},{2,0}});
  cdouble datar[1];
  t5.getSlice({0}, datar);

  EXPECT_EQ(-75.0, datar[0]);

}

TEST(Contract, abcd_dce_eba) {
  Tensor t1(4,2);
  cdouble data[4];
  data[0] = 2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1.setSlice({-1,-1,0,0}, data);
  data[0] = -2.0; data[1] = 1.0;
  data[2] = 5.0;  data[3] = 1.0;
  t1.setSlice({-1,-1,1,0}, data);
  data[0] = 1.0; data[1] = -1.0;
  data[2] = 1.0; data[3] = -1.0;
  t1.setSlice({-1,-1,0,1}, data);
  data[0] = -2.0; data[1] = 2.0;
  data[2] = -5.0;  data[3] = 3.0;
  t1.setSlice({-1,-1,1,1}, data);

  Tensor t2(3,2);
  data[0] = -4.0; data[1] = -1.0;
  data[2] = 2.0;  data[3] = -1.0;
  t2.setSlice({-1,-1,0}, data);
  data[0] = 1.0; data[1] = 1.0;
  data[2] = 2.0; data[3] = 3.0;
  t2.setSlice({-1,-1,1}, data);

  Tensor t3(3,2);
  data[0] = 1.0; data[1] = -3.0;
  data[2] = 2.0; data[3] = -1.0;
  t3.setSlice({-1,-1,0}, data);
  data[0] = -2.0; data[1] = -1.0;
  data[2] = 2.0;  data[3] = -2.0;
  t3.setSlice({-1,-1,1}, data);

  Tensor t4 = contract(t1,t2,{{2,1},{3,0}});
  Tensor t5 = contract(t4,t3,{{0,2},{1,1},{2,0}});
  cdouble datar[1];
  t5.getSlice({0}, datar);

  EXPECT_EQ(17.0, datar[0]);

}


}
