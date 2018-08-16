#include "pichi/graph.h"
#include "gtest/gtest.h"

/*
 * Unit tests of the contraction logic defined in GRAPH.CC
 */

using namespace pichi;
using namespace std;

namespace {

TEST(Graph, DefaultConstructorMakesEmptyGraph) {
  Graph g;
  ASSERT_EQ(0, g.getNodes().size());
}

TEST(Graph, CreateGraphWithOneNode) {

  Graph g("0aa");
  auto s = g.getNodes();
  ASSERT_EQ(1, s.size());
  EXPECT_NE(s.find(0) , s.end());

}

TEST(Graph, CreateGraphWithTwoNodes) {

  Graph g("0ab2ab");
  auto s = g.getNodes();
  ASSERT_EQ(2, s.size());
  EXPECT_NE(s.find(0) , s.end());
  EXPECT_NE(s.find(2) , s.end());

}

TEST(Graph, OneConnectedNode) {

  Graph g("3aa");

  vector<pair<int,int>> c = g.connections(3);
  ASSERT_EQ(2,c.size());

  EXPECT_EQ(3, c[0].first);
  EXPECT_EQ(1, c[0].second);

  EXPECT_EQ(3, c[1].first);
  EXPECT_EQ(0, c[1].second);

}

TEST(Graph, TwoConnectedNodes) {

  Graph g("3ab12ba");

  vector<pair<int,int>> c = g.connections(3);
  ASSERT_EQ(2,c.size());

  EXPECT_EQ(12, c[0].first);
  EXPECT_EQ(1, c[0].second);

  EXPECT_EQ(12, c[1].first);
  EXPECT_EQ(0, c[1].second);

  c = g.connections(12);
  ASSERT_EQ(2,c.size());

  EXPECT_EQ(3, c[0].first);
  EXPECT_EQ(1, c[0].second);

  EXPECT_EQ(3, c[1].first);
  EXPECT_EQ(0, c[1].second);

}

TEST(Graph, OpenNode) {

  Graph g("3ab12bc");

  vector<pair<int,int>> c = g.connections(3);
  ASSERT_EQ(2,c.size());

  EXPECT_EQ(-1, c[0].first);
  EXPECT_EQ(-1, c[0].second);

  EXPECT_EQ(12, c[1].first);
  EXPECT_EQ(0, c[1].second);

  c = g.connections(12);
  ASSERT_EQ(2,c.size());

  EXPECT_EQ(3, c[0].first);
  EXPECT_EQ(1, c[0].second);

  EXPECT_EQ(-1, c[1].first);
  EXPECT_EQ(-1, c[1].second);

}

TEST(Graph, DontAcceptDuplicateNode) {
  Graph g("1ab2bc");
  EXPECT_THROW(g.addNode(1,2), invalid_argument);
}

TEST(Graph, AddNodeWithTwoOpenConnections) {
  Graph g("0ab1bc");
  g.addNode(4, 2);

  auto s = g.getNodes();
  ASSERT_EQ(3, s.size());
  ASSERT_NE(s.find(4),s.end());
  vector<pair<int,int>> c = g.connections(4);
  ASSERT_EQ(2, c.size());
  EXPECT_EQ(make_pair(-1,-1), c[0]);
  EXPECT_EQ(make_pair(-1,-1), c[1]);
}

TEST(Graph, RemoveNodeWithTwoOpenConnections) {
  Graph g("0ab1bc");
  g.addNode(4, 2);
  ASSERT_EQ(3, g.getNodes().size());
  g.removeNode(4);
  auto s = g.getNodes();
  ASSERT_EQ(2, s.size());
  EXPECT_NE(s.find(0), s.end());
  EXPECT_NE(s.find(1), s.end());
  EXPECT_EQ(s.find(4), s.end());
}

TEST(Graph, RemoveNodeWithTwoConnections) {
  Graph g("0ab1bc2ca");
  g.removeNode(1);
  auto s = g.getNodes();
  ASSERT_EQ(2, s.size());
  EXPECT_NE(s.find(0), s.end());
  EXPECT_NE(s.find(2), s.end());

  vector<pair<int,int>> c = g.connections(0);
  EXPECT_EQ(make_pair(2,1), c[0]);
  EXPECT_EQ(make_pair(-1,-1), c[1]);

  c = g.connections(2);
  EXPECT_EQ(make_pair(-1,-1), c[0]);
  EXPECT_EQ(make_pair(0,0), c[1]);

  EXPECT_THROW(g.connections(1), out_of_range);
}

TEST(Graph, ConnectTwoOpenNodes) {
  Graph g("0ab1cd");
  g.connect(0,0,1,1);
  EXPECT_EQ(make_pair(1,1), g.connections(0)[0]);
  EXPECT_EQ(make_pair(-1,-1), g.connections(0)[1]);
  EXPECT_EQ(make_pair(-1,-1), g.connections(1)[0]);
  EXPECT_EQ(make_pair(0,0), g.connections(1)[1]);

}

TEST(Graph, ConnectConnectedNodes) {
  Graph g("0ab1ab2cc");
  g.connect(2,0,0,0);

  EXPECT_EQ(make_pair(2,0), g.connections(0)[0]);
  EXPECT_EQ(make_pair(1,1), g.connections(0)[1]);

  EXPECT_EQ(make_pair(-1,-1), g.connections(1)[0]);
  EXPECT_EQ(make_pair(0,1), g.connections(1)[1]);

  EXPECT_EQ(make_pair(0,0), g.connections(2)[0]);
  EXPECT_EQ(make_pair(-1,-1), g.connections(2)[1]);

}

TEST(Graph, AllConnectionsEmptyOnNonConnectedGraph) {
  Graph g("1ab");
  EXPECT_TRUE(g.allConnections().empty());
}

TEST(Graph, SingleConnectionInGraphWithOneNodeConnectedToItself) {
  Graph g("1aa");
  auto s = g.allConnections();
  ASSERT_EQ(1, s.size());
  EXPECT_NE(s.find(make_pair(make_pair(1,0),make_pair(1,1))), s.end());
}

TEST(Graph, SingleConnectionInGraphWithTwoNodesConnectedAtSameIndex) {
  Graph g("1ab2cb");
  auto s = g.allConnections();
  ASSERT_EQ(1, s.size());
  EXPECT_NE(s.find(make_pair(make_pair(1,1),make_pair(2,1))), s.end());
}


TEST(GraphContains, FailOnMissingNode) {
  Graph g1("3ab12ab");
  Graph g2("4ab");
  EXPECT_FALSE(g1.contains(g2));
}

TEST(GraphContains, SuccessOnExistingNode) {
  Graph g1("3ab12ab");
  Graph g2("3qr");
  EXPECT_TRUE(g1.contains(g2));
}

TEST(GraphContains, FailOnOneMissinOneExistingNode) {
  Graph g1("3ab12ab");
  Graph g2("3qr4hu");
  EXPECT_FALSE(g1.contains(g2));
}

TEST(GraphContains, FailOnExistingButWronglyConnectedNodes) {
  Graph g1("3ab12ab");
  Graph g2("3qr12rq");
  EXPECT_FALSE(g1.contains(g2));
}

TEST(GraphContains, ComplicatedTest) {
  Graph g1("3abc12abd56cd8ee");
  Graph g2("3abc12ade56fe");
  Graph g3("8ab");
  Graph g4("3abc12adc56fe");
  EXPECT_TRUE(g1.contains(g2));
  EXPECT_TRUE(g1.contains(g3));
  EXPECT_FALSE(g1.contains(g4));
}

TEST(GraphToString, NoNodesGivesEmptyString) {
  Graph g1;
  EXPECT_EQ("", g1.toString());
}

TEST(GraphToString, SingleNodeConnectedToSelf) {
  Graph g1("1aa");
  EXPECT_EQ("1:(1,1)(1,0) ", g1.toString());
}

TEST(GraphToString, TwoNodesWithOpenConnections) {
  Graph g1("1ab2cdb");
  EXPECT_EQ("1:(-1,-1)(2,2) 2:(-1,-1)(-1,-1)(1,1) ", g1.toString());
}


TEST(GraphCopyConstructor, CopyNodesAndConnections) {
  Graph g("0ab1acd2bce");
  Graph h(g);

  g.addNode(3, 3);
  g.connect(1,2,2,2);

  auto nodes = h.getNodes();
  ASSERT_EQ(3, nodes.size());
  EXPECT_NE(nodes.find(0), nodes.end());
  EXPECT_NE(nodes.find(1), nodes.end());
  EXPECT_NE(nodes.find(2), nodes.end());

  vector<pair<int,int>> c = h.connections(0);
  ASSERT_EQ(2, c.size());
  EXPECT_EQ(make_pair(1,0), c[0]);
  EXPECT_EQ(make_pair(2,0), c[1]);

  c = h.connections(1);
  ASSERT_EQ(3, c.size());
  EXPECT_EQ(make_pair(0,0), c[0]);
  EXPECT_EQ(make_pair(2,1), c[1]);
  EXPECT_EQ(make_pair(-1,-1), c[2]);

  c = h.connections(2);
  ASSERT_EQ(3, c.size());
  EXPECT_EQ(make_pair(0,1), c[0]);
  EXPECT_EQ(make_pair(1,1), c[1]);
  EXPECT_EQ(make_pair(-1,-1), c[2]);
}



TEST(GraphCompare, EmptyGraphsEqual) {
  Graph g1; Graph g2;
  EXPECT_TRUE(g1 == g2);
  EXPECT_FALSE(g1 != g2);
  EXPECT_FALSE(g1 < g2);
  EXPECT_FALSE(g1 > g2);
  EXPECT_TRUE(g1 <= g2);
  EXPECT_TRUE(g1 >= g2);
}

TEST(GraphCompare, MoreNodesGreaterThanFewerNodes) {
  Graph g1("1ab"); Graph g2;
  EXPECT_FALSE(g1 == g2);
  EXPECT_TRUE(g1 != g2);
  EXPECT_FALSE(g1 < g2);
  EXPECT_TRUE(g1 > g2);
  EXPECT_FALSE(g1 <= g2);
  EXPECT_TRUE(g1 >= g2);
}

TEST(GraphCompare, HighValueNodesGreaterThanSmallValueNodes) {
  Graph g1("1ab"); Graph g2("2ab");
  EXPECT_FALSE(g1 == g2);
  EXPECT_TRUE(g1 != g2);
  EXPECT_TRUE(g1 < g2);
  EXPECT_FALSE(g1 > g2);
  EXPECT_TRUE(g1 <= g2);
  EXPECT_FALSE(g1 >= g2);
}

TEST(GraphCompare, SameNodeOneClosedOneOpen) {
  Graph g1("1ab"); Graph g2("1aa");
  EXPECT_FALSE(g1 == g2);
  EXPECT_TRUE(g1 != g2);
  EXPECT_TRUE(g1 < g2);
  EXPECT_FALSE(g1 > g2);
  EXPECT_TRUE(g1 <= g2);
  EXPECT_FALSE(g1 >= g2);
}





TEST(GraphReduce, ReduceThreeRank2NodesToTwoRankTwoNodes) {
  Graph g("0ab1bc2ac");
  Graph h("1ab2cb");

  ASSERT_TRUE(g.reduce(h, 3));

  auto s = g.getNodes();
  ASSERT_EQ(2, s.size());
  EXPECT_NE(s.find(0), s.end());
  EXPECT_NE(s.find(3), s.end());

  vector<pair<int,int>> c = g.connections(0);

  ASSERT_EQ(2, c.size());
  EXPECT_EQ(make_pair(3,1), c[0]);
  EXPECT_EQ(make_pair(3,0), c[1]);

  c = g.connections(3);

  ASSERT_EQ(2, c.size());
  EXPECT_EQ(make_pair(0,1), c[0]);
  EXPECT_EQ(make_pair(0,0), c[1]);

}

TEST(GraphReduce, DontReduceIfGraphIsNotThere) {
  Graph g("0ab1bc2ac");
  Graph h("3ab1cb");

  ASSERT_FALSE(g.reduce(h, 4));

  auto s = g.getNodes();
  ASSERT_EQ(3, s.size());
  EXPECT_NE(s.find(0), s.end());
  EXPECT_NE(s.find(1), s.end());
  EXPECT_NE(s.find(2), s.end());

}

TEST(GraphReduce, ReduceGraphWithOpenConnections) {
  Graph g("0ab1bcd2de");
  Graph h("1abc2cd");

  ASSERT_TRUE(g.reduce(h, 3));

  auto s = g.getNodes();
  ASSERT_EQ(2, g.getNodes().size());
  EXPECT_NE(s.find(0), s.end());
  EXPECT_NE(s.find(3), s.end());

  vector<pair<int,int>> c = g.connections(0);

  ASSERT_EQ(2, c.size());
  EXPECT_EQ(make_pair(-1,-1), c[0]);
  EXPECT_EQ(make_pair(3,0), c[1]);

  c = g.connections(3);

  ASSERT_EQ(3, c.size());
  EXPECT_EQ(make_pair(0,1), c[0]);
  EXPECT_EQ(make_pair(-1,-1), c[1]);
  EXPECT_EQ(make_pair(-1,-1), c[2]);

}

TEST(GraphReduce, ReduceSingleRank2NodeToSingleRank0Node) {
  Graph g("0aa");
  Graph h("0bb");

  ASSERT_TRUE(g.reduce(h, 3));

  ASSERT_EQ(1, g.getNodes().size());
  EXPECT_NE(g.getNodes().find(3), g.getNodes().end());

  vector<pair<int,int>> c = g.connections(3);

  ASSERT_EQ(0, c.size());

}

TEST(GraphReduce, ReduceTwoRank2NodesToSingleRank0Node) {
  Graph g("0ab18ab");
  Graph h("0vb18vb");

  ASSERT_TRUE(g.reduce(h, 3));

  ASSERT_EQ(1, g.getNodes().size());
  EXPECT_NE(g.getNodes().find(3), g.getNodes().end());

  vector<pair<int,int>> c = g.connections(3);

  ASSERT_EQ(0, c.size());

}

TEST(GraphReduce, DontReduceWithDuplicateName) {
  Graph g("0ab1bc2ac");
  Graph h("0ab1bc");
  EXPECT_THROW(g.reduce(h, 2), invalid_argument);
}


}


