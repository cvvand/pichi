#include "slice_iterator.h"
#include "gtest/gtest.h"

/*
 * Unit tests of the iteration logic defined in DOUBLE_SLICE_ITERATOR.CC
 */

using namespace pichi;
using namespace std;

namespace {

TEST(DoubleIterator2, Simple) {
  DoubleSliceIterator s(3, 3, 2, {{1,1},{2,0}});
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

TEST(DoubleIterator2, NextContracted) {
  DoubleSliceIterator s(7, 7, 2, {{3,3},{4,2},{5,1},{6,0}});
  vector<int> s1 = s.getSlice1();
  vector<int> s2 = s.getSlice2();
  vector<int> so = s.getSliceOut();

  ASSERT_EQ(7, s1.size());
  ASSERT_EQ(7, s2.size());
  ASSERT_EQ(6, so.size());


  EXPECT_EQ(0, s1[0]); EXPECT_EQ(0, s1[1]); EXPECT_EQ(0, s1[2]);
  EXPECT_EQ(-1, s1[3]); EXPECT_EQ(-2, s1[4]); EXPECT_EQ(0, s1[5]);
  EXPECT_EQ(0, s1[6]);

  EXPECT_EQ(0, s2[0]); EXPECT_EQ(0, s2[1]); EXPECT_EQ(-2, s2[2]);
  EXPECT_EQ(-1, s2[3]); EXPECT_EQ(0, s2[4]); EXPECT_EQ(0, s2[5]);
  EXPECT_EQ(0, s2[6]);

  EXPECT_EQ(-1, so[0]); EXPECT_EQ(0, so[1]); EXPECT_EQ(0, so[2]);
  EXPECT_EQ(-1, so[3]); EXPECT_EQ(0, so[4]); EXPECT_EQ(0, so[5]);

  EXPECT_TRUE(s.nextContracted());
  EXPECT_EQ(1, s.getSlice1()[5]);
  EXPECT_EQ(1, s.getSlice2()[1]);

  EXPECT_TRUE(s.nextContracted());
  EXPECT_EQ(0, s.getSlice1()[5]);
  EXPECT_EQ(0, s.getSlice2()[1]);
  EXPECT_EQ(1, s.getSlice1()[6]);
  EXPECT_EQ(1, s.getSlice2()[0]);

  EXPECT_TRUE(s.nextContracted());
  EXPECT_EQ(1, s.getSlice1()[5]);
  EXPECT_EQ(1, s.getSlice2()[1]);
  EXPECT_EQ(1, s.getSlice1()[6]);
  EXPECT_EQ(1, s.getSlice2()[0]);

  EXPECT_FALSE(s.nextContracted());
  EXPECT_EQ(0, s.getSlice1()[5]);
  EXPECT_EQ(0, s.getSlice2()[1]);
  EXPECT_EQ(0, s.getSlice1()[6]);
  EXPECT_EQ(0, s.getSlice2()[0]);

}

TEST(DoubleIterator2, NextSlicedFree) {
  DoubleSliceIterator s(7, 7, 2, {{3,3},{4,2},{5,1},{6,0}});
  vector<int> s1 = s.getSlice1();
  vector<int> s2 = s.getSlice2();
  vector<int> so = s.getSliceOut();

  ASSERT_EQ(7, s1.size());
  ASSERT_EQ(7, s2.size());
  ASSERT_EQ(6, so.size());


  EXPECT_EQ(0, s1[0]); EXPECT_EQ(0, s1[1]); EXPECT_EQ(0, s1[2]);
  EXPECT_EQ(-1, s1[3]); EXPECT_EQ(-2, s1[4]); EXPECT_EQ(0, s1[5]);
  EXPECT_EQ(0, s1[6]);

  EXPECT_EQ(0, s2[0]); EXPECT_EQ(0, s2[1]); EXPECT_EQ(-2, s2[2]);
  EXPECT_EQ(-1, s2[3]); EXPECT_EQ(0, s2[4]); EXPECT_EQ(0, s2[5]);
  EXPECT_EQ(0, s2[6]);

  EXPECT_EQ(-1, so[0]); EXPECT_EQ(0, so[1]); EXPECT_EQ(0, so[2]);
  EXPECT_EQ(-1, so[3]); EXPECT_EQ(0, so[4]); EXPECT_EQ(0, so[5]);

  EXPECT_TRUE(s.nextSlicedFree());
  EXPECT_EQ(1, s.getSlice1()[0]);

  EXPECT_TRUE(s.nextSlicedFree());
  EXPECT_EQ(0, s.getSlice1()[0]);
  EXPECT_EQ(1, s.getSlice2()[4]);

  EXPECT_TRUE(s.nextSlicedFree());
  EXPECT_EQ(1, s.getSlice1()[0]);
  EXPECT_EQ(1, s.getSlice2()[4]);

  EXPECT_FALSE(s.nextSlicedFree());
  EXPECT_EQ(0, s.getSlice1()[0]);
  EXPECT_EQ(0, s.getSlice2()[4]);

}

TEST(DoubleIterator2, NextNonSlicedFree) {
  DoubleSliceIterator s(7, 7, 2, {{3,3},{4,2},{5,1},{6,0}});
  vector<int> s1 = s.getSlice1();
  vector<int> s2 = s.getSlice2();
  vector<int> so = s.getSliceOut();

  ASSERT_EQ(7, s1.size());
  ASSERT_EQ(7, s2.size());
  ASSERT_EQ(6, so.size());


  EXPECT_EQ(0, s1[0]); EXPECT_EQ(0, s1[1]); EXPECT_EQ(0, s1[2]);
  EXPECT_EQ(-1, s1[3]); EXPECT_EQ(-2, s1[4]); EXPECT_EQ(0, s1[5]);
  EXPECT_EQ(0, s1[6]);

  EXPECT_EQ(0, s2[0]); EXPECT_EQ(0, s2[1]); EXPECT_EQ(-2, s2[2]);
  EXPECT_EQ(-1, s2[3]); EXPECT_EQ(0, s2[4]); EXPECT_EQ(0, s2[5]);
  EXPECT_EQ(0, s2[6]);

  EXPECT_EQ(-1, so[0]); EXPECT_EQ(0, so[1]); EXPECT_EQ(0, so[2]);
  EXPECT_EQ(-1, so[3]); EXPECT_EQ(0, so[4]); EXPECT_EQ(0, so[5]);

  EXPECT_TRUE(s.nextNonSlicedFree());
  EXPECT_EQ(1, s.getSlice1()[1]);
  EXPECT_EQ(1, s.getSliceOut()[1]);

  EXPECT_TRUE(s.nextNonSlicedFree());
  EXPECT_EQ(0, s.getSlice1()[1]);
  EXPECT_EQ(0, s.getSliceOut()[1]);
  EXPECT_EQ(1, s.getSlice1()[2]);
  EXPECT_EQ(1, s.getSliceOut()[2]);

  EXPECT_TRUE(s.nextNonSlicedFree());
  EXPECT_EQ(1, s.getSlice1()[1]);
  EXPECT_EQ(1, s.getSliceOut()[1]);
  EXPECT_EQ(1, s.getSlice1()[2]);
  EXPECT_EQ(1, s.getSliceOut()[2]);

  EXPECT_TRUE(s.nextNonSlicedFree());
  EXPECT_EQ(0, s.getSlice1()[1]);
  EXPECT_EQ(0, s.getSliceOut()[1]);
  EXPECT_EQ(0, s.getSlice1()[2]);
  EXPECT_EQ(0, s.getSliceOut()[2]);
  EXPECT_EQ(1, s.getSlice2()[5]);
  EXPECT_EQ(1, s.getSliceOut()[4]);

  EXPECT_TRUE(s.nextNonSlicedFree());
  EXPECT_EQ(1, s.getSlice1()[1]);
  EXPECT_EQ(1, s.getSliceOut()[1]);
  EXPECT_EQ(0, s.getSlice1()[2]);
  EXPECT_EQ(0, s.getSliceOut()[2]);
  EXPECT_EQ(1, s.getSlice2()[5]);
  EXPECT_EQ(1, s.getSliceOut()[4]);

  EXPECT_TRUE(s.nextNonSlicedFree());
  EXPECT_EQ(0, s.getSlice1()[1]);
  EXPECT_EQ(0, s.getSliceOut()[1]);
  EXPECT_EQ(1, s.getSlice1()[2]);
  EXPECT_EQ(1, s.getSliceOut()[2]);
  EXPECT_EQ(1, s.getSlice2()[5]);
  EXPECT_EQ(1, s.getSliceOut()[4]);

  EXPECT_TRUE(s.nextNonSlicedFree());
  EXPECT_EQ(1, s.getSlice1()[1]);
  EXPECT_EQ(1, s.getSliceOut()[1]);
  EXPECT_EQ(1, s.getSlice1()[2]);
  EXPECT_EQ(1, s.getSliceOut()[2]);
  EXPECT_EQ(1, s.getSlice2()[5]);
  EXPECT_EQ(1, s.getSliceOut()[4]);

  EXPECT_TRUE(s.nextNonSlicedFree());
  EXPECT_EQ(0, s.getSlice1()[1]);
  EXPECT_EQ(0, s.getSliceOut()[1]);
  EXPECT_EQ(0, s.getSlice1()[2]);
  EXPECT_EQ(0, s.getSliceOut()[2]);
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
  EXPECT_EQ(0, s.getSlice1()[1]);
  EXPECT_EQ(0, s.getSliceOut()[1]);
  EXPECT_EQ(0, s.getSlice1()[2]);
  EXPECT_EQ(0, s.getSliceOut()[2]);
  EXPECT_EQ(0, s.getSlice2()[5]);
  EXPECT_EQ(0, s.getSliceOut()[4]);
  EXPECT_EQ(0, s.getSlice2()[6]);
  EXPECT_EQ(0, s.getSliceOut()[5]);

}

TEST(DoubleIterator1, Simple) {
  DoubleSliceIterator s(3, 2, 2, {{2,0}});
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

TEST(DoubleIteratorOnlyContracted, Simple) {
  DoubleSliceIterator s(3, 3, 2, {{0,2},{1,1},{2,0}});
  vector<int> s1 = s.getSlice1();
  vector<int> s2 = s.getSlice2();
  vector<int> so = s.getSliceOut();

  ASSERT_EQ(3, s1.size());
  ASSERT_EQ(3, s2.size());
  ASSERT_EQ(0, so.size());

  EXPECT_EQ(-1, s1[0]); EXPECT_EQ(-2, s1[1]); EXPECT_EQ(0, s1[2]);
  EXPECT_EQ(0, s2[0]); EXPECT_EQ(-2, s2[1]); EXPECT_EQ(-1, s2[2]);

  EXPECT_FALSE(s.nextNonSlicedFree());
  EXPECT_FALSE(s.nextSlicedFree());

  EXPECT_TRUE(s.nextContracted());
  EXPECT_EQ(1, s.getSlice1()[2]);
  EXPECT_EQ(1, s.getSlice2()[0]);

  EXPECT_FALSE(s.nextContracted());
  EXPECT_EQ(0, s.getSlice1()[2]);
  EXPECT_EQ(0, s.getSlice2()[0]);

}

TEST(DoubleIteratorErrorHandling, ValidRankAndSize) {
  EXPECT_THROW(DoubleSliceIterator(1,2,64,{{0,1}}), invalid_argument);
  EXPECT_THROW(DoubleSliceIterator(2,3,1,{{0,1}}), invalid_argument);
}

TEST(DoubleIteratorErrorHandling, ErrorIfContractionsIsEmpty) {
  EXPECT_THROW(DoubleSliceIterator(4,2,64,{}), invalid_argument);
}

TEST(DoubleIteratorErrorHandling, ErrorIfContractionIndexIsInvalid) {
  EXPECT_THROW(DoubleSliceIterator(4,2,64,{{4,1}}), invalid_argument);
  EXPECT_THROW(DoubleSliceIterator(4,2,64,{{3,-1}}), invalid_argument);
}

TEST(DoubleIteratorErrorHandling, ErrorIfContractionIndexIsRepeated) {
  EXPECT_THROW(DoubleSliceIterator(2,6,64,{{0,4},{0,1}}), invalid_argument);
}

}