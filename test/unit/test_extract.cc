#include "diagrams.h"
#include "gtest/gtest.h"

/*
 * Unit tests of the extract function defined in DIAGRAMS.CC
 */

using namespace pichi;
using namespace std;

namespace {

TEST(Extract, Diagram0) {
  Graph g("0aa");
  Graph h = extract(g, 0);

  auto s = g.getNodes();
  EXPECT_EQ(1, s.size());
  EXPECT_NE(s.find(0), s.end());

  vector<pair<int,int>> c = h.connections(0);
  ASSERT_EQ(2, c.size());
  EXPECT_EQ(make_pair(0,1), c[0]);
  EXPECT_EQ(make_pair(0,0), c[1]);
}

TEST(Extract, Diagram1) {
  Graph g("1ab2ba");
  Graph h = extract(g, 1);

  auto s = g.getNodes();
  EXPECT_EQ(2, h.getNodes().size());
  EXPECT_NE(s.find(1), s.end());
  EXPECT_NE(s.find(2), s.end());

  vector<pair<int,int>> c = h.connections(1);
  ASSERT_EQ(2, c.size());
  EXPECT_EQ(make_pair(2,1), c[0]);
  EXPECT_EQ(make_pair(2,0), c[1]);

  c = h.connections(2);
  ASSERT_EQ(2, c.size());
  EXPECT_EQ(make_pair(1,1), c[0]);
  EXPECT_EQ(make_pair(1,0), c[1]);
}

TEST(Extract, Diagram2) {
  Graph g("1ab2bc3ac");
  Graph h = extract(g, 2);

  auto s = h.getNodes();
  EXPECT_EQ(2, s.size());
  EXPECT_NE(s.find(2), s.end());
  EXPECT_NE(s.find(3), s.end());

  vector<pair<int,int>> c = h.connections(2);
  ASSERT_EQ(2, c.size());
  EXPECT_EQ(make_pair(-1,-1), c[0]);
  EXPECT_EQ(make_pair(3,1), c[1]);

  c = h.connections(3);
  ASSERT_EQ(2, c.size());
  EXPECT_EQ(make_pair(-1,-1), c[0]);
  EXPECT_EQ(make_pair(2,1), c[1]);
}

TEST(Extract, Diagram3) {
  Graph g("1ab2cd3ac4bd");
  Graph h = extract(g, 3);

  auto s = h.getNodes();
  EXPECT_EQ(2, s.size());
  EXPECT_NE(s.find(2), s.end());
  EXPECT_NE(s.find(4), s.end());

  vector<pair<int,int>> c = h.connections(2);
  ASSERT_EQ(2, c.size());
  EXPECT_EQ(make_pair(-1,-1), c[0]);
  EXPECT_EQ(make_pair(4,1), c[1]);

  c = h.connections(4);
  ASSERT_EQ(2, c.size());
  EXPECT_EQ(make_pair(-1,-1), c[0]);
  EXPECT_EQ(make_pair(2,1), c[1]);
}

TEST(Extract, Diagram4) {
  Graph g("1abc2acb");
  Graph h = extract(g, 4);

  auto s = h.getNodes();
  EXPECT_EQ(2, h.getNodes().size());
  EXPECT_NE(s.find(1), s.end());
  EXPECT_NE(s.find(2), s.end());

  vector<pair<int,int>> c = h.connections(1);
  ASSERT_EQ(3, c.size());
  EXPECT_EQ(make_pair(2,0), c[0]);
  EXPECT_EQ(make_pair(2,2), c[1]);
  EXPECT_EQ(make_pair(2,1), c[2]);

  c = h.connections(2);
  ASSERT_EQ(3, c.size());
  EXPECT_EQ(make_pair(1,0), c[0]);
  EXPECT_EQ(make_pair(1,2), c[1]);
  EXPECT_EQ(make_pair(1,1), c[2]);
}

TEST(Extract, Diagram5) {
  Graph g("1abc2adb4dc");
  Graph h = extract(g, 5);

  auto s = h.getNodes();
  EXPECT_EQ(2, h.getNodes().size());
  EXPECT_NE(s.find(2), s.end());
  EXPECT_NE(s.find(4), s.end());

  vector<pair<int,int>> c = h.connections(4);
  ASSERT_EQ(2, c.size());
  EXPECT_EQ(make_pair(2,1), c[0]);
  EXPECT_EQ(make_pair(-1,-1), c[1]);

  c = h.connections(2);
  ASSERT_EQ(3, c.size());
  EXPECT_EQ(make_pair(-1,-1), c[0]);
  EXPECT_EQ(make_pair(4,0), c[1]);
  EXPECT_EQ(make_pair(-1,-1), c[2]);
}

TEST(Extract, Diagram6) {
  Graph g("1abc2adb4de8ce");
  Graph h = extract(g, 6);

  auto s = h.getNodes();
  EXPECT_EQ(2, h.getNodes().size());
  EXPECT_NE(s.find(4), s.end());
  EXPECT_NE(s.find(8), s.end());

  vector<pair<int,int>> c = h.connections(4);
  ASSERT_EQ(2, c.size());
  EXPECT_EQ(make_pair(-1,-1), c[0]);
  EXPECT_EQ(make_pair(8,1), c[1]);

  c = h.connections(8);
  ASSERT_EQ(2, c.size());
  EXPECT_EQ(make_pair(-1,-1), c[0]);
  EXPECT_EQ(make_pair(4,1), c[1]);
}

TEST(Extract, Diagram7FirstNode3) {
  Graph g("1abc2ade4db8ce");
  Graph h = extract(g, 7);

  auto s = h.getNodes();
  EXPECT_EQ(2, h.getNodes().size());
  EXPECT_NE(s.find(2), s.end());
  EXPECT_NE(s.find(8), s.end());

  vector<pair<int,int>> c = h.connections(2);
  ASSERT_EQ(3, c.size());
  EXPECT_EQ(make_pair(-1,-1), c[0]);
  EXPECT_EQ(make_pair(-1,-1), c[1]);
  EXPECT_EQ(make_pair(8,1), c[2]);

  c = h.connections(8);
  ASSERT_EQ(2, c.size());
  EXPECT_EQ(make_pair(-1,-1), c[0]);
  EXPECT_EQ(make_pair(2,2), c[1]);
}

TEST(Extract, Diagram7FirstNode2) {
  Graph g("1db4abc2ade8ce");
  Graph h = extract(g, 7);

  auto s = h.getNodes();
  EXPECT_EQ(2, h.getNodes().size());
  EXPECT_NE(s.find(4), s.end());
  EXPECT_NE(s.find(8), s.end());

  vector<pair<int,int>> c = h.connections(4);
  ASSERT_EQ(3, c.size());
  EXPECT_EQ(make_pair(-1,-1), c[0]);
  EXPECT_EQ(make_pair(-1,-1), c[1]);
  EXPECT_EQ(make_pair(8,0), c[2]);

  c = h.connections(8);
  ASSERT_EQ(2, c.size());
  EXPECT_EQ(make_pair(4,2), c[0]);
  EXPECT_EQ(make_pair(-1,-1), c[1]);
}

TEST(Extract, Diagram8) {
  Graph g("4abc5ecf9abd15def");
  Graph h = extract(g, 8);

  auto s = h.getNodes();
  EXPECT_EQ(2, h.getNodes().size());
  EXPECT_NE(s.find(5), s.end());
  EXPECT_NE(s.find(15), s.end());

  vector<pair<int,int>> c = h.connections(5);
  ASSERT_EQ(3, c.size());
  EXPECT_EQ(make_pair(15,1), c[0]);
  EXPECT_EQ(make_pair(-1,-1), c[1]);
  EXPECT_EQ(make_pair(15,2), c[2]);

  c = h.connections(15);
  ASSERT_EQ(3, c.size());
  EXPECT_EQ(make_pair(-1,-1), c[0]);
  EXPECT_EQ(make_pair(5,0), c[1]);
  EXPECT_EQ(make_pair(5,2), c[2]);
}

TEST(Extract, Diagram9) {
  Graph g("1abcd2bc3ad");
  Graph h = extract(g, 9);

  Graph exp("1abcd3ad");
  EXPECT_EQ(h,exp);

}

TEST(Extract, UnknownSelfConnected) {
  Graph g("1aabc");
  EXPECT_THROW(extract(g,-1), invalid_argument);
}


}