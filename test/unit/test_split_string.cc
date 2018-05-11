#include "util.h"
#include "gtest/gtest.h"


using namespace pichi;
using namespace std;

namespace {

TEST(SplitString, DontSplitSingle) {
  vector<string> r = split("1aa");
  ASSERT_EQ(1, r.size());
  EXPECT_EQ("1aa", r[0]);
}

TEST(SplitString, SplitTwoSingles) {
  vector<string> r = split("1aa2bb");
  ASSERT_EQ(2, r.size());
  EXPECT_EQ("1aa", r[0]);
  EXPECT_EQ("2bb", r[1]);
}

TEST(SplitString, DontSplitTwoConnected) {
  vector<string> r = split("1ab2ab");
  ASSERT_EQ(1, r.size());
  EXPECT_EQ("1ab2ab", r[0]);
}

TEST(SplitString, DontSplitFourConnectedWithBreaks) {
  vector<string> r = split("1ab2cd3ac4bd");
  ASSERT_EQ(1, r.size());
  EXPECT_EQ("1ab3ac4bd2cd", r[0]);
}

TEST(SplitString, SplitTwoConnectedAndTwoSingles) {
  vector<string> r = split("1abc2dd3abc4ee");
  ASSERT_EQ(3, r.size());
  EXPECT_EQ("1abc3abc", r[0]);
  EXPECT_EQ("4ee", r[1]);
  EXPECT_EQ("2dd", r[2]);
}

}