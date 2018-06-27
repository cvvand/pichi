#include "gtest/gtest.h"
#include "diagrams.h"
#include "tensor.h"

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

  }

  vector<Tensor> tensors;

};

TEST_F(ComputeTest, Diagram0) {
  EXPECT_EQ(1.0, compute(Graph("0aa"),tensors));
}

TEST_F(ComputeTest, TwoTimesDiagram0) {
  EXPECT_EQ(-2.0, compute(Graph("0aa1bb"),tensors));
}

TEST_F(ComputeTest, Diagram1) {
  EXPECT_EQ(18.0, compute(Graph("0ab1ab"),tensors));
}

TEST_F(ComputeTest, Diagram2) {
  EXPECT_EQ(57.0, compute(Graph("0ab2bc1ac"),tensors));
}

TEST_F(ComputeTest, Diagram3) {
  EXPECT_EQ(-102.0, compute(Graph("0ab2dc1ac3bd"),tensors));
}

TEST_F(ComputeTest, Diagram4) {
  EXPECT_EQ(8.0, compute(Graph("4abc5abc"),tensors));
}

TEST_F(ComputeTest, Diagram5) {
  EXPECT_EQ(16.0, compute(Graph("4bdc5abc2da"),tensors));
}

TEST_F(ComputeTest, Diagram6) {
  EXPECT_EQ(373.0, compute(Graph("4abd7abe0df3ef"),tensors));
}

TEST_F(ComputeTest, Diagram7) {
  EXPECT_EQ(-32.0, compute(Graph("4abc5dbe2ae1cd"),tensors));
}

TEST_F(ComputeTest, Diagram8) {
  EXPECT_EQ(-499.0, compute(Graph("4adc5aec7efg6fgd"),tensors));
}

}

