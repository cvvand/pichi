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

TEST(Graph, ConnectOneNode) {

  Graph g("3aa");

  vector<pair<int,int>> c = g.connections(3);
  ASSERT_EQ(2,c.size());

  EXPECT_EQ(3, c[0].first);
  EXPECT_EQ(1, c[0].second);

  EXPECT_EQ(3, c[1].first);
  EXPECT_EQ(0, c[1].second);

}

TEST(Graph, ConnectTwoNodes) {

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



}

