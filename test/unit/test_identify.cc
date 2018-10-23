#include "diagrams.h"
#include "gtest/gtest.h"

/*
 * Unit tests of the identifyDiagram function implemented in DIAGRAMS.CC
 */

using namespace pichi;
using namespace std;

namespace {

TEST(IdentifyDiagram, Diagram0) {
  Graph g("4gg");
  EXPECT_EQ(0, identifyDiagram(g));
}

TEST(IdentifyDiagram, SingleUnconnectedRank2Unknown) {
  Graph g("4ab");
  EXPECT_EQ(-1, identifyDiagram(g));
}

TEST(IdentifyDiagram, Diagram1) {
  Graph g("4gd12dg");
  EXPECT_EQ(1, identifyDiagram(g));
}

TEST(IdentifyDiagram, Diagram2) {
  Graph g("4gd12xg321xd");
  EXPECT_EQ(2, identifyDiagram(g));
}

TEST(IdentifyDiagram, Diagram3) {
  Graph g("4gd12xh321xd21hg");
  EXPECT_EQ(3, identifyDiagram(g));
}

TEST(IdentifyDiagram, Diagram4) {
  Graph g("4gdi44igd");
  EXPECT_EQ(4, identifyDiagram(g));
}

TEST(IdentifyDiagram, Diagram5) {
  Graph g("4gde44kgd90ek");
  EXPECT_EQ(5, identifyDiagram(g));
}

TEST(IdentifyDiagram, Diagram6) {
  Graph g("90ex4gde44kgd111xk");
  EXPECT_EQ(6, identifyDiagram(g));
}

TEST(IdentifyDiagram, Diagram7) {
  Graph g("4eup44xuk90ex111pk");
  EXPECT_EQ(7, identifyDiagram(g));
}

TEST(IdentifyDiagram, Diagram8) {
  Graph g("4ckp44cix90xiy111pky");
  EXPECT_EQ(8, identifyDiagram(g));
}

TEST(IdentifyDiagram, FiveTensorsUnknownDiagram) {
  Graph g("1ae2ab3bc4cd5de");
  EXPECT_EQ(-1, identifyDiagram(g));
}

TEST(IdentifyDiagram, Rank4TensorUnknown) {
  Graph g("1abcd2abee3cd");
  EXPECT_EQ(-1, identifyDiagram(g));
}

TEST(IdentifyDiagram, DisconnectedUnknown) {
  Graph g("1aa3cc");
  EXPECT_EQ(-1, identifyDiagram(g));
}

}