#include "diagrams.h"
#include "gtest/gtest.h"

using namespace pichi;
using namespace std;

namespace {

class ComputeTest : public ::testing::Test {
protected:

  ComputeTest() {

  }

  virtual ~ComputeTest() {

  }

  virtual void SetUp() {
    Tensor t1 = Tensor(2,2);
    cdouble data[4];
    data[0] = 2.0; data[1] = 3.0;
    data[2] = 3.0; data[3] = -1.0;
    t1.setSlice({-1,-1},data);

    Tensor t2 = Tensor(2,2);
    data[0] = 2.0; data[1] = -2.0;
    data[2] = 5.0; data[3] = -5.0;
    t2.setSlice({-1,-1},data);

    Tensor t3 = Tensor(2,2);
    data[0] = -1.0; data[1] = 0.0;
    data[2] = 1.0; data[3] = 3.0;
    t3.setSlice({-1,-1},data);

    Tensor t4 = Tensor(2,2);
    data[0] = -3.0; data[1] = 2.0;
    data[2] = 1.0; data[3] = -2.0;
    t4.setSlice({-1,-1},data);

    Tensor t5 = Tensor(3,2);
    data[0] = -2.0; data[1] = 3.0;
    data[2] = 3.0; data[3] = -1.0;
    t5.setSlice({-1,-1,0}, data);
    data[0] = -2.0; data[1] = 1.0;
    data[2] = 5.0;  data[3] = 1.0;
    t5.setSlice({-1,-1,1}, data);

    Tensor t6 = Tensor(3,2);
    data[0] = -4.0; data[1] = -1.0;
    data[2] = 2.0;  data[3] = -1.0;
    t6.setSlice({-1,-1,0}, data);
    data[0] = 1.0; data[1] = 1.0;
    data[2] = 2.0; data[3] = 3.0;
    t6.setSlice({-1,-1,1}, data);

    Tensor t7 = Tensor(3,2);
    data[0] = 0.0; data[1] = 2.0;
    data[2] = 5.0; data[3] = 1.0;
    t7.setSlice({-1,-1,0}, data);
    data[0] = 1.0; data[1] = -1.0;
    data[2] = 3.0;  data[3] = -2.0;
    t7.setSlice({-1,-1,1}, data);

    Tensor t8 = Tensor(3,2);
    data[0] = 8.0; data[1] = 0.0;
    data[2] = -3.0;  data[3] = 0.0;
    t8.setSlice({-1,-1,0}, data);
    data[0] = 2.0; data[1] = -4.0;
    data[2] = 3.0; data[3] = 3.0;
    t8.setSlice({-1,-1,1}, data);

    Tensor t9 = Tensor(2,2);
    data[0] = 3.0; data[1] = 2.0;
    data[2] = 2.0; data[3] = -4.0;
    t9.setSlice({-1,-1},data);

    tensors.push_back(t1); //#0
    tensors.push_back(t2);
    tensors.push_back(t3); //#2
    tensors.push_back(t4);
    tensors.push_back(t5); //#4
    tensors.push_back(t6);
    tensors.push_back(t7); //#6
    tensors.push_back(t8);
    tensors.push_back(Tensor(2,2)); // Just a filler
    tensors.push_back(Tensor(2,2)); // Just a filler
    tensors.push_back(t9); //#10

  }

  virtual void TearDown() {

  }

  vector<Tensor> tensors;
};

TEST_F(ComputeTest, SingleMesonIsDiagram0) {
  ASSERT_EQ(-3.0, compute("1aa", tensors));
}

TEST_F(ComputeTest, TwoDisconnectedDiagram0) {
  ASSERT_EQ(-2.0, compute("1aa0bb", tensors));
}

TEST_F(ComputeTest, TwoMesonsDiagram1) {
  ASSERT_EQ(-2.0, compute("2ab0ab", tensors));
}

TEST_F(ComputeTest, TwoTimesTwoMesonsDiagram1) {
  ASSERT_EQ(30.0, compute("2ab0ab1cd10cd", tensors));
}

TEST_F(ComputeTest, ThreeMesonsDiagram2) {
  ASSERT_EQ(25.0, compute("2ab0ac3bc", tensors));
}

TEST_F(ComputeTest, FourMesonsDiagram3) {
  ASSERT_EQ(-67.0, compute("2ab0ac3bd1cd", tensors));
}

TEST_F(ComputeTest, TwoBaryonsDiagram4) {
  ASSERT_EQ(30.0, compute("6abc4abc", tensors));
}

TEST_F(ComputeTest, TwoBaryonsOneMesonDiagram5) {
  ASSERT_EQ(132.0, compute("6abc4abd0cd", tensors));
}

TEST_F(ComputeTest, TwoBaryonsTwoMesonsDiagram6) {
  ASSERT_EQ(32.0, compute("4abc6abd1ce3de", tensors));
}

TEST_F(ComputeTest, TwoBaryonsTwoMesonsDiagram7) {
  ASSERT_EQ(22.0, compute("4abc6ade1bd3ce", tensors));
}

TEST_F(ComputeTest, FourBaryonsDiagram8) {
  ASSERT_EQ(-458.0, compute("4abc6abd5def7cef", tensors));
}



}


