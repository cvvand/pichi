#include "gtest/gtest.h"
#include "graph.h"

/*
 * Unit tests of the splitToConnected function defined in Graph.cc
 */

using namespace pichi;
using namespace std;

namespace {

TEST(GraphSplit, ConnectedGraphReturnCopy) {
  Graph g("1ab2ab");
  set<Graph> r = g.splitToConnected();

  ASSERT_EQ(1, r.size());

  EXPECT_NE(r.find(Graph("1gh2gh")), r.end());
}

}