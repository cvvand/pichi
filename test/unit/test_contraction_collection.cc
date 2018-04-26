#include "contraction.h"

#include "gtest/gtest.h"

/*
 * Unit tests of the collection properties of the Contraction class,
 * implemented in CONTRACTION.CC
 */

using namespace pichi;
using namespace std;

namespace {

TEST(ContractionCollection, NoDoubleInsert) {
  Tensor t1(2,2);
  Tensor t2(3,2);
  Contraction<char> c;
  c.addTensor('A',t1);
  c.addTensor('A',t2);
  EXPECT_EQ(2, c.getTensor('A').rank());
}

TEST(ContractionCollection, ErrorOnNonExistingTensor) {
  Tensor t1(2,2);
  Contraction<char> c;
  c.addTensor('A',t1);
  EXPECT_THROW(c.getTensor('B'), out_of_range);
}

TEST(ContractionCollection, AlterTensorAfterInsertion) {
  Tensor t1(2,2);
  cdouble data[4];
  data[0] = 3.0; data[1] = 3.0; data[2] = 3.0; data[3] = -1.0;
  t1.setSlice({-1,-1},data);

  Contraction<char> c;
  c.addTensor('A', t1);
  EXPECT_EQ(2.0, c.contract('A',{{0,1}}));

  cdouble data2[4];
  data2[0] = 1.0; data2[1] = 0.0; data2[2] = 0.0; data2[3] = -1.0;
  c.getTensor('A').setSlice({-1,-1}, data2);

  EXPECT_EQ(0.0, c.contract('A',{{0,1}}));
}

TEST(ContractionCollection, RemoveTensorAfterInsertion) {
  Tensor t1(2,2);
  cdouble data[4];
  data[0] = 3.0; data[1] = 3.0; data[2] = 3.0; data[3] = -1.0;
  t1.setSlice({-1,-1},data);

  Contraction<char> c;
  c.addTensor('A', t1);
  EXPECT_EQ(2.0, c.contract('A',{{0,1}}));

  c.removeTensor('A');
  EXPECT_THROW(c.contract('A',{{0,1}}), invalid_argument);
}


}
