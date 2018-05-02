#include "contraction.h"
#include "gtest/gtest.h"

/*
 * Unit tests of the contraction logic defined in CONTRACTION.CC
 */

using namespace pichi;
using namespace std;

namespace {

TEST(ContractErrors, SingleTensorToNumberErrorIfTooFewContractedIndices) {
  Tensor t(4,2);
  EXPECT_THROW(contract(t,{{0,1}}), invalid_argument);
}

TEST(ContractErrors, SingleTensorToNumberErrorIfTooManyContractedIndices) {
  Tensor t(2,2);
  EXPECT_THROW(contract(t,{{0,1},{2,3}}), invalid_argument);
}

TEST(ContractErrors, SingleTensorToNumberErrorIfContractedIndicesAreRepeated) {
  Tensor t(4,2);
  EXPECT_THROW(contract(t,{{0,1},{1,2}}), invalid_argument);
}

TEST(ContractErrors, SingleTensorToNumberErrorIfContractedAreOutOfRank) {
  Tensor t(4,2);
  EXPECT_THROW(contract(t,{{0,1},{2,4}}), invalid_argument);
  EXPECT_THROW(contract(t,{{-1,1},{2,4}}), invalid_argument);
}


TEST(ContractErrors, TwoTensorToNumberErrorIfTooFewContractedIndices) {
  Tensor t1(3,2);
  Tensor t2(3,2);
  EXPECT_THROW(contract(t1,t2,{{0,0},{1,1}}), invalid_argument);
}

TEST(ContractErrors, TwoTensorToNumberErrorIfTooManyContractedIndices) {
  Tensor t1(2,2);
  Tensor t2(2,2);
  EXPECT_THROW(contract(t1,t2,{{0,0},{1,1},{2,2}}), invalid_argument);
}

TEST(ContractErrors, TwoTensorToNumberErrorIfContractedIndicesAreRepeated) {
  Tensor t1(2,2);
  Tensor t2(2,2);
  EXPECT_THROW(contract(t1,t2,{{0,0},{0,1}}), invalid_argument);
}

TEST(ContractErrors, TwoTensorToNumberErrorIfContractedIndicesAreOutOfRank) {
  Tensor t1(2,2);
  Tensor t2(2,2);
  EXPECT_THROW(contract(t1,t2,{{0,0},{-1,1}}), invalid_argument);
  EXPECT_THROW(contract(t1,t2,{{0,2},{1,1}}), invalid_argument);
}

TEST(ContractErrors, TwoTensorToNumberErrorIfTensorsDoNotHaveEqualRank) {
  Tensor t1(2,2);
  Tensor t2(3,2);
  EXPECT_THROW(contract(t1,t2,{{0,1},{1,0}}), invalid_argument);
}



TEST(ContractErrors, SingleTensorToTensorErrorIfNoContractedIndices) {
  Tensor t(4,2);
  Tensor t2(4,2);
  EXPECT_THROW(contract(t,{},t2), invalid_argument);
}

TEST(ContractErrors, SingleTensorToTensorErrorIfTooManyContractedIndices) {
  Tensor t(4,2);
  Tensor t2(2,2);
  EXPECT_THROW(contract(t,{{0,1},{2,3}},t2), invalid_argument);
}

TEST(ContractErrors, SingleTensorToTensorErrorIfContractedIndicesAreRepeated) {
  Tensor t(4,2);
  Tensor t2(2,2);
  EXPECT_THROW(contract(t,{{0,0}},t2), invalid_argument);
}

TEST(ContractErrors, SingleTensorToTensorErrorIfContractedAreOutOfRank) {
  Tensor t(4,2);
  Tensor t1(2,2);
  EXPECT_THROW(contract(t,{{0,-1}},t1), invalid_argument);
  EXPECT_THROW(contract(t,{{4,1}},t1), invalid_argument);
}



TEST(ContractErrors, DoubleTensorToTensorErrorIfNoContractedIndices) {
  Tensor t1(4,2);
  Tensor t2(2,2);
  Tensor t3(6,2);
  EXPECT_THROW(contract(t1,t2,{},t3), invalid_argument);
}

TEST(ContractErrors, DoubleTensorToTensorErrorIfTooManyContractedIndices) {
  Tensor t1(2,2);
  Tensor t2(2,2);
  Tensor t3(2,2);
  EXPECT_THROW(contract(t1,t2,{{0,1},{1,0}},t3), invalid_argument);
}

TEST(ContractErrors, DoubleTensorToTensorErrorIfContractedIndicesAreRepeated) {
  Tensor t1(4,2);
  Tensor t2(2,2);
  Tensor t3(2,2);
  EXPECT_THROW(contract(t1,t2,{{0,0},{0,1}},t3), invalid_argument);
  EXPECT_THROW(contract(t1,t2,{{0,0},{1,0}},t3), invalid_argument);
}

TEST(ContractErrors, DoubleTensorToTensorErrorIfContractedAreOutOfRank) {
  Tensor t1(4,2);
  Tensor t2(2,2);
  Tensor t3(4,2);
  EXPECT_THROW(contract(t1,t2,{{0,-1}},t3), invalid_argument);
  EXPECT_THROW(contract(t1,t2,{{4,1}},t3), invalid_argument);
}


}