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

  DiagramNode n;
  n.t = &t1;

  vector<DiagramNode> nodes;
  nodes.push_back(n);

  EXPECT_EQ(5.0, diagram0(nodes));
}

TEST(Diagram, Diagram1_ab_ba) {
  Tensor* t1 = new Tensor(2,3);
  cdouble data[9];
  data[0] = 2.0; data[1] = 3.0; data[2] = 3.0;
  data[3] = -1.0; data[4] = 2.0; data[5] = -4.0;
  data[6] = 2.0; data[7] = -5.0; data[8] = -2.0;
  t1->setSlice({-1,-1},data);

  Tensor* t2 = new Tensor(2,3);
  data[0] = 2.0; data[1] = -2.0; data[2] = 5.0;
  data[3] = -5.0; data[4] = -1.0; data[5] = -1.0;
  data[6] = 3.0; data[7] = 3.0; data[8] = -3.0;
  t2->setSlice({-1,-1},data);

  DiagramNode n1;
  n1.t = t1;
  n1.idx = "ab";

  DiagramNode n2;
  n2.t = t2;
  n2.idx = "ba";

  vector<DiagramNode> nodes;
  nodes.push_back(n1);
  nodes.push_back(n2);

  EXPECT_EQ(7.0, diagram1(nodes));
  
  delete t1;
}

TEST(Diagram, Diagram1_ab_ab) {
  Tensor* t1 = new Tensor(2,3);
  cdouble data[9];
  data[0] = 2.0; data[1] = 3.0; data[2] = 3.0;
  data[3] = -1.0; data[4] = 2.0; data[5] = -4.0;
  data[6] = 2.0; data[7] = -5.0; data[8] = -2.0;
  t1->setSlice({-1,-1},data);

  Tensor* t2 = new Tensor(2,3);
  data[0] = 2.0; data[1] = -2.0; data[2] = 5.0;
  data[3] = -5.0; data[4] = -1.0; data[5] = -1.0;
  data[6] = 3.0; data[7] = 3.0; data[8] = -3.0;
  t2->setSlice({-1,-1},data);

  DiagramNode n1;
  n1.t = t1;
  n1.idx = "ab";

  DiagramNode n2;
  n2.t = t2;
  n2.idx = "ab";

  vector<DiagramNode> nodes;
  nodes.push_back(n1);
  nodes.push_back(n2);

  EXPECT_EQ(17.0, diagram1(nodes));

  delete t1;
  delete t2;
}

TEST(Diagram, Diagram2_ab_bc_ca) {
  Tensor* t1 = new Tensor(2,2);
  cdouble data[4];
  data[0] = 2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1->setSlice({-1,-1},data);

  Tensor* t2 = new Tensor(2,2);
  data[0] = 2.0; data[1] = -2.0;
  data[2] = 5.0; data[3] = -5.0;
  t2->setSlice({-1,-1},data);

  Tensor* t3 = new Tensor(2,2);
  data[0] = -1.0; data[1] = 0.0;
  data[2] = 1.0; data[3] = 3.0;
  t3->setSlice({-1,-1},data);

  DiagramNode n1;
  n1.t = t1;
  n1.idx = "ab";

  DiagramNode n2;
  n2.t = t2;
  n2.idx = "bc";

  DiagramNode n3;
  n3.t = t3;
  n3.idx = "ca";

  vector<DiagramNode> nodes;
  nodes.push_back(n1);
  nodes.push_back(n2);
  nodes.push_back(n3);

  EXPECT_EQ(70.0, diagram2(nodes));

  delete t1;
  delete t2;
  delete t3;
}

TEST(Diagram, Diagram2_ba_cb_ca) {
  Tensor* t1 = new Tensor(2,2);
  cdouble data[4];
  data[0] = 2.0; data[1] = -3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1->setSlice({-1,-1},data);

  Tensor* t2 = new Tensor(2,2);
  data[0] = 2.0; data[1] = -2.0;
  data[2] = 5.0; data[3] = -5.0;
  t2->setSlice({-1,-1},data);

  Tensor* t3 = new Tensor(2,2);
  data[0] = -1.0; data[1] = 0.0;
  data[2] = 1.0; data[3] = 3.0;
  t3->setSlice({-1,-1},data);

  DiagramNode n1;
  n1.t = t1;
  n1.idx = "ba";

  DiagramNode n2;
  n2.t = t2;
  n2.idx = "cb";

  DiagramNode n3;
  n3.t = t3;
  n3.idx = "ca";

  vector<DiagramNode> nodes;
  nodes.push_back(n1);
  nodes.push_back(n2);
  nodes.push_back(n3);

  EXPECT_EQ(9.0, diagram2(nodes));

  delete t1;
  delete t2;
  delete t3;

}

TEST(Diagram, Diagram2_ba_bc_ca) {
  Tensor* t1 = new Tensor(2,2);
  cdouble data[4];
  data[0] = 2.0; data[1] = -3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1->setSlice({-1,-1},data);

  Tensor* t2 = new Tensor(2,2);
  data[0] = 2.0; data[1] = -2.0;
  data[2] = 5.0; data[3] = -5.0;
  t2->setSlice({-1,-1},data);

  Tensor* t3 = new Tensor(2,2);
  data[0] = -1.0; data[1] = 0.0;
  data[2] = 1.0; data[3] = 3.0;
  t3->setSlice({-1,-1},data);

  DiagramNode n1;
  n1.t = t1;
  n1.idx = "ba";

  DiagramNode n2;
  n2.t = t2;
  n2.idx = "bc";

  DiagramNode n3;
  n3.t = t3;
  n3.idx = "ca";

  vector<DiagramNode> nodes;
  nodes.push_back(n1);
  nodes.push_back(n2);
  nodes.push_back(n3);

  EXPECT_EQ(58.0, diagram2(nodes));

  delete t1;
  delete t2;
  delete t3;
}

TEST(Diagram, Diagram3_ab_bc_cd_da) {
  Tensor* t1 = new Tensor(2,2);
  cdouble data[4];
  data[0] = 2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1->setSlice({-1,-1},data);

  Tensor* t2 = new Tensor(2,2);
  data[0] = 2.0; data[1] = -2.0;
  data[2] = 5.0; data[3] = -5.0;
  t2->setSlice({-1,-1},data);

  Tensor* t3 = new Tensor(2,2);
  data[0] = -1.0; data[1] = 0.0;
  data[2] = 1.0; data[3] = 3.0;
  t3->setSlice({-1,-1},data);

  Tensor* t4 = new Tensor(2,2);
  data[0] = -3.0; data[1] = 2.0;
  data[2] = 1.0; data[3] = -2.0;
  t4->setSlice({-1,-1},data);

  DiagramNode n1;
  n1.t = t1;
  n1.idx = "ab";

  DiagramNode n2;
  n2.t = t2;
  n2.idx = "bc";

  DiagramNode n3;
  n3.t = t3;
  n3.idx = "cd";

  DiagramNode n4;
  n4.t = t4;
  n4.idx = "da";

  vector<DiagramNode> nodes;
  nodes.push_back(n1);
  nodes.push_back(n2);
  nodes.push_back(n3);
  nodes.push_back(n4);

  EXPECT_EQ(-184.0, diagram3(nodes));

  delete t1;
  delete t2;
  delete t3;
  delete t4;
}

TEST(Diagram, Diagram3_ba_cb_dc_ad) {
  Tensor* t1 = new Tensor(2,2);
  cdouble data[4];
  data[0] = 2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1->setSlice({-1,-1},data);

  Tensor* t2 = new Tensor(2,2);
  data[0] = 2.0; data[1] = -2.0;
  data[2] = 5.0; data[3] = -5.0;
  t2->setSlice({-1,-1},data);

  Tensor* t3 = new Tensor(2,2);
  data[0] = -1.0; data[1] = 0.0;
  data[2] = 1.0; data[3] = 3.0;
  t3->setSlice({-1,-1},data);

  Tensor* t4 = new Tensor(2,2);
  data[0] = -3.0; data[1] = 2.0;
  data[2] = 1.0; data[3] = -2.0;
  t4->setSlice({-1,-1},data);

  DiagramNode n1;
  n1.t = t1;
  n1.idx = "ba";

  DiagramNode n2;
  n2.t = t2;
  n2.idx = "cb";

  DiagramNode n3;
  n3.t = t3;
  n3.idx = "cd";

  DiagramNode n4;
  n4.t = t4;
  n4.idx = "da";

  vector<DiagramNode> nodes;
  nodes.push_back(n1);
  nodes.push_back(n2);
  nodes.push_back(n3);
  nodes.push_back(n4);

  EXPECT_EQ(-16.0, diagram3(nodes));

  delete t1;
  delete t2;
  delete t3;
  delete t4;
}

TEST(Diagram, Diagram3_ab_cd_ac_bd) {
  Tensor* t1 = new Tensor(2,2);
  cdouble data[4];
  data[0] = 2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1->setSlice({-1,-1},data);

  Tensor* t2 = new Tensor(2,2);
  data[0] = 2.0; data[1] = -2.0;
  data[2] = 5.0; data[3] = -5.0;
  t2->setSlice({-1,-1},data);

  Tensor* t3 = new Tensor(2,2);
  data[0] = -1.0; data[1] = 0.0;
  data[2] = 1.0; data[3] = 3.0;
  t3->setSlice({-1,-1},data);

  Tensor* t4 = new Tensor(2,2);
  data[0] = -3.0; data[1] = 2.0;
  data[2] = 1.0; data[3] = -2.0;
  t4->setSlice({-1,-1},data);

  DiagramNode n1;
  n1.t = t1;
  n1.idx = "ab";

  DiagramNode n2;
  n2.t = t2;
  n2.idx = "cd";

  DiagramNode n3;
  n3.t = t3;
  n3.idx = "ac";

  DiagramNode n4;
  n4.t = t4;
  n4.idx = "bd";

  vector<DiagramNode> nodes;
  nodes.push_back(n1);
  nodes.push_back(n2);
  nodes.push_back(n3);
  nodes.push_back(n4);

  EXPECT_EQ(31.0, diagram3(nodes));

  delete t1;
  delete t2;
  delete t3;
  delete t4;
}

TEST(Diagram, Diagram4_abc_cba) {
  Tensor* t1 = new Tensor(3,2);
  cdouble data[4];
  data[0] = -2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1->setSlice({-1,-1,0}, data);
  data[0] = -2.0; data[1] = 1.0;
  data[2] = 5.0;  data[3] = 1.0;
  t1->setSlice({-1,-1,1}, data);

  Tensor* t2 = new Tensor(3,2);
  data[0] = -4.0; data[1] = -1.0;
  data[2] = 2.0;  data[3] = -1.0;
  t2->setSlice({-1,-1,0}, data);
  data[0] = 1.0; data[1] = 1.0;
  data[2] = 2.0; data[3] = 3.0;
  t2->setSlice({-1,-1,1}, data);

  DiagramNode n1;
  n1.t = t1;
  n1.idx = "abc";

  DiagramNode n2;
  n2.t = t2;
  n2.idx = "cba";

  vector<DiagramNode> nodes;
  nodes.push_back(n1);
  nodes.push_back(n2);

  EXPECT_EQ(16.0, diagram4(nodes));

  delete t1;
  delete t2;

}

TEST(Diagram, Diagram4_abc_bac) {
  Tensor* t1 = new Tensor(3,2);
  cdouble data[4];
  data[0] = -2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1->setSlice({-1,-1,0}, data);
  data[0] = -2.0; data[1] = 1.0;
  data[2] = 5.0;  data[3] = 1.0;
  t1->setSlice({-1,-1,1}, data);

  Tensor* t2 = new Tensor(3,2);
  data[0] = -4.0; data[1] = -1.0;
  data[2] = 2.0;  data[3] = -1.0;
  t2->setSlice({-1,-1,0}, data);
  data[0] = 1.0; data[1] = 1.0;
  data[2] = 2.0; data[3] = 3.0;
  t2->setSlice({-1,-1,1}, data);

  DiagramNode n1;
  n1.t = t1;
  n1.idx = "abc";

  DiagramNode n2;
  n2.t = t2;
  n2.idx = "bac";

  vector<DiagramNode> nodes;
  nodes.push_back(n1);
  nodes.push_back(n2);

  EXPECT_EQ(20.0, diagram4(nodes));

  delete t1;
  delete t2;

}


TEST(Diagram, Diagram5_abc_abd_cd) {
  Tensor* t1 = new Tensor(3,2);
  cdouble data[4];
  data[0] = -2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1->setSlice({-1,-1,0}, data);
  data[0] = -2.0; data[1] = 1.0;
  data[2] = 5.0;  data[3] = 1.0;
  t1->setSlice({-1,-1,1}, data);

  Tensor* t2 = new Tensor(3,2);
  data[0] = -4.0; data[1] = -1.0;
  data[2] = 2.0;  data[3] = -1.0;
  t2->setSlice({-1,-1,0}, data);
  data[0] = 1.0; data[1] = 1.0;
  data[2] = 2.0; data[3] = 3.0;
  t2->setSlice({-1,-1,1}, data);

  Tensor* t3 = new Tensor(2,2);
  data[0] = 2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t3->setSlice({-1,-1},data);

  DiagramNode n1;
  n1.t = t1;
  n1.idx = "abc";

  DiagramNode n2;
  n2.t = t2;
  n2.idx = "abd";

  DiagramNode n3;
  n3.t = t3;
  n3.idx = "cd";

  vector<DiagramNode> nodes;
  nodes.push_back(n1);
  nodes.push_back(n2);
  nodes.push_back(n3);

  EXPECT_EQ(72.0, diagram5(nodes));

  delete t1;
  delete t2;
  delete t3;

}

TEST(Diagram, Diagram5_ba_acd_bcd) {
  Tensor* t1 = new Tensor(3,2);
  cdouble data[4];
  data[0] = -2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1->setSlice({-1,-1,0}, data);
  data[0] = -2.0; data[1] = 1.0;
  data[2] = 5.0;  data[3] = 1.0;
  t1->setSlice({-1,-1,1}, data);

  Tensor* t2 = new Tensor(3,2);
  data[0] = -4.0; data[1] = -1.0;
  data[2] = 2.0;  data[3] = -1.0;
  t2->setSlice({-1,-1,0}, data);
  data[0] = 1.0; data[1] = 1.0;
  data[2] = 2.0; data[3] = 3.0;
  t2->setSlice({-1,-1,1}, data);

  Tensor* t3 = new Tensor(2,2);
  data[0] = 2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t3->setSlice({-1,-1},data);

  DiagramNode n1;
  n1.t = t1;
  n1.idx = "acd";

  DiagramNode n2;
  n2.t = t3;
  n2.idx = "ba";

  DiagramNode n3;
  n3.t = t2;
  n3.idx = "bcd";

  vector<DiagramNode> nodes;
  nodes.push_back(n1);
  nodes.push_back(n2);
  nodes.push_back(n3);

  EXPECT_EQ(45.0, diagram5(nodes));

  delete t1;
  delete t2;
  delete t3;

}

TEST(Diagram, Diagram6_abc_abd_ce_de) {
  Tensor* t1 = new Tensor(3,2);
  cdouble data[4];
  data[0] = -2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1->setSlice({-1,-1,0}, data);
  data[0] = -2.0; data[1] = 1.0;
  data[2] = 5.0;  data[3] = 1.0;
  t1->setSlice({-1,-1,1}, data);

  Tensor* t2 = new Tensor(3,2);
  data[0] = -4.0; data[1] = -1.0;
  data[2] = 2.0;  data[3] = -1.0;
  t2->setSlice({-1,-1,0}, data);
  data[0] = 1.0; data[1] = 1.0;
  data[2] = 2.0; data[3] = 3.0;
  t2->setSlice({-1,-1,1}, data);

  Tensor* t3 = new Tensor(2,2);
  data[0] = 2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t3->setSlice({-1,-1},data);

  Tensor* t4 = new Tensor(2,2);
  data[0] = 2.0; data[1] = -2.0;
  data[2] = 5.0; data[3] = -5.0;
  t4->setSlice({-1,-1},data);

  DiagramNode n1;
  n1.t = t1;
  n1.idx = "abc";

  DiagramNode n2;
  n2.t = t2;
  n2.idx = "abd";

  DiagramNode n3;
  n3.t = t3;
  n3.idx = "ce";

  DiagramNode n4;
  n4.t = t4;
  n4.idx = "de";

  vector<DiagramNode> nodes;
  nodes.push_back(n1);
  nodes.push_back(n2);
  nodes.push_back(n3);
  nodes.push_back(n4);

  EXPECT_EQ(156.0, diagram6(nodes));

  delete t1;
  delete t2;
  delete t3;
  delete t4;

}

TEST(Diagram, Diagram6_cba_abd_ec_de) {
  Tensor* t1 = new Tensor(3,2);
  cdouble data[4];
  data[0] = -2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1->setSlice({-1,-1,0}, data);
  data[0] = -2.0; data[1] = 1.0;
  data[2] = 5.0;  data[3] = 1.0;
  t1->setSlice({-1,-1,1}, data);

  Tensor* t2 = new Tensor(3,2);
  data[0] = -4.0; data[1] = -1.0;
  data[2] = 2.0;  data[3] = -1.0;
  t2->setSlice({-1,-1,0}, data);
  data[0] = 1.0; data[1] = 1.0;
  data[2] = 2.0; data[3] = 3.0;
  t2->setSlice({-1,-1,1}, data);

  Tensor* t3 = new Tensor(2,2);
  data[0] = 2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t3->setSlice({-1,-1},data);

  Tensor* t4 = new Tensor(2,2);
  data[0] = 2.0; data[1] = -2.0;
  data[2] = 5.0; data[3] = -5.0;
  t4->setSlice({-1,-1},data);

  DiagramNode n1;
  n1.t = t1;
  n1.idx = "cba";

  DiagramNode n2;
  n2.t = t2;
  n2.idx = "abd";

  DiagramNode n3;
  n3.t = t3;
  n3.idx = "ec";

  DiagramNode n4;
  n4.t = t4;
  n4.idx = "de";

  vector<DiagramNode> nodes;
  nodes.push_back(n1);
  nodes.push_back(n2);
  nodes.push_back(n3);
  nodes.push_back(n4);

  EXPECT_EQ(-135.0, diagram6(nodes));

  delete t1;
  delete t2;
  delete t3;
  delete t4;

}

TEST(Diagram, Diagram7_abc_ead_bd_ec) {
  Tensor* t1 = new Tensor(3,2);
  cdouble data[4];
  data[0] = -2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1->setSlice({-1,-1,0}, data);
  data[0] = -2.0; data[1] = 1.0;
  data[2] = 5.0;  data[3] = 1.0;
  t1->setSlice({-1,-1,1}, data);

  Tensor* t2 = new Tensor(3,2);
  data[0] = -4.0; data[1] = -1.0;
  data[2] = 2.0;  data[3] = -1.0;
  t2->setSlice({-1,-1,0}, data);
  data[0] = 1.0; data[1] = 1.0;
  data[2] = 2.0; data[3] = 3.0;
  t2->setSlice({-1,-1,1}, data);

  Tensor* t3 = new Tensor(2,2);
  data[0] = 2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t3->setSlice({-1,-1},data);

  Tensor* t4 = new Tensor(2,2);
  data[0] = 2.0; data[1] = -2.0;
  data[2] = 5.0; data[3] = -5.0;
  t4->setSlice({-1,-1},data);

  DiagramNode n1;
  n1.t = t1;
  n1.idx = "abc";

  DiagramNode n2;
  n2.t = t2;
  n2.idx = "ead";

  DiagramNode n3;
  n3.t = t3;
  n3.idx = "bd";

  DiagramNode n4;
  n4.t = t4;
  n4.idx = "ec";

  vector<DiagramNode> nodes;
  nodes.push_back(n1);
  nodes.push_back(n2);
  nodes.push_back(n3);
  nodes.push_back(n4);

  EXPECT_EQ(-132.0, diagram7(nodes));

  delete t1;
  delete t2;
  delete t3;
  delete t4;

}

TEST(Diagram, Diagram7_abc_bd_ade_ec) {
  Tensor* t1 = new Tensor(3,2);
  cdouble data[4];
  data[0] = -2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1->setSlice({-1,-1,0}, data);
  data[0] = -2.0; data[1] = 1.0;
  data[2] = 5.0;  data[3] = 1.0;
  t1->setSlice({-1,-1,1}, data);

  Tensor* t2 = new Tensor(3,2);
  data[0] = -4.0; data[1] = -1.0;
  data[2] = 2.0;  data[3] = -1.0;
  t2->setSlice({-1,-1,0}, data);
  data[0] = 1.0; data[1] = 1.0;
  data[2] = 2.0; data[3] = 3.0;
  t2->setSlice({-1,-1,1}, data);

  Tensor* t3 = new Tensor(2,2);
  data[0] = 2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t3->setSlice({-1,-1},data);

  Tensor* t4 = new Tensor(2,2);
  data[0] = 2.0; data[1] = -2.0;
  data[2] = 5.0; data[3] = -5.0;
  t4->setSlice({-1,-1},data);

  DiagramNode n1;
  n1.t = t1;
  n1.idx = "abc";

  DiagramNode n2;
  n2.t = t3;
  n2.idx = "bd";

  DiagramNode n3;
  n3.t = t2;
  n3.idx = "ade";

  DiagramNode n4;
  n4.t = t4;
  n4.idx = "ec";

  vector<DiagramNode> nodes;
  nodes.push_back(n1);
  nodes.push_back(n2);
  nodes.push_back(n3);
  nodes.push_back(n4);

  EXPECT_EQ(-507.0, diagram7(nodes));

  delete t1;
  delete t2;
  delete t3;
  delete t4;

}


TEST(Diagram, Diagram8_abc_dab_dfe_cfe) {
  Tensor* t1 = new Tensor(3,2);
  cdouble data[4];
  data[0] = -2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1->setSlice({-1,-1,0}, data);
  data[0] = -2.0; data[1] = 1.0;
  data[2] = 5.0;  data[3] = 1.0;
  t1->setSlice({-1,-1,1}, data);

  Tensor* t2 = new Tensor(3,2);
  data[0] = -4.0; data[1] = -1.0;
  data[2] = 2.0;  data[3] = -1.0;
  t2->setSlice({-1,-1,0}, data);
  data[0] = 1.0; data[1] = 1.0;
  data[2] = 2.0; data[3] = 3.0;
  t2->setSlice({-1,-1,1}, data);

  Tensor* t3 = new Tensor(3,2);
  data[0] = 0.0; data[1] = 2.0;
  data[2] = 5.0; data[3] = 1.0;
  t3->setSlice({-1,-1,0}, data);
  data[0] = 1.0; data[1] = -1.0;
  data[2] = 3.0;  data[3] = -2.0;
  t3->setSlice({-1,-1,1}, data);

  Tensor* t4 = new Tensor(3,2);
  data[0] = 8.0; data[1] = 0.0;
  data[2] = -3.0;  data[3] = 0.0;
  t4->setSlice({-1,-1,0}, data);
  data[0] = 2.0; data[1] = -4.0;
  data[2] = 3.0; data[3] = 3.0;
  t4->setSlice({-1,-1,1}, data);

  DiagramNode n1;
  n1.t = t1;
  n1.idx = "abc";

  DiagramNode n2;
  n2.t = t2;
  n2.idx = "dab";

  DiagramNode n3;
  n3.t = t3;
  n3.idx = "dfe";

  DiagramNode n4;
  n4.t = t4;
  n4.idx = "cfe";

  vector<DiagramNode> nodes;
  nodes.push_back(n1);
  nodes.push_back(n2);
  nodes.push_back(n3);
  nodes.push_back(n4);

  EXPECT_EQ(2.0, diagram8(nodes));

  delete t1;
  delete t2;
  delete t3;
  delete t4;

}

TEST(Diagram, Diagram8_abc_def_dba_fce) {
  Tensor* t1 = new Tensor(3,2);
  cdouble data[4];
  data[0] = -2.0; data[1] = 3.0;
  data[2] = 3.0; data[3] = -1.0;
  t1->setSlice({-1,-1,0}, data);
  data[0] = -2.0; data[1] = 1.0;
  data[2] = 5.0;  data[3] = 1.0;
  t1->setSlice({-1,-1,1}, data);

  Tensor* t2 = new Tensor(3,2);
  data[0] = -4.0; data[1] = -1.0;
  data[2] = 2.0;  data[3] = -1.0;
  t2->setSlice({-1,-1,0}, data);
  data[0] = 1.0; data[1] = 1.0;
  data[2] = 2.0; data[3] = 3.0;
  t2->setSlice({-1,-1,1}, data);

  Tensor* t3 = new Tensor(3,2);
  data[0] = 0.0; data[1] = 2.0;
  data[2] = 5.0; data[3] = 1.0;
  t3->setSlice({-1,-1,0}, data);
  data[0] = 1.0; data[1] = -1.0;
  data[2] = 3.0;  data[3] = -2.0;
  t3->setSlice({-1,-1,1}, data);

  Tensor* t4 = new Tensor(3,2);
  data[0] = 8.0; data[1] = 0.0;
  data[2] = -3.0;  data[3] = 0.0;
  t4->setSlice({-1,-1,0}, data);
  data[0] = 2.0; data[1] = -4.0;
  data[2] = 3.0; data[3] = 3.0;
  t4->setSlice({-1,-1,1}, data);

  DiagramNode n1;
  n1.t = t1;
  n1.idx = "abc";

  DiagramNode n2;
  n2.t = t2;
  n2.idx = "def";

  DiagramNode n3;
  n3.t = t3;
  n3.idx = "dba";

  DiagramNode n4;
  n4.t = t4;
  n4.idx = "fce";

  vector<DiagramNode> nodes;
  nodes.push_back(n1);
  nodes.push_back(n2);
  nodes.push_back(n3);
  nodes.push_back(n4);


  EXPECT_EQ(182.0, diagram8(nodes));

  delete t1;
  delete t2;
  delete t3;
  delete t4;

}




}