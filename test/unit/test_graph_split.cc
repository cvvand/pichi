#include "gtest/gtest.h"
#include "pichi/graph.h"

/*
 * Unit tests of the splitToConnected function defined in Graph.cc
 */

using namespace pichi;
using namespace std;

namespace {

TEST(GraphSplit, ConnectedGraphReturnCopy) {
  Graph g("1ab2ab");
  vector<Graph> r = g.splitToConnected();

  ASSERT_EQ(1, r.size());

  EXPECT_EQ(r[0], Graph("1gh2gh"));
}

TEST(GraphSplit, TwoDisconnectedSingleNodeGraphs) {
  Graph g("1aa2bc");
  vector<Graph> r = g.splitToConnected();

  ASSERT_EQ(2, r.size());

  EXPECT_EQ(r[0], Graph("1bb"));
  EXPECT_EQ(r[1], Graph("2bc"));
}

TEST(GraphSplit, TwoConnectedNodesAndOneNodeWithNoConnections) {
  Graph g("0ab1ab");
  g.addNode(2,0);
  vector<Graph> r = g.splitToConnected();

  ASSERT_EQ(2, r.size());

  EXPECT_EQ(r[0], Graph("0ab1ab"));
  EXPECT_EQ(r[1], Graph("2"));
}

}