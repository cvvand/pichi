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

  Contraction<char> c;
  c.addTensor('A', t1);
  c.addTensor('B', t2);

  // Expect default storage
  EXPECT_EQ(0, c.getTensor('A').getStorage()[0]);
  EXPECT_EQ(1, c.getTensor('A').getStorage()[1]);
  EXPECT_EQ(2, c.getTensor('A').getStorage()[2]);

  EXPECT_EQ(0, c.getTensor('B').getStorage()[0]);
  EXPECT_EQ(1, c.getTensor('B').getStorage()[1]);

  c.contract('A','B',{{2,0}},'D');

  EXPECT_EQ(0, c.getTensor('A').getStorage()[0]);
  EXPECT_EQ(2, c.getTensor('A').getStorage()[1]);
  EXPECT_EQ(1, c.getTensor('A').getStorage()[2]);

  EXPECT_EQ(0, c.getTensor('B').getStorage()[0]);
  EXPECT_EQ(1, c.getTensor('B').getStorage()[1]);

  EXPECT_EQ(0, c.getTensor('D').getStorage()[0]);
  EXPECT_EQ(2, c.getTensor('D').getStorage()[1]);
  EXPECT_EQ(1, c.getTensor('D').getStorage()[2]);

}

TEST(ContractionStorageRules, TwoTensorThreeIndexWithNumberOutput) {
  Tensor t1(3,2);
  Tensor t2(3,2);

  Contraction<char> c;
  c.addTensor('A', t1);
  c.addTensor('B', t2);

  // Expect default storage
  EXPECT_EQ(0, c.getTensor('A').getStorage()[0]);
  EXPECT_EQ(1, c.getTensor('A').getStorage()[1]);
  EXPECT_EQ(2, c.getTensor('A').getStorage()[2]);

  EXPECT_EQ(0, c.getTensor('B').getStorage()[0]);
  EXPECT_EQ(1, c.getTensor('B').getStorage()[1]);
  EXPECT_EQ(2, c.getTensor('B').getStorage()[2]);

  c.contract('A','B',{{2,0},{1,1},{0,2}});

  EXPECT_EQ(1, c.getTensor('A').getStorage()[0]);
  EXPECT_EQ(2, c.getTensor('A').getStorage()[1]);
  EXPECT_EQ(0, c.getTensor('A').getStorage()[2]);

  EXPECT_EQ(0, c.getTensor('B').getStorage()[0]);
  EXPECT_EQ(1, c.getTensor('B').getStorage()[1]);
  EXPECT_EQ(2, c.getTensor('B').getStorage()[2]);


}

TEST(ContractionStorageRules, OneTensorWithTensorOutput) {
  Tensor t1(5,2);

  Contraction<char> c;
  c.addTensor('A', t1);

  // Expect default storage
  EXPECT_EQ(0, c.getTensor('A').getStorage()[0]);
  EXPECT_EQ(1, c.getTensor('A').getStorage()[1]);
  EXPECT_EQ(2, c.getTensor('A').getStorage()[2]);
  EXPECT_EQ(3, c.getTensor('A').getStorage()[3]);
  EXPECT_EQ(4, c.getTensor('A').getStorage()[4]);


  c.contract('A',{{0,2}},'B');

  EXPECT_EQ(0, c.getTensor('A').getStorage()[0]);
  EXPECT_EQ(2, c.getTensor('A').getStorage()[1]);
  EXPECT_EQ(1, c.getTensor('A').getStorage()[2]);
  EXPECT_EQ(3, c.getTensor('A').getStorage()[3]);
  EXPECT_EQ(4, c.getTensor('A').getStorage()[4]);

  EXPECT_EQ(0, c.getTensor('B').getStorage()[0]);
  EXPECT_EQ(1, c.getTensor('B').getStorage()[1]);
  EXPECT_EQ(2, c.getTensor('B').getStorage()[2]);


}

TEST(ContractionStorageRules, OneTensorWithNumberOutput) {
  Tensor t1(4,2);

  Contraction<char> c;
  c.addTensor('A', t1);

  // Expect default storage
  EXPECT_EQ(0, c.getTensor('A').getStorage()[0]);
  EXPECT_EQ(1, c.getTensor('A').getStorage()[1]);
  EXPECT_EQ(2, c.getTensor('A').getStorage()[2]);
  EXPECT_EQ(3, c.getTensor('A').getStorage()[3]);

  c.contract('A',{{0,2},{1,3}});

  EXPECT_EQ(0, c.getTensor('A').getStorage()[0]);
  EXPECT_EQ(2, c.getTensor('A').getStorage()[1]);
  EXPECT_EQ(1, c.getTensor('A').getStorage()[2]);
  EXPECT_EQ(3, c.getTensor('A').getStorage()[3]);


}

}