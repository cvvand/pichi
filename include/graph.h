#ifndef PICHI_GRAPH_H
#define PICHI_GRAPH_H

#include <vector>
#include <string>
#include <map>
#include <set>

namespace pichi {

/* ************************************************************************
 *
 * This file contains the definition of a Graph in the context of PICHI.
 *
 * A Graph consists of nodes and edges. In this implementation, each node is
 * identified by an integer (which should be positive, although this is
 * currently not reflected by the type). Contrary to most Graph
 * implementations, each node is connected to the other nodes by a fixed
 * number of ordered edges. To impress this point, we use the phrase
 * "connections" rather than "edges". We use an adjacency-like implementation
 * to keep track of the connections between the nodes.
 *
 * For example:
 * Consider a graph with three nodes, 10, 24 and 39. Each node has a fixed
 * number of connections. Node 1 and 3 have three connections and node 2 has
 * two connections. We use the following notation:
 *
 * N10: [(39,0),(24,1),(39,1)]
 * N24: [(39,2),(10,1)]           (*) See constructor docs to
 * N39: [(10,0),(10,2),(24,0)]        see how to create this example.
 *
 * Each connection on a node is signified by two numbers (X,Y). X is the node
 * to which the connection flows, and Y is the connection index on the
 * receiving node. We see in the example that the first connection on Node "10"
 * goes to the first connection on Node "39", signified by (39,0), while the
 * second connection goes to the second connection on Node "24" (24,1).
 *
 * A connection can also flow to another connection on the same node:
 * N56: [(56,1),(56,0)]
 * Here, the first connection flows to the second connection of Node "56"
 * (56,1) and vice versa.
 *
 * Finally, a connection can also be open, meaning that it doesn't connect to
 * anything. This is signified by (-1,-1):
 * N10: [(10,2),(-1,-1),(10,0)]
 * Here, the first and third connections are to each other, while the second
 * connection is open.
 *
 *
 * ***********************************************************************/

class Graph {

public:
  /*
   * Creates an empty graph with no nodes.
   */
  Graph();

  /*
   * Creates a graph from a string representation.
   * A possible input could be
   * "10abc23db39acd"
   * which would create the example in the introduction (*)
   * The numbers are the node names. A repeated index signifies a connection.
   * In order to create an open index, use a non-repeated index:
   * "10abc23db39ecd"
   * This example is identical to the one above, except the first connection
   * from 10 to 39 is now broken.
   */
  Graph(const std::string&);

  /*
   * Copy constructor.
   * Creates a copy of the input graph.
   */
  Graph(const Graph&);



  /*
   * Add a named node to the graph with a given number of open connections.
   */
  void addNode(int name, int connections);

  /*
   * Removes a node from the graph.
   * Any connections to this node will be made open.
   */
  void removeNode(int);

  /*
   * Gets a list of all the nodes in the graph
   */
  std::set<int> getNodes() const;

  /*
   * Connects two nodes of a graph using the indicated connections. If the
   * connections are already in use, they will first be broken and made open,
   * before being reconnected.
   */
  void connect(int node1, int conn1, int node2, int conn2);

  /*
   * Gets a list of all the connections from a given node.
   * Each pair of integers in the output represents a connection, and the
   * order matters. The first integer in a pair is the receiving node, and
   * the second integer is the connection index on the receiving node.
   * An open connection is indicated by the pair (-1,-1).
   */
  std::vector<std::pair<int,int>> connections(int node) const;

  /*
   * Gets all the connections in the graph. Each connection is represented by
   * a pair of pairs of integers. The first pair in the connection is (node1,
   * index1), the second pair being (node2,index2).
   */
  std::set<std::pair<std::pair<int,int>,std::pair<int,int>>>
      allConnections() const;

  /*
   * Checks whether a graph contains another graph.
   * A graph h is contained in another graph g, if all the nodes of h also
   * exist in g, and if any non-open connection between two nodes in h is
   * also present in g, in the same order.
   */
  bool contains(const Graph& graph) const;

  /*
   * Reduces the graph by replacing a part of the graph with a single node.
   * For example, consider the graph "A_ab B_bc C_ca". If we reduce this
   * graph using the reduction graph "B_ab C_bc" using the replacement node
   * "D", we get the resulting graph "A_ab D_ba".
   *
   * Note that the ordering of nodes matters. If we had instead reduced the
   * graph above with the graph "C_ab B_bc", the resulting graph would have
   * been instead "A_ab D_ab" in order to preserve connections.
   *
   * Graphs with open connections can also be reduced.
   *
   * Returns true if the subgraph was successfully replaced. Returns false
   * otherwise and leaves the graph unchanged.
   */
  bool reduce(const Graph& graph, int replacement);

  /*
   * Splits the graph into a list of connected graphs. This has no effect on
   * the calling graph, and if the graph is already completely connected, the
   * only element in the output list will be a copy of the calling graph.
   */
  std::vector<Graph> splitToConnected() const;

  /*
   * Comparison operators
   */
  bool operator==(const Graph& rhs) const;
  bool operator!=(const Graph& rhs) const {
    return !operator==(rhs);
  }
  bool operator<(const Graph& rhs) const;
  bool operator>(const Graph& rhs) const {
    return rhs.operator<(*this);
  }
  bool operator<=(const Graph& rhs) const {
    return !operator>(rhs);
  }
  bool operator>=(const Graph& rhs) const {
    return !operator<(rhs);
  }

private:
  /*
   * List of all nodes and connections
   */
  std::set<int> nodes;
  std::map<int,std::vector<std::pair<int,int>>> conn;

};



}

#endif //PICHI_GRAPH_H
