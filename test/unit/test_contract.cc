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

  Contraction<char> c;
  c.addTensor('A', t1);

  EXPECT_EQ(2.0, c.contract('A'));
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

  Contraction<char> c;
  c.addTensor('A', t1);
  c.addTensor('B', t2);

  EXPECT_EQ(7.0, c.contract('A','B',{{0,1},{1,0}}));
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

  Contraction<char> c;
  c.addTensor('A', t1);
  c.addTensor('B', t2);

  EXPECT_EQ(17.0, c.contract('A','B',{{0,0},{1,1}}));
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

  Contraction<char> c;
  c.addTensor('A', t1);
  c.addTensor('B', t2);
  c.addTensor('C', t3);

  c.contract('A','B',{{1,0}},'D');

  EXPECT_EQ(70.0, c.contract('D','C',{{0,1},{1,0}}));
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

  Contraction<char> c;
  c.addTensor('A', t1);
  c.addTensor('B', t2);
  c.addTensor('C', t3);

  c.contract('A','B',{{1,1}},'D');

  EXPECT_EQ(-41.0, c.contract('D','C',{{0,0},{1,1}}));
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

  Contraction<char> c;
  c.addTensor('A', t1);
  c.addTensor('B', t2);
  c.addTensor('C', t3);

  c.contract('A','B',{{0,0}},'D');

  EXPECT_EQ(70.0, c.contract('D','C',{{0,1},{1,0}}));
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

  Contraction<char> c;
  c.addTensor('A', t1);
  c.addTensor('B', t2);
  c.addTensor('C', t3);

  c.contract('A','B',{{0,1}},'D');

  EXPECT_EQ(-21.0, c.contract('D','C',{{0,1},{1,0}}));
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

  Contraction<char> c;
  c.addTensor('A', t1);
  c.addTensor('B', t2);
  c.addTensor('C', t3);

  c.contract('A','B',{{0,1},{1,0}},'D');

  EXPECT_EQ(-28.0, c.contract('D','C',{{0,1},{1,0}}));

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

  Contraction<char> c;
  c.addTensor('A', t1);
  c.addTensor('B', t2);
  c.addTensor('C', t3);

  c.contract('A','B',{{0,0},{1,1}},'D');

  EXPECT_EQ(-48.0, c.contract('D','C',{{0,0},{1,1}}));

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

  Contraction<char> c;
  c.addTensor('A', t1);
  c.addTensor('B', t2);
  c.addTensor('C', t3);

  c.contract('A','B',{{1,1},{0,0}},'D');

  EXPECT_EQ(-48.0, c.contract('D','C',{{0,0},{1,1}}));

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

  Contraction<char> c;
  c.addTensor('A', t1);
  c.addTensor('B', t2);
  c.addTensor('C', t3);

  c.contract('A','B',{{1,2},{2,1},{3,0}},'D');

  EXPECT_EQ(-68.0, c.contract('D','C',{{0,1},{1,0}}));

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

  Contraction<char> c;
  c.addTensor('A', t1);
  c.addTensor('B', t2);

  EXPECT_EQ(16.0, c.contract('A','B',{{0,2},{1,1},{2,0}}));

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

  Contraction<char> c;
  c.addTensor('A', t1);
  c.addTensor('B', t2);
  c.addTensor('C', t3);

  c.contract('A','B',{{2,0}},'D');

  EXPECT_EQ(-75.0, c.contract('D','C',{{0,2},{1,1},{2,0}}));

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

  Contraction<char> c;
  c.addTensor('A', t1);
  c.addTensor('B', t2);
  c.addTensor('C', t3);

  c.contract('A','B',{{2,1},{3,0}},'D');

  EXPECT_EQ(17.0, c.contract('D','C',{{0,2},{1,1},{2,0}}));

}


}
