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

  // Expect default storage
  EXPECT_EQ(0, t1.getStorage()[0]);
  EXPECT_EQ(1, t1.getStorage()[1]);
  EXPECT_EQ(2, t1.getStorage()[2]);

  EXPECT_EQ(0, t2.getStorage()[0]);
  EXPECT_EQ(1, t2.getStorage()[1]);

  Tensor t3 = contract(t1,t2,{{2,0}});

  EXPECT_EQ(0, t1.getStorage()[0]);
  EXPECT_EQ(1, t1.getStorage()[1]);
  EXPECT_EQ(2, t1.getStorage()[2]);

  EXPECT_EQ(0, t2.getStorage()[0]);
  EXPECT_EQ(1, t2.getStorage()[1]);

  EXPECT_EQ(0, t3.getStorage()[0]);
  EXPECT_EQ(2, t3.getStorage()[1]);
  EXPECT_EQ(1, t3.getStorage()[2]);

}

TEST(ContractionStorageRules, TwoTensorThreeIndexWithNumberOutput) {
  Tensor t1(3,2);
  Tensor t2(3,2);

  // Expect default storage
  EXPECT_EQ(0, t1.getStorage()[0]);
  EXPECT_EQ(1, t1.getStorage()[1]);
  EXPECT_EQ(2, t1.getStorage()[2]);

  EXPECT_EQ(0, t2.getStorage()[0]);
  EXPECT_EQ(1, t2.getStorage()[1]);
  EXPECT_EQ(2, t2.getStorage()[2]);

  contract(t1,t2,{{2,0},{1,1},{0,2}});

  EXPECT_EQ(0, t1.getStorage()[0]);
  EXPECT_EQ(1, t1.getStorage()[1]);
  EXPECT_EQ(2, t1.getStorage()[2]);

  EXPECT_EQ(0, t2.getStorage()[0]);
  EXPECT_EQ(1, t2.getStorage()[1]);
  EXPECT_EQ(2, t2.getStorage()[2]);


}

TEST(ContractionStorageRules, OneTensorWithTensorOutput) {
  Tensor t1(5,2);

  // Expect default storage
  EXPECT_EQ(0, t1.getStorage()[0]);
  EXPECT_EQ(1, t1.getStorage()[1]);
  EXPECT_EQ(2, t1.getStorage()[2]);
  EXPECT_EQ(3, t1.getStorage()[3]);
  EXPECT_EQ(4, t1.getStorage()[4]);

  Tensor t2 = contract(t1,{{0,2}});

  EXPECT_EQ(0, t1.getStorage()[0]);
  EXPECT_EQ(2, t1.getStorage()[1]);
  EXPECT_EQ(1, t1.getStorage()[2]);
  EXPECT_EQ(3, t1.getStorage()[3]);
  EXPECT_EQ(4, t1.getStorage()[4]);

  EXPECT_EQ(0, t2.getStorage()[0]);
  EXPECT_EQ(1, t2.getStorage()[1]);
  EXPECT_EQ(2, t2.getStorage()[2]);


}

TEST(ContractionStorageRules, OneTensorWithNumberOutput) {
  Tensor t1(4,2);

  // Expect default storage
  EXPECT_EQ(0, t1.getStorage()[0]);
  EXPECT_EQ(1, t1.getStorage()[1]);
  EXPECT_EQ(2, t1.getStorage()[2]);
  EXPECT_EQ(3, t1.getStorage()[3]);

  contract(t1,{{0,2},{1,3}});

  EXPECT_EQ(0, t1.getStorage()[0]);
  EXPECT_EQ(2, t1.getStorage()[1]);
  EXPECT_EQ(1, t1.getStorage()[2]);
  EXPECT_EQ(3, t1.getStorage()[3]);


}

}