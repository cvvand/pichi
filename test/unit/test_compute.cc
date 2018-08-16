#include "gtest/gtest.h"
#include "pichi/pichi.h"

/*
 * Unit tests of the splitToConnected function defined in Graph.cc
 */

using namespace pichi;
using namespace std;

namespace {

class ComputeTest : public ::testing::Test {

protected:

  ComputeTest() {

    Tensor t1(2,2);
    cdouble data[4];
    data[0] = 2.0; data[1] = 3.0;
    data[2] = 3.0; data[3] = -1.0;
    t1.setSlice({-1,-1},data);
    tensors.push_back(t1);

    Tensor t2(2,2);
    data[0] = 2.0; data[1] = -2.0;
    data[2] = 5.0; data[3] = -5.0;
    t2.setSlice({-1,-1},data);
    tensors.push_back(t2);

    Tensor t3(2,2);
    data[0] = -1.0; data[1] = 0.0;
    data[2] = 1.0; data[3] = 3.0;
    t3.setSlice({-1,-1},data);
    tensors.push_back(t3);

    Tensor t4(2,2);
    data[0] = -3.0; data[1] = 2.0;
    data[2] = 1.0; data[3] = -2.0;
    t4.setSlice({-1,-1},data);
    tensors.push_back(t4);

    Tensor t5(3,2);
    data[0] = 2.0; data[1] = 3.0;
    data[2] = 3.0; data[3] = -1.0;
    t5.setSlice({-1,-1,0}, data);
    data[0] = -2.0; data[1] = 1.0;
    data[2] = 5.0;  data[3] = 1.0;
    t5.setSlice({-1,-1,1}, data);
    tensors.push_back(t5);

    Tensor t6(3,2);
    data[0] = -4.0; data[1] = -1.0;
    data[2] = 2.0;  data[3] = -1.0;
    t6.setSlice({-1,-1,0}, data);
    data[0] = 1.0; data[1] = 1.0;
    data[2] = 2.0; data[3] = 3.0;
    t6.setSlice({-1,-1,1}, data);
    tensors.push_back(t6);

    Tensor t7(3,2);
    data[0] = 0.0; data[1] = 2.0;
    data[2] = 5.0; data[3] = 1.0;
    t7.setSlice({-1,-1,0}, data);
    data[0] = 1.0; data[1] = -1.0;
    data[2] = 3.0;  data[3] = -2.0;
    t7.setSlice({-1,-1,1}, data);
    tensors.push_back(t7);

    Tensor t8(3,2);
    data[0] = 8.0; data[1] = 0.0;
    data[2] = -3.0;  data[3] = 0.0;
    t8.setSlice({-1,-1,0}, data);
    data[0] = 2.0; data[1] = -4.0;
    data[2] = 3.0; data[3] = 3.0;
    t8.setSlice({-1,-1,1}, data);
    tensors.push_back(t8);

    Tensor t9(2,2);
    data[0] = 6.0; data[1] = 3.0;
    data[2] = -67.0; data[3] = -1.0;
    t9.setSlice({-1,-1},data);
    tensors.push_back(t9);

  }

  vector<Tensor> tensors;

};

TEST_F(ComputeTest, Diagram0) {
  cdouble r[1]; contract(Graph("0aa"),tensors).getSlice({0},r);
  EXPECT_EQ(1.0, r[0]);
}

TEST_F(ComputeTest, Diagram1) {
  cdouble r[1]; contract(Graph("0ab1ab"),tensors).getSlice({0},r);
  EXPECT_EQ(18.0, r[0]);
}

TEST_F(ComputeTest, Diagram2) {
  cdouble r[1]; contract(Graph("0ab2bc1ac"),tensors).getSlice({0},r);
  EXPECT_EQ(57.0, r[0]);
}

TEST_F(ComputeTest, Diagram3) {
  cdouble r[1]; contract(Graph("0ab2dc1ac3bd"),tensors).getSlice({0},r);
  EXPECT_EQ(-102.0, r[0]);
}

TEST_F(ComputeTest, Diagram4) {
  cdouble r[1]; contract(Graph("4abc5abc"),tensors).getSlice({0},r);
  EXPECT_EQ(8.0, r[0]);
}

TEST_F(ComputeTest, Diagram5) {
  cdouble r[1]; contract(Graph("4bdc5abc2da"),tensors).getSlice({0},r);
  EXPECT_EQ(16.0, r[0]);
}

TEST_F(ComputeTest, Diagram6) {
  cdouble r[1]; contract(Graph("4abd7abe0df3ef"),tensors).getSlice({0},r);
  EXPECT_EQ(373.0, r[0]);
}

TEST_F(ComputeTest, Diagram7) {
  cdouble r[1]; contract(Graph("4abc5dbe2ae1cd"),tensors).getSlice({0},r);
  EXPECT_EQ(-32.0, r[0]);
}

TEST_F(ComputeTest, Diagram8) {
  cdouble r[1]; contract(Graph("4adc5aec7efg6fgd"),tensors).getSlice({0},r);
  EXPECT_EQ(-499.0, r[0]);
}

TEST_F(ComputeTest, UnknownDiagram) {
  cdouble r[1];
  EXPECT_THROW(contract(Graph("1ae2ab3bc4cd9de"),tensors).getSlice({0},r),
               invalid_argument);
}

}

