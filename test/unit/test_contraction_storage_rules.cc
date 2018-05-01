#include "contraction.h"
#include "gtest/gtest.h"

/*
 * Unit tests of the storage rules implemented in CONTRACTION.CC
 */

using namespace pichi;
using namespace std;

namespace {

TEST(ContractionStorageRules, TwoTensorSingleIndexWithTensorOutput) {
  Tensor t1(3,2);
  Tensor t2(2,2);

  Contraction c;
  c.addTensor(0, t1);
  c.addTensor(1, t2);

  // Expect default storage
  EXPECT_EQ(0, c.getTensor(0).getStorage()[0]);
  EXPECT_EQ(1, c.getTensor(0).getStorage()[1]);
  EXPECT_EQ(2, c.getTensor(0).getStorage()[2]);

  EXPECT_EQ(0, c.getTensor(1).getStorage()[0]);
  EXPECT_EQ(1, c.getTensor(1).getStorage()[1]);

  c.contract(0,1,{{2,0}},3);

  EXPECT_EQ(0, c.getTensor(0).getStorage()[0]);
  EXPECT_EQ(2, c.getTensor(0).getStorage()[1]);
  EXPECT_EQ(1, c.getTensor(0).getStorage()[2]);

  EXPECT_EQ(0, c.getTensor(1).getStorage()[0]);
  EXPECT_EQ(1, c.getTensor(1).getStorage()[1]);

  EXPECT_EQ(0, c.getTensor(3).getStorage()[0]);
  EXPECT_EQ(2, c.getTensor(3).getStorage()[1]);
  EXPECT_EQ(1, c.getTensor(3).getStorage()[2]);

}

TEST(ContractionStorageRules, TwoTensorThreeIndexWithNumberOutput) {
  Tensor t1(3,2);
  Tensor t2(3,2);

  Contraction c;
  c.addTensor(0, t1);
  c.addTensor(1, t2);

  // Expect default storage
  EXPECT_EQ(0, c.getTensor(0).getStorage()[0]);
  EXPECT_EQ(1, c.getTensor(0).getStorage()[1]);
  EXPECT_EQ(2, c.getTensor(0).getStorage()[2]);

  EXPECT_EQ(0, c.getTensor(1).getStorage()[0]);
  EXPECT_EQ(1, c.getTensor(1).getStorage()[1]);
  EXPECT_EQ(2, c.getTensor(1).getStorage()[2]);

  c.contract(0,1,{{2,0},{1,1},{0,2}});

  EXPECT_EQ(1, c.getTensor(0).getStorage()[0]);
  EXPECT_EQ(2, c.getTensor(0).getStorage()[1]);
  EXPECT_EQ(0, c.getTensor(0).getStorage()[2]);

  EXPECT_EQ(0, c.getTensor(1).getStorage()[0]);
  EXPECT_EQ(1, c.getTensor(1).getStorage()[1]);
  EXPECT_EQ(2, c.getTensor(1).getStorage()[2]);


}

TEST(ContractionStorageRules, OneTensorWithTensorOutput) {
  Tensor t1(5,2);

  Contraction c;
  c.addTensor(0, t1);

  // Expect default storage
  EXPECT_EQ(0, c.getTensor(0).getStorage()[0]);
  EXPECT_EQ(1, c.getTensor(0).getStorage()[1]);
  EXPECT_EQ(2, c.getTensor(0).getStorage()[2]);
  EXPECT_EQ(3, c.getTensor(0).getStorage()[3]);
  EXPECT_EQ(4, c.getTensor(0).getStorage()[4]);


  c.contract(0,{{0,2}},1);

  EXPECT_EQ(0, c.getTensor(0).getStorage()[0]);
  EXPECT_EQ(2, c.getTensor(0).getStorage()[1]);
  EXPECT_EQ(1, c.getTensor(0).getStorage()[2]);
  EXPECT_EQ(3, c.getTensor(0).getStorage()[3]);
  EXPECT_EQ(4, c.getTensor(0).getStorage()[4]);

  EXPECT_EQ(0, c.getTensor(1).getStorage()[0]);
  EXPECT_EQ(1, c.getTensor(1).getStorage()[1]);
  EXPECT_EQ(2, c.getTensor(1).getStorage()[2]);


}

TEST(ContractionStorageRules, OneTensorWithNumberOutput) {
  Tensor t1(4,2);

  Contraction c;
  c.addTensor(0, t1);

  // Expect default storage
  EXPECT_EQ(0, c.getTensor(0).getStorage()[0]);
  EXPECT_EQ(1, c.getTensor(0).getStorage()[1]);
  EXPECT_EQ(2, c.getTensor(0).getStorage()[2]);
  EXPECT_EQ(3, c.getTensor(0).getStorage()[3]);

  c.contract(0,{{0,2},{1,3}});

  EXPECT_EQ(0, c.getTensor(0).getStorage()[0]);
  EXPECT_EQ(2, c.getTensor(0).getStorage()[1]);
  EXPECT_EQ(1, c.getTensor(0).getStorage()[2]);
  EXPECT_EQ(3, c.getTensor(0).getStorage()[3]);


}

}