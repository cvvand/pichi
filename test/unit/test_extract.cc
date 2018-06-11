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

  EXPECT_EQ(1, h.getNodes().size());
  EXPECT_EQ(0, h.getNodes()[0]);

  vector<pair<int,int>> c = h.connections(0);
  ASSERT_EQ(2, c.size());
  EXPECT_EQ(make_pair(0,1), c[0]);
  EXPECT_EQ(make_pair(0,0), c[1]);
}

TEST(Extract, Diagram1) {
  Graph g("1ab2ba");
  Graph h = extract(g, 1);

  EXPECT_EQ(2, h.getNodes().size());
  EXPECT_EQ(1, h.getNodes()[0]);
  EXPECT_EQ(2, h.getNodes()[1]);

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

  EXPECT_EQ(2, h.getNodes().size());
  EXPECT_EQ(2, h.getNodes()[0]);
  EXPECT_EQ(3, h.getNodes()[1]);

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

  EXPECT_EQ(2, h.getNodes().size());
  EXPECT_EQ(2, h.getNodes()[0]);
  EXPECT_EQ(4, h.getNodes()[1]);

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

  EXPECT_EQ(2, h.getNodes().size());
  EXPECT_EQ(1, h.getNodes()[0]);
  EXPECT_EQ(2, h.getNodes()[1]);

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

  EXPECT_EQ(2, h.getNodes().size());
  EXPECT_EQ(2, h.getNodes()[0]);
  EXPECT_EQ(4, h.getNodes()[1]);

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

  EXPECT_EQ(2, h.getNodes().size());
  EXPECT_EQ(4, h.getNodes()[0]);
  EXPECT_EQ(8, h.getNodes()[1]);

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

  EXPECT_EQ(2, h.getNodes().size());
  EXPECT_EQ(2, h.getNodes()[0]);
  EXPECT_EQ(8, h.getNodes()[1]);

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
  Graph g("4db1abc2ade8ce");
  Graph h = extract(g, 7);

  EXPECT_EQ(2, h.getNodes().size());
  EXPECT_EQ(1, h.getNodes()[0]);
  EXPECT_EQ(8, h.getNodes()[1]);

  vector<pair<int,int>> c = h.connections(1);
  ASSERT_EQ(3, c.size());
  EXPECT_EQ(make_pair(-1,-1), c[0]);
  EXPECT_EQ(make_pair(-1,-1), c[1]);
  EXPECT_EQ(make_pair(8,0), c[2]);

  c = h.connections(8);
  ASSERT_EQ(2, c.size());
  EXPECT_EQ(make_pair(1,2), c[0]);
  EXPECT_EQ(make_pair(-1,-1), c[1]);
}

TEST(Extract, Diagram8) {
  Graph g("4abc5ecf9abd15def");
  Graph h = extract(g, 8);

  EXPECT_EQ(2, h.getNodes().size());
  EXPECT_EQ(5, h.getNodes()[0]);
  EXPECT_EQ(15, h.getNodes()[1]);

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

}