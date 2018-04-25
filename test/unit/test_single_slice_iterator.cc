#include "slice_iterator.h"
#include "gtest/gtest.h"

/*
 * Unit tests of the iteration logic defined in SINGLE_SLICE_ITERATOR.CC
 */

using namespace pichi;
using namespace std;

namespace {

TEST(SingleIterator, Trace) {
  SingleSliceIterator s(2,64,{{0,1}});
  ASSERT_EQ(2,s.getSlice1().size());
  EXPECT_EQ(-1, s.getSlice1()[0]);
  EXPECT_EQ(-1, s.getSlice1()[1]);
  ASSERT_EQ(0, s.getSliceOut().size());
}

TEST(SingleIterator, AllContracted) {
  SingleSliceIterator s(4,64,{{0,1},{2,3}});
  ASSERT_EQ(4,s.getSlice1().size());
  EXPECT_EQ(-1, s.getSlice1()[0]);
  EXPECT_EQ(-1, s.getSlice1()[1]);
  EXPECT_EQ(0, s.getSlice1()[2]);
  EXPECT_EQ(0, s.getSlice1()[3]);
  ASSERT_EQ(0, s.getSliceOut().size());
}

TEST(SingleIterator, NextContracted) {
  SingleSliceIterator s(6,2,{{0,1},{2,3},{4,5}});
  ASSERT_EQ(6,s.getSlice1().size());
  EXPECT_EQ(-1, s.getSlice1()[0]);
  EXPECT_EQ(-1, s.getSlice1()[1]);
  EXPECT_EQ(0, s.getSlice1()[2]);
  EXPECT_EQ(0, s.getSlice1()[3]);
  EXPECT_EQ(0, s.getSlice1()[4]);
  EXPECT_EQ(0, s.getSlice1()[5]);
  ASSERT_EQ(0, s.getSliceOut().size());

  EXPECT_TRUE(s.nextContracted());

  EXPECT_EQ(1, s.getSlice1()[2]);
  EXPECT_EQ(1, s.getSlice1()[3]);

  EXPECT_TRUE(s.nextContracted());

  EXPECT_EQ(0, s.getSlice1()[2]);
  EXPECT_EQ(0, s.getSlice1()[3]);
  EXPECT_EQ(1, s.getSlice1()[4]);
  EXPECT_EQ(1, s.getSlice1()[5]);

  EXPECT_TRUE(s.nextContracted());

  EXPECT_EQ(1, s.getSlice1()[2]);
  EXPECT_EQ(1, s.getSlice1()[3]);
  EXPECT_EQ(1, s.getSlice1()[4]);
  EXPECT_EQ(1, s.getSlice1()[5]);

  EXPECT_FALSE(s.nextContracted());

  EXPECT_EQ(0, s.getSlice1()[2]);
  EXPECT_EQ(0, s.getSlice1()[3]);
  EXPECT_EQ(0, s.getSlice1()[4]);
  EXPECT_EQ(0, s.getSlice1()[5]);

}

TEST(SingleIterator, NextSlicedFree) {
  SingleSliceIterator s(4,2,{{0,1}});
  ASSERT_EQ(4,s.getSlice1().size());
  EXPECT_EQ(-1, s.getSlice1()[0]);
  EXPECT_EQ(-1, s.getSlice1()[1]);
  EXPECT_EQ(0, s.getSlice1()[2]);
  EXPECT_EQ(0, s.getSlice1()[3]);
  ASSERT_EQ(2, s.getSliceOut().size());
  EXPECT_EQ(-1, s.getSliceOut()[0]);
  EXPECT_EQ(-1, s.getSliceOut()[1]);

  EXPECT_TRUE(s.nextSlicedFree());
  EXPECT_EQ(1, s.getSlice1()[2]);
  EXPECT_EQ(0, s.getSlice1()[3]);

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

TEST(SingleIterator, NextNonSlicedFree) {
  SingleSliceIterator s(6,2,{{0,1}});
  ASSERT_EQ(6,s.getSlice1().size());
  EXPECT_EQ(-1, s.getSlice1()[0]);
  EXPECT_EQ(-1, s.getSlice1()[1]);
  EXPECT_EQ(0, s.getSlice1()[2]);
  EXPECT_EQ(0, s.getSlice1()[3]);
  EXPECT_EQ(0, s.getSlice1()[4]);
  EXPECT_EQ(0, s.getSlice1()[5]);
  ASSERT_EQ(4, s.getSliceOut().size());
  EXPECT_EQ(-1, s.getSliceOut()[0]);
  EXPECT_EQ(-1, s.getSliceOut()[1]);
  EXPECT_EQ(0, s.getSliceOut()[2]);
  EXPECT_EQ(0, s.getSliceOut()[3]);

  EXPECT_TRUE(s.nextNonSlicedFree());
  EXPECT_EQ(1, s.getSlice1()[4]);
  EXPECT_EQ(1, s.getSliceOut()[2]);

  EXPECT_TRUE(s.nextNonSlicedFree());
  EXPECT_EQ(0, s.getSlice1()[4]);
  EXPECT_EQ(0, s.getSliceOut()[2]);
  EXPECT_EQ(1, s.getSlice1()[5]);
  EXPECT_EQ(1, s.getSliceOut()[3]);

  EXPECT_TRUE(s.nextNonSlicedFree());
  EXPECT_EQ(1, s.getSlice1()[4]);
  EXPECT_EQ(1, s.getSliceOut()[2]);
  EXPECT_EQ(1, s.getSlice1()[5]);
  EXPECT_EQ(1, s.getSliceOut()[3]);

  EXPECT_FALSE(s.nextNonSlicedFree());
  EXPECT_EQ(0, s.getSlice1()[4]);
  EXPECT_EQ(0, s.getSliceOut()[2]);
  EXPECT_EQ(0, s.getSlice1()[5]);
  EXPECT_EQ(0, s.getSliceOut()[3]);

}

}


