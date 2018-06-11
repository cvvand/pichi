#include "graph.h"
#include "gtest/gtest.h"

/*
 * Unit tests of the contraction logic defined in GRAPH.CC
 */

using namespace pichi;
using namespace std;

namespace {

TEST(Graph, CreateGraphWithOneNode) {

  Graph g("0aa");
  ASSERT_EQ(1, g.getNodes().size());
  EXPECT_EQ(0, g.getNodes()[0]);

}

TEST(Graph, CreateGraphWithTwoNodes) {

  Graph g("0ab2ab");
  ASSERT_EQ(2, g.getNodes().size());
  EXPECT_EQ(0, g.getNodes()[0]);
  EXPECT_EQ(2, g.getNodes()[1]);

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

TEST(Graph, AddNodeWithTwoOpenConnections) {
  Graph g("0ab1bc");
  g.addNode(4, 2);
  ASSERT_EQ(3, g.getNodes().size());
  ASSERT_EQ(4, g.getNodes()[2]);
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
  ASSERT_EQ(2, g.getNodes().size());
  EXPECT_EQ(0, g.getNodes()[0]);
  EXPECT_EQ(1, g.getNodes()[1]);
}

TEST(Graph, RemoveNodeWithTwoConnections) {
  Graph g("0ab1bc2ca");
  g.removeNode(1);
  ASSERT_EQ(2, g.getNodes().size());
  EXPECT_EQ(0, g.getNodes()[0]);
  EXPECT_EQ(2, g.getNodes()[1]);

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

TEST(GraphCopyConstructor, CopyNodesAndConnections) {
  Graph g("0ab1acd2bce");
  Graph h(g);

  g.addNode(3, 3);
  g.connect(1,2,2,2);

  vector<int> nodes = h.getNodes();
  ASSERT_EQ(3, nodes.size());
  EXPECT_EQ(0, nodes[0]);
  EXPECT_EQ(1, nodes[1]);
  EXPECT_EQ(2, nodes[2]);

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


TEST(GraphReduce, ReduceThreeRank2NodesToTwoRankTwoNodes) {
  Graph g("0ab1bc2ac");
  Graph h("1ab2cb");

  ASSERT_TRUE(g.reduce(h, 3));

  ASSERT_EQ(2, g.getNodes().size());
  EXPECT_EQ(0, g.getNodes()[0]);
  EXPECT_EQ(3, g.getNodes()[1]);

  vector<pair<int,int>> c = g.connections(0);

  ASSERT_EQ(2, c.size());
  EXPECT_EQ(make_pair(3,1), c[0]);
  EXPECT_EQ(make_pair(3,0), c[1]);

  c = g.connections(3);

  ASSERT_EQ(2, c.size());
  EXPECT_EQ(make_pair(0,1), c[0]);
  EXPECT_EQ(make_pair(0,0), c[1]);

}

TEST(GraphReduce, ReduceThreeRank2NodesToTwoInOppositeOrder) {
  Graph g("0ab1bc2ac");
  Graph h("2ab1cb");

  ASSERT_TRUE(g.reduce(h, 4));

  ASSERT_EQ(2, g.getNodes().size());
  EXPECT_EQ(0, g.getNodes()[0]);
  EXPECT_EQ(4, g.getNodes()[1]);

  vector<pair<int,int>> c = g.connections(0);

  ASSERT_EQ(2, c.size());
  EXPECT_EQ(make_pair(4,0), c[0]);
  EXPECT_EQ(make_pair(4,1), c[1]);

  c = g.connections(4);

  ASSERT_EQ(2, c.size());
  EXPECT_EQ(make_pair(0,0), c[0]);
  EXPECT_EQ(make_pair(0,1), c[1]);

}

TEST(GraphReduce, DontReduceIfGraphIsNotThere) {
  Graph g("0ab1bc2ac");
  Graph h("3ab1cb");

  ASSERT_FALSE(g.reduce(h, 4));

  ASSERT_EQ(3, g.getNodes().size());
  EXPECT_EQ(0, g.getNodes()[0]);
  EXPECT_EQ(1, g.getNodes()[1]);
  EXPECT_EQ(2, g.getNodes()[2]);

}

TEST(GraphReduce, ReduceGraphWithOpenConnections) {
  Graph g("0ab1bcd2de");
  Graph h("1abc2cd");

  ASSERT_TRUE(g.reduce(h, 3));

  ASSERT_EQ(2, g.getNodes().size());
  EXPECT_EQ(0, g.getNodes()[0]);
  EXPECT_EQ(3, g.getNodes()[1]);

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
  EXPECT_EQ(3, g.getNodes()[0]);

  vector<pair<int,int>> c = g.connections(3);

  ASSERT_EQ(0, c.size());

}

TEST(GraphReduce, ReduceTwoRank2NodesToSingleRank0Node) {
  Graph g("0ab18ab");
  Graph h("0vb18vb");

  ASSERT_TRUE(g.reduce(h, 3));

  ASSERT_EQ(1, g.getNodes().size());
  EXPECT_EQ(3, g.getNodes()[0]);

  vector<pair<int,int>> c = g.connections(3);

  ASSERT_EQ(0, c.size());

}


}


