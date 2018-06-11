#include "string_utils.h"
#include "gtest/gtest.h"


using namespace pichi;
using namespace std;

namespace {

TEST(ResidualString, ResidualOfABCD_BDE) {
  string s = residualString("abcd", "bde");
  EXPECT_EQ("ace", s);
}

TEST(MatchChars, MatchABCD_BDE) {
  vector<pair<int,int>> m = matchChars("abcd","bde");
  ASSERT_EQ(2, m.size());

  EXPECT_EQ(1, m[0].first);
  EXPECT_EQ(0, m[0].second);

  EXPECT_EQ(3, m[1].first);
  EXPECT_EQ(1, m[1].second);
}

TEST(SplitToNodes, SingleNode) {
  vector<string> r = splitToNodes("1aa");
  ASSERT_EQ(1, r.size());
  EXPECT_EQ("1aa", r[0]);
}

TEST(SplitToNodes, FourNodesOfRank2Or3) {
  vector<string> r = splitToNodes("1abc2dd3abc4ee");
  ASSERT_EQ(4, r.size());
  EXPECT_EQ("1abc", r[0]);
  EXPECT_EQ("2dd", r[1]);
  EXPECT_EQ("3abc", r[2]);
  EXPECT_EQ("4ee", r[3]);
}



TEST(SplitToConnected, DontSplitSingle) {
  vector<string> r = splitToConnected("1aa");
  ASSERT_EQ(1, r.size());
  EXPECT_EQ("1aa", r[0]);
}

TEST(SplitToConnected, SplitTwoSingles) {
  vector<string> r = splitToConnected("1aa2bb");
  ASSERT_EQ(2, r.size());
  EXPECT_EQ("1aa", r[0]);
  EXPECT_EQ("2bb", r[1]);
}

TEST(SplitToConnected, DontSplitTwoConnected) {
  vector<string> r = splitToConnected("1ab2ab");
  ASSERT_EQ(1, r.size());
  EXPECT_EQ("1ab2ab", r[0]);
}

TEST(SplitToConnected, DontSplitFourConnectedWithBreaks) {
  vector<string> r = splitToConnected("1ab2cd3ac4bd");
  ASSERT_EQ(1, r.size());
  EXPECT_EQ("1ab3ac4bd2cd", r[0]);
}

TEST(SplitToConnected, SplitTwoConnectedAndTwoSingles) {
  vector<string> r = splitToConnected("1abc2dd3abc4ee");
  ASSERT_EQ(3, r.size());
  EXPECT_EQ("1abc3abc", r[0]);
  EXPECT_EQ("4ee", r[1]);
  EXPECT_EQ("2dd", r[2]);
}

TEST(SplitToConnected, TwoDigitTensor) {
  vector<string> r = splitToConnected("2ab0ab10cd1cd");
  ASSERT_EQ(2, r.size());
  EXPECT_EQ("2ab0ab", r[0]);
  EXPECT_EQ("10cd1cd", r[1]);
}

}