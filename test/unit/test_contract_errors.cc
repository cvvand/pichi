#include "pichi/contraction.h"
#include "gtest/gtest.h"

/*
 * Unit tests of the contraction logic defined in CONTRACTION.CC
 */

using namespace pichi;
using namespace std;

namespace {

TEST(ContractErrors, SingleTensorErrorIfTooManyContractedIndices) {
  Tensor t(2,2);
  EXPECT_THROW(contract(t,{{0,1},{2,3}}), invalid_argument);
}

TEST(ContractErrors, SingleTensorErrorIfContractedIndicesAreRepeated) {
  Tensor t(4,2);
  EXPECT_THROW(contract(t,{{0,1},{1,2}}), invalid_argument);
}

TEST(ContractErrors, SingleTensorErrorIfContractedAreOutOfRank) {
  Tensor t(4,2);
  EXPECT_THROW(contract(t,{{0,1},{2,4}}), invalid_argument);
  EXPECT_THROW(contract(t,{{-1,1},{2,4}}), invalid_argument);
}

TEST(ContractErrors, SingleTensorErrorIfNoContractedIndices) {
  Tensor t(4,2);
  EXPECT_THROW(contract(t,{}), invalid_argument);
}


TEST(ContractErrors, TwoTensorErrorIfTooManyContractedIndices) {
  Tensor t1(2,2);
  Tensor t2(2,2);
  EXPECT_THROW(contract(t1,t2,{{0,0},{1,1},{2,2}}), invalid_argument);
}

TEST(ContractErrors, TwoTensorErrorIfContractedIndicesAreRepeated) {
  Tensor t1(2,2);
  Tensor t2(2,2);
  EXPECT_THROW(contract(t1,t2,{{0,0},{0,1}}), invalid_argument);
}

TEST(ContractErrors, TwoTensorErrorIfContractedIndicesAreOutOfRank) {
  Tensor t1(2,2);
  Tensor t2(2,2);
  EXPECT_THROW(contract(t1,t2,{{0,0},{-1,1}}), invalid_argument);
  EXPECT_THROW(contract(t1,t2,{{0,2},{1,1}}), invalid_argument);
}

TEST(ContractErrors, TwoTensorErrorIfNoContractedIndices) {
  Tensor t1(4,2);
  Tensor t2(2,2);
  EXPECT_THROW(contract(t1,t2,{}), invalid_argument);
}

TEST(ContractErrors, TwoTensorErrorIfContractedAreOutOfRank) {
  Tensor t1(4,2);
  Tensor t2(2,2);

  EXPECT_THROW(contract(t1,t2,{{0,-1}}), invalid_argument);
  EXPECT_THROW(contract(t1,t2,{{4,1}}), invalid_argument);
}


}