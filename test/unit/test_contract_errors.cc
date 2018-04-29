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
  Contraction<char> c;
  c.addTensor('A',t);
  EXPECT_THROW(c.contract('A',{{0,1}}), invalid_argument);
}

TEST(ContractErrors, SingleTensorToNumberErrorIfTooManyContractedIndices) {
  Tensor t(2,2);
  Contraction<char> c;
  c.addTensor('A',t);
  EXPECT_THROW(c.contract('A',{{0,1},{2,3}}), invalid_argument);
}

TEST(ContractErrors, SingleTensorToNumberErrorIfContractedIndicesAreRepeated) {
  Tensor t(4,2);
  Contraction<char> c;
  c.addTensor('A',t);
  EXPECT_THROW(c.contract('A',{{0,1},{1,2}}), invalid_argument);
}

TEST(ContractErrors, SingleTensorToNumberErrorIfContractedAreOutOfRank) {
  Tensor t(4,2);
  Contraction<char> c;
  c.addTensor('A',t);
  EXPECT_THROW(c.contract('A',{{0,1},{2,4}}), invalid_argument);
  EXPECT_THROW(c.contract('A',{{-1,1},{2,4}}), invalid_argument);
}

TEST(ContractErrors, SingleTensorToNumberErrorIfTensorIsMissing) {
  Tensor t(2,2);
  Contraction<char> c;
  c.addTensor('A',t);
  EXPECT_THROW(c.contract('B',{{0,1}}), invalid_argument);
}





TEST(ContractErrors, TwoTensorToNumberErrorIfTooFewContractedIndices) {
  Tensor t1(3,2);
  Tensor t2(3,2);
  Contraction<char> c;
  c.addTensor('A',t1);
  c.addTensor('B',t2);
  EXPECT_THROW(c.contract('A','B',{{0,0},{1,1}}), invalid_argument);
}

TEST(ContractErrors, TwoTensorToNumberErrorIfTooManyContractedIndices) {
  Tensor t1(2,2);
  Tensor t2(2,2);
  Contraction<char> c;
  c.addTensor('A',t1);
  c.addTensor('B',t2);
  EXPECT_THROW(c.contract('A','B',{{0,0},{1,1},{2,2}}), invalid_argument);
}

TEST(ContractErrors, TwoTensorToNumberErrorIfContractedIndicesAreRepeated) {
  Tensor t1(2,2);
  Tensor t2(2,2);
  Contraction<char> c;
  c.addTensor('A',t1);
  c.addTensor('B',t2);
  EXPECT_THROW(c.contract('A','B',{{0,0},{0,1}}), invalid_argument);
}

TEST(ContractErrors, TwoTensorToNumberErrorIfContractedIndicesAreOutOfRank) {
  Tensor t1(2,2);
  Tensor t2(2,2);
  Contraction<char> c;
  c.addTensor('A',t1);
  c.addTensor('B',t2);
  EXPECT_THROW(c.contract('A','B',{{0,0},{-1,1}}), invalid_argument);
  EXPECT_THROW(c.contract('A','B',{{0,2},{1,1}}), invalid_argument);
}

TEST(ContractErrors, TwoTensorToNumberErrorIfTensorIsMissing) {
  Tensor t1(2,2);
  Tensor t2(2,2);
  Contraction<char> c;
  c.addTensor('A',t1);
  c.addTensor('B',t2);
  EXPECT_THROW(c.contract('A','C',{{0,1},{1,0}}), invalid_argument);
}

TEST(ContractErrors, TwoTensorToNumberErrorIfTensorsDoNotHaveEqualRank) {
  Tensor t1(2,2);
  Tensor t2(3,2);
  Contraction<char> c;
  c.addTensor('A',t1);
  c.addTensor('B',t2);
  EXPECT_THROW(c.contract('A','B',{{0,1},{1,0}}), invalid_argument);
}



TEST(ContractErrors, SingleTensorToTensorErrorIfNoContractedINdices) {
  Tensor t(4,2);
  Contraction<char> c;
  c.addTensor('A',t);
  EXPECT_THROW(c.contract('A',{},'B'), invalid_argument);
}

TEST(ContractErrors, SingleTensorToTensorErrorIfTooManyContractedIndices) {
  Tensor t(4,2);
  Contraction<char> c;
  c.addTensor('A',t);
  EXPECT_THROW(c.contract('A',{{0,1},{2,3}},'B'), invalid_argument);
}

TEST(ContractErrors, SingleTensorToTensorErrorIfContractedIndicesAreRepeated) {
  Tensor t(4,2);
  Contraction<char> c;
  c.addTensor('A',t);
  EXPECT_THROW(c.contract('A',{{0,0}},'B'), invalid_argument);
}

TEST(ContractErrors, SingleTensorToTensorErrorIfContractedAreOutOfRank) {
  Tensor t(4,2);
  Contraction<char> c;
  c.addTensor('A',t);
  EXPECT_THROW(c.contract('A',{{0,-1}},'B'), invalid_argument);
  EXPECT_THROW(c.contract('A',{{4,1}},'B'), invalid_argument);
}

TEST(ContractErrors, SingleTensorToTensorErrorIfTensorIsMissing) {
  Tensor t(4,2);
  Contraction<char> c;
  c.addTensor('A',t);
  EXPECT_THROW(c.contract('B',{{0,1}},'C'), invalid_argument);
}

TEST(ContractErrors, SingleTensorToTensorErrorIfOutputTensorKeyIsAlreadyPresent) {
  Tensor t(4,2);
  Tensor t2(4,2);
  Contraction<char> c;
  c.addTensor('A',t);
  c.addTensor('B',t2);
  EXPECT_THROW(c.contract('A',{{0,1}},'B'), invalid_argument);
}



TEST(ContractErrors, DoubleTensorToTensorErrorIfNoContractedINdices) {
  Tensor t1(4,2);
  Tensor t2(2,2);
  Contraction<char> c;
  c.addTensor('A',t1);
  c.addTensor('B',t2);
  EXPECT_THROW(c.contract('A','B',{},'C'), invalid_argument);
}

TEST(ContractErrors, DoubleTensorToTensorErrorIfTooManyContractedIndices) {
  Tensor t1(2,2);
  Tensor t2(2,2);
  Contraction<char> c;
  c.addTensor('A',t1);
  c.addTensor('B',t2);
  EXPECT_THROW(c.contract('A','B',{{0,1},{1,0}},'C'), invalid_argument);
}

TEST(ContractErrors, DoubleTensorToTensorErrorIfContractedIndicesAreRepeated) {
  Tensor t1(4,2);
  Tensor t2(2,2);
  Contraction<char> c;
  c.addTensor('A',t1);
  c.addTensor('B',t2);
  EXPECT_THROW(c.contract('A','B',{{0,0},{0,1}},'C'), invalid_argument);
  EXPECT_THROW(c.contract('A','B',{{0,0},{1,0}},'C'), invalid_argument);
}

TEST(ContractErrors, DoubleTensorToTensorErrorIfContractedAreOutOfRank) {
  Tensor t1(4,2);
  Tensor t2(2,2);
  Contraction<char> c;
  c.addTensor('A',t1);
  c.addTensor('B',t2);
  EXPECT_THROW(c.contract('A','B',{{0,-1}},'C'), invalid_argument);
  EXPECT_THROW(c.contract('A','B',{{4,1}},'C'), invalid_argument);
}

TEST(ContractErrors, DoubleTensorToTensorErrorIfTensorIsMissing) {
  Tensor t1(4,2);
  Tensor t2(2,2);
  Contraction<char> c;
  c.addTensor('A',t1);
  c.addTensor('B',t2);
  EXPECT_THROW(c.contract('A','C',{{0,1}},'D'), invalid_argument);
}

TEST(ContractErrors, DoubleTensorToTensorErrorIfOutputTensorKeyIsAlreadyPresent) {
  Tensor t(4,2);
  Tensor t2(4,2);
  Contraction<char> c;
  c.addTensor('A',t);
  c.addTensor('B',t2);
  EXPECT_THROW(c.contract('A','B',{{0,1}},'A'), invalid_argument);
}



}