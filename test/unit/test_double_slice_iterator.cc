#include "slice_iterator.h"
#include "gtest/gtest.h"

/*
 * Unit tests of the iteration logic defined in DOUBLE_SLICE_ITERATOR.CC
 */

using namespace pichi;
using namespace std;

namespace {

TEST(DoubleIterator, Simple) {
  DoubleSliceIterator s(Tensor(3,2), Tensor(3,2), {{1,1},{2,0}});
  vector<int> s1 = s.getSlice1();
  vector<int> s2 = s.getSlice2();
  vector<int> so = s.getSliceOut();

  ASSERT_EQ(3, s1.size());
  ASSERT_EQ(3, s2.size());
  ASSERT_EQ(2, so.size());


  EXPECT_EQ(0, s1[0]); EXPECT_EQ(-1, s1[1]); EXPECT_EQ(-2, s1[2]);
  EXPECT_EQ(-2, s2[0]); EXPECT_EQ(-1, s2[1]); EXPECT_EQ(0, s2[2]);
  EXPECT_EQ(-1, so[0]); EXPECT_EQ(-1, so[1]);

  EXPECT_FALSE(s.nextContracted());
  EXPECT_FALSE(s.nextNonSlicedFree());

  EXPECT_TRUE(s.nextSlicedFree());
  EXPECT_EQ(1, s.getSlice1()[0]);

  EXPECT_TRUE(s.nextSlicedFree());
  EXPECT_EQ(0, s.getSlice1()[0]);
  EXPECT_EQ(1, s.getSlice2()[2]);

  EXPECT_TRUE(s.nextSlicedFree());
  EXPECT_EQ(1, s.getSlice1()[0]);
  EXPECT_EQ(1, s.getSlice2()[2]);

  EXPECT_FALSE(s.nextSlicedFree());
  EXPECT_EQ(0, s.getSlice1()[0]);
  EXPECT_EQ(0, s.getSlice2()[2]);

}

TEST(DoubleIterator, NextContracted) {
  DoubleSliceIterator s(Tensor(7,2), Tensor(7,2), {{3,3},{4,2},{5,1},{6,0}});
  vector<int> s1 = s.getSlice1();
  vector<int> s2 = s.getSlice2();
  vector<int> so = s.getSliceOut();

  ASSERT_EQ(7, s1.size());
  ASSERT_EQ(7, s2.size());
  ASSERT_EQ(6, so.size());


  EXPECT_EQ(0, s1[0]); EXPECT_EQ(0, s1[1]); EXPECT_EQ(0, s1[2]);
  EXPECT_EQ(0, s1[3]); EXPECT_EQ(0, s1[4]); EXPECT_EQ(-1, s1[5]);
  EXPECT_EQ(-2, s1[6]);

  EXPECT_EQ(-2, s2[0]); EXPECT_EQ(-1, s2[1]); EXPECT_EQ(0, s2[2]);
  EXPECT_EQ(0, s2[3]); EXPECT_EQ(0, s2[4]); EXPECT_EQ(0, s2[5]);
  EXPECT_EQ(0, s2[6]);

  EXPECT_EQ(-1, so[0]); EXPECT_EQ(-1, so[1]); EXPECT_EQ(0, so[2]);
  EXPECT_EQ(0, so[3]); EXPECT_EQ(0, so[4]); EXPECT_EQ(0, so[5]);

  EXPECT_TRUE(s.nextContracted());
  EXPECT_EQ(1, s.getSlice1()[3]);
  EXPECT_EQ(1, s.getSlice2()[3]);

  EXPECT_TRUE(s.nextContracted());
  EXPECT_EQ(0, s.getSlice1()[3]);
  EXPECT_EQ(0, s.getSlice2()[3]);
  EXPECT_EQ(1, s.getSlice1()[4]);
  EXPECT_EQ(1, s.getSlice2()[2]);

  EXPECT_TRUE(s.nextContracted());
  EXPECT_EQ(1, s.getSlice1()[3]);
  EXPECT_EQ(1, s.getSlice2()[3]);
  EXPECT_EQ(1, s.getSlice1()[4]);
  EXPECT_EQ(1, s.getSlice2()[2]);

  EXPECT_FALSE(s.nextContracted());
  EXPECT_EQ(0, s.getSlice1()[3]);
  EXPECT_EQ(0, s.getSlice2()[3]);
  EXPECT_EQ(0, s.getSlice1()[4]);
  EXPECT_EQ(0, s.getSlice2()[2]);

}

TEST(DoubleIterator, NextSlicedFree) {
  DoubleSliceIterator s(Tensor(7,2), Tensor(7,2), {{3,3},{4,2},{5,1},{6,0}});
  vector<int> s1 = s.getSlice1();
  vector<int> s2 = s.getSlice2();
  vector<int> so = s.getSliceOut();

  ASSERT_EQ(7, s1.size());
  ASSERT_EQ(7, s2.size());
  ASSERT_EQ(6, so.size());


  EXPECT_EQ(0, s1[0]); EXPECT_EQ(0, s1[1]); EXPECT_EQ(0, s1[2]);
  EXPECT_EQ(0, s1[3]); EXPECT_EQ(0, s1[4]); EXPECT_EQ(-1, s1[5]);
  EXPECT_EQ(-2, s1[6]);

  EXPECT_EQ(-2, s2[0]); EXPECT_EQ(-1, s2[1]); EXPECT_EQ(0, s2[2]);
  EXPECT_EQ(0, s2[3]); EXPECT_EQ(0, s2[4]); EXPECT_EQ(0, s2[5]);
  EXPECT_EQ(0, s2[6]);

  EXPECT_EQ(-1, so[0]); EXPECT_EQ(-1, so[1]); EXPECT_EQ(0, so[2]);
  EXPECT_EQ(0, so[3]); EXPECT_EQ(0, so[4]); EXPECT_EQ(0, so[5]);

  EXPECT_TRUE(s.nextSlicedFree());
  EXPECT_EQ(1, s.getSlice1()[0]);

  EXPECT_TRUE(s.nextSlicedFree());
  EXPECT_EQ(0, s.getSlice1()[0]);
  EXPECT_EQ(1, s.getSlice1()[1]);

  EXPECT_TRUE(s.nextSlicedFree());
  EXPECT_EQ(1, s.getSlice1()[0]);
  EXPECT_EQ(1, s.getSlice1()[1]);

  EXPECT_FALSE(s.nextSlicedFree());
  EXPECT_EQ(0, s.getSlice1()[0]);
  EXPECT_EQ(0, s.getSlice1()[1]);

}

TEST(DoubleIterator, NextNonSlicedFree) {
  DoubleSliceIterator s(Tensor(7,2), Tensor(7,2), {{3,3},{4,2},{5,1},{6,0}});
  vector<int> s1 = s.getSlice1();
  vector<int> s2 = s.getSlice2();
  vector<int> so = s.getSliceOut();

  ASSERT_EQ(7, s1.size());
  ASSERT_EQ(7, s2.size());
  ASSERT_EQ(6, so.size());


  EXPECT_EQ(0, s1[0]); EXPECT_EQ(0, s1[1]); EXPECT_EQ(0, s1[2]);
  EXPECT_EQ(0, s1[3]); EXPECT_EQ(0, s1[4]); EXPECT_EQ(-1, s1[5]);
  EXPECT_EQ(-2, s1[6]);

  EXPECT_EQ(-2, s2[0]); EXPECT_EQ(-1, s2[1]); EXPECT_EQ(0, s2[2]);
  EXPECT_EQ(0, s2[3]); EXPECT_EQ(0, s2[4]); EXPECT_EQ(0, s2[5]);
  EXPECT_EQ(0, s2[6]);

  EXPECT_EQ(-1, so[0]); EXPECT_EQ(-1, so[1]); EXPECT_EQ(0, so[2]);
  EXPECT_EQ(0, so[3]); EXPECT_EQ(0, so[4]); EXPECT_EQ(0, so[5]);

  EXPECT_TRUE(s.nextNonSlicedFree());
  EXPECT_EQ(1, s.getSlice1()[2]);
  EXPECT_EQ(1, s.getSliceOut()[2]);

  EXPECT_TRUE(s.nextNonSlicedFree());
  EXPECT_EQ(0, s.getSlice1()[2]);
  EXPECT_EQ(0, s.getSliceOut()[2]);
  EXPECT_EQ(1, s.getSlice2()[4]);
  EXPECT_EQ(1, s.getSliceOut()[3]);

  EXPECT_TRUE(s.nextNonSlicedFree());
  EXPECT_EQ(1, s.getSlice1()[2]);
  EXPECT_EQ(1, s.getSliceOut()[2]);
  EXPECT_EQ(1, s.getSlice2()[4]);
  EXPECT_EQ(1, s.getSliceOut()[3]);

  EXPECT_TRUE(s.nextNonSlicedFree());
  EXPECT_EQ(0, s.getSlice1()[2]);
  EXPECT_EQ(0, s.getSliceOut()[2]);
  EXPECT_EQ(0, s.getSlice2()[4]);
  EXPECT_EQ(0, s.getSliceOut()[3]);
  EXPECT_EQ(1, s.getSlice2()[5]);
  EXPECT_EQ(1, s.getSliceOut()[4]);

  EXPECT_TRUE(s.nextNonSlicedFree());
  EXPECT_EQ(1, s.getSlice1()[2]);
  EXPECT_EQ(1, s.getSliceOut()[2]);
  EXPECT_EQ(0, s.getSlice2()[4]);
  EXPECT_EQ(0, s.getSliceOut()[3]);
  EXPECT_EQ(1, s.getSlice2()[5]);
  EXPECT_EQ(1, s.getSliceOut()[4]);

  EXPECT_TRUE(s.nextNonSlicedFree());
  EXPECT_EQ(0, s.getSlice1()[2]);
  EXPECT_EQ(0, s.getSliceOut()[2]);
  EXPECT_EQ(1, s.getSlice2()[4]);
  EXPECT_EQ(1, s.getSliceOut()[3]);
  EXPECT_EQ(1, s.getSlice2()[5]);
  EXPECT_EQ(1, s.getSliceOut()[4]);

  EXPECT_TRUE(s.nextNonSlicedFree());
  EXPECT_EQ(1, s.getSlice1()[2]);
  EXPECT_EQ(1, s.getSliceOut()[2]);
  EXPECT_EQ(1, s.getSlice2()[4]);
  EXPECT_EQ(1, s.getSliceOut()[3]);
  EXPECT_EQ(1, s.getSlice2()[5]);
  EXPECT_EQ(1, s.getSliceOut()[4]);

  EXPECT_TRUE(s.nextNonSlicedFree());
  EXPECT_EQ(0, s.getSlice1()[2]);
  EXPECT_EQ(0, s.getSliceOut()[2]);
  EXPECT_EQ(0, s.getSlice2()[4]);
  EXPECT_EQ(0, s.getSliceOut()[3]);
  EXPECT_EQ(0, s.getSlice2()[5]);
  EXPECT_EQ(0, s.getSliceOut()[4]);
  EXPECT_EQ(1, s.getSlice2()[6]);
  EXPECT_EQ(1, s.getSliceOut()[5]);

  EXPECT_TRUE(s.nextNonSlicedFree());
  EXPECT_TRUE(s.nextNonSlicedFree());
  EXPECT_TRUE(s.nextNonSlicedFree());
  EXPECT_TRUE(s.nextNonSlicedFree());
  EXPECT_TRUE(s.nextNonSlicedFree());
  EXPECT_TRUE(s.nextNonSlicedFree());
  EXPECT_TRUE(s.nextNonSlicedFree());

  EXPECT_FALSE(s.nextNonSlicedFree());
  EXPECT_EQ(0, s.getSlice1()[2]);
  EXPECT_EQ(0, s.getSliceOut()[2]);
  EXPECT_EQ(0, s.getSlice2()[4]);
  EXPECT_EQ(0, s.getSliceOut()[3]);
  EXPECT_EQ(0, s.getSlice2()[5]);
  EXPECT_EQ(0, s.getSliceOut()[4]);
  EXPECT_EQ(0, s.getSlice2()[6]);
  EXPECT_EQ(0, s.getSliceOut()[5]);

}

TEST(DoubleIterator, SingleContractedIndex) {
  DoubleSliceIterator s(Tensor(3,2), Tensor(2,2), {{2,0}});
  vector<int> s1 = s.getSlice1();
  vector<int> s2 = s.getSlice2();
  vector<int> so = s.getSliceOut();

  ASSERT_EQ(3, s1.size());
  ASSERT_EQ(2, s2.size());
  ASSERT_EQ(3, so.size());

  EXPECT_EQ(-2, s1[0]); EXPECT_EQ(0, s1[1]); EXPECT_EQ(-1, s1[2]);
  EXPECT_EQ(-1, s2[0]); EXPECT_EQ(-2, s2[1]);
  EXPECT_EQ(-1, so[0]); EXPECT_EQ(0, so[1]); EXPECT_EQ(-1, so[2]);

  EXPECT_FALSE(s.nextContracted());
  EXPECT_FALSE(s.nextSlicedFree());

}

TEST(DoubleIterator, OnlyContractedIndices) {
  DoubleSliceIterator s(Tensor(3,2), Tensor(3,2), {{0,2},{1,1},{2,0}});
  vector<int> s1 = s.getSlice1();
  vector<int> s2 = s.getSlice2();
  vector<int> so = s.getSliceOut();

  ASSERT_EQ(3, s1.size());
  ASSERT_EQ(3, s2.size());
  ASSERT_EQ(0, so.size());

  EXPECT_EQ(-2, s1[0]); EXPECT_EQ(-1, s1[1]); EXPECT_EQ(0, s1[2]);
  EXPECT_EQ(0, s2[0]); EXPECT_EQ(-1, s2[1]); EXPECT_EQ(-2, s2[2]);

  EXPECT_FALSE(s.nextNonSlicedFree());
  EXPECT_FALSE(s.nextSlicedFree());

  EXPECT_TRUE(s.nextContracted());
  EXPECT_EQ(1, s.getSlice1()[2]);
  EXPECT_EQ(1, s.getSlice2()[0]);

  EXPECT_FALSE(s.nextContracted());
  EXPECT_EQ(0, s.getSlice1()[2]);
  EXPECT_EQ(0, s.getSlice2()[0]);

}

TEST(DoubleIterator, OneFreeOneFullyContracted) {
  DoubleSliceIterator s(Tensor(4,2), Tensor(2,2), {{0,0},{1,1}});
  vector<int> s1 = s.getSlice1();
  vector<int> s2 = s.getSlice2();
  vector<int> so = s.getSliceOut();

  ASSERT_EQ(4, s1.size());
  ASSERT_EQ(2, s2.size());
  ASSERT_EQ(2, so.size());

  EXPECT_EQ(-1, s1[0]); EXPECT_EQ(-2, s1[1]);
  EXPECT_EQ(0, s1[2]); EXPECT_EQ(0, s1[3]);

  EXPECT_EQ(-1, s2[0]); EXPECT_EQ(-2, s2[1]);

  EXPECT_EQ(-1, so[0]); EXPECT_EQ(-1, so[1]);

  EXPECT_FALSE(s.nextContracted());

  EXPECT_FALSE(s.nextNonSlicedFree());

  EXPECT_TRUE(s.nextSlicedFree());
  EXPECT_EQ(1, s.getSlice1()[2]);

  EXPECT_TRUE(s.nextSlicedFree());
  EXPECT_EQ(0, s.getSlice1()[2]);
  EXPECT_EQ(1, s.getSlice1()[3]);

  EXPECT_TRUE(s.nextSlicedFree());
  EXPECT_EQ(1, s.getSlice1()[2]);
  EXPECT_EQ(1, s.getSlice1()[3]);

  EXPECT_FALSE(s.nextSlicedFree());
  EXPECT_EQ(0, s.getSlice1()[2]);
  EXPECT_EQ(0, s.getSlice1()[3]);

}



TEST(DoubleIterator, MinimumStorageSlicingOneContractedIndexLeading) {

  Tensor t1(3,64,{1,2,0});
  Tensor t2(3,64,{0,1,2});

  DoubleSliceIterator s(t1,t2, {{1,1}});

  EXPECT_EQ(0, s.getSlice1()[0]);
  EXPECT_EQ(-1, s.getSlice1()[1]);
  EXPECT_EQ(-2, s.getSlice1()[2]);

  EXPECT_EQ(-2, s.getSlice2()[0]);
  EXPECT_EQ(-1, s.getSlice2()[1]);
  EXPECT_EQ(0, s.getSlice2()[2]);

  EXPECT_EQ(0, s.getSliceOut()[0]);
  EXPECT_EQ(-1, s.getSliceOut()[1]);
  EXPECT_EQ(-1, s.getSliceOut()[2]);
  EXPECT_EQ(0, s.getSliceOut()[3]);

}

TEST(DoubleIterator, MinimumStorageSlicingOneContractedIndexLeading2) {

  Tensor t1(3,64,{0,1,2});
  Tensor t2(3,64,{1,2,0});

  DoubleSliceIterator s(t1,t2, {{1,1}});

  EXPECT_EQ(-2, s.getSlice1()[0]);
  EXPECT_EQ(-1, s.getSlice1()[1]);
  EXPECT_EQ(0, s.getSlice1()[2]);

  EXPECT_EQ(0, s.getSlice2()[0]);
  EXPECT_EQ(-1, s.getSlice2()[1]);
  EXPECT_EQ(-2, s.getSlice2()[2]);

  EXPECT_EQ(-1, s.getSliceOut()[0]);
  EXPECT_EQ(0, s.getSliceOut()[1]);
  EXPECT_EQ(0, s.getSliceOut()[2]);
  EXPECT_EQ(-1, s.getSliceOut()[3]);

}

TEST(DoubleIterator, MinimumStorageSlicingOneContractedIndexNonLeading) {

  Tensor t1(3,64,{1,2,0});
  Tensor t2(3,64,{0,1,2});

  DoubleSliceIterator s(t1,t2, {{0,2}});

  EXPECT_EQ(-1, s.getSlice1()[0]);
  EXPECT_EQ(-2, s.getSlice1()[1]);
  EXPECT_EQ(0, s.getSlice1()[2]);

  EXPECT_EQ(-2, s.getSlice2()[0]);
  EXPECT_EQ(0, s.getSlice2()[1]);
  EXPECT_EQ(-1, s.getSlice2()[2]);

  EXPECT_EQ(-1, s.getSliceOut()[0]);
  EXPECT_EQ(0, s.getSliceOut()[1]);
  EXPECT_EQ(-1, s.getSliceOut()[2]);
  EXPECT_EQ(0, s.getSliceOut()[3]);

}

TEST(DoubleIterator, MinimumStorageSlicingTwoContractedIndices1) {

  Tensor t1(4,64,{0,1,2,3});
  Tensor t2(4,64,{0,1,2,3});

  DoubleSliceIterator s(t1,t2, {{1,1},{0,0}});

  EXPECT_EQ(-2, s.getSlice1()[0]);
  EXPECT_EQ(-1, s.getSlice1()[1]);
  EXPECT_EQ(0, s.getSlice1()[2]);
  EXPECT_EQ(0, s.getSlice1()[3]);

  EXPECT_EQ(-2, s.getSlice2()[0]);
  EXPECT_EQ(-1, s.getSlice2()[1]);
  EXPECT_EQ(0, s.getSlice2()[2]);
  EXPECT_EQ(0, s.getSlice2()[3]);

  EXPECT_EQ(-1, s.getSliceOut()[0]);
  EXPECT_EQ(-1, s.getSliceOut()[1]);
  EXPECT_EQ(0, s.getSliceOut()[2]);
  EXPECT_EQ(0, s.getSliceOut()[3]);



}



TEST(DoubleIteratorErrorHandling, ValidRankAndSize) {
  EXPECT_THROW(DoubleSliceIterator(Tensor(1,2), Tensor(2,2),{{0,1}}),
               invalid_argument);
  EXPECT_THROW(DoubleSliceIterator(Tensor(2,1), Tensor(3,1),{{0,1}}),
               invalid_argument);
}

TEST(DoubleIteratorErrorHandling, ErrorIfContractionsIsEmpty) {
  EXPECT_THROW(DoubleSliceIterator(Tensor(4,64), Tensor(2,64),{}),
               invalid_argument);
}

TEST(DoubleIteratorErrorHandling, ErrorIfContractionIndexIsInvalid) {
  EXPECT_THROW(DoubleSliceIterator(Tensor(4,64), Tensor(2,64),{{4,1}}),
               invalid_argument);
  EXPECT_THROW(DoubleSliceIterator(Tensor(4,64), Tensor(2,64),{{3,-1}}),
               invalid_argument);
}

TEST(DoubleIteratorErrorHandling, ErrorIfContractionIndexIsRepeated) {
  EXPECT_THROW(DoubleSliceIterator(Tensor(2,64), Tensor(6,64),{{0,4},{0,1}})
  , invalid_argument);
}

}