#include <vector>
#include <queue>
#include <set>
#include <string>
#include <iostream>
#include <algorithm>
#include "pichi/graph.h"
#include "string_utils.h"

using namespace std;

namespace pichi {

Graph::Graph() = default;

Graph::Graph(const string& pattern) {

  // Iterate through nodes in the graph pattern
  for (string node : splitToNodes(pattern)) {

    // Make the node
    int idx = 0;
    while(isdigit(node[idx]))
      idx++; // Find the first non-digit character

    int name = stoi(node.substr(0,idx));
    string indices = node.substr(idx);
    nodes.insert(name);

    // We use (-1,-1) for an open connection
    vector<pair<int,int>> node_conns(indices.size(), make_pair(-1,-1));

    // Look for connections to other nodes
    for (string partner : splitToNodes(pattern)) {
      idx = 0;
      while(isdigit(partner[idx]))
        idx++;
      int partner_name = stoi(partner.substr(0,idx));

      if (partner_name == name) {
        // Look for connection to self
        for (int i = 0; i < indices.size(); ++i) {
          for (int j = i+1; j < indices.size(); ++j) {
            if (indices[i] == indices[j]) {
              node_conns[i].first = name;
              node_conns[i].second = j;
              node_conns[j].first = name;
              node_conns[j].second = i;
            }
          }
        }
      }
      else {
        string partner_indices = partner.substr(idx);
        vector<pair<int,int>> match = matchChars(indices, partner_indices);
        for(pair<int,int> p : match) {
          node_conns[p.first].first = partner_name;
          node_conns[p.first].second = p.second;
        }
      }
    }

    conn.insert(make_pair(name, node_conns));

  }

}

Graph::Graph(const Graph& other) {
  // Simple copies, we have no allocated data
  nodes = other.nodes;
  conn = other.conn;
}

void Graph::addNode(int name, int connections) {
  if(!nodes.insert(name).second)
    throw invalid_argument("Node " + to_string(name) + " was already inserted");
  // The node is initialised with open connections (-1,-1)
  vector<pair<int,int>> conns(connections, make_pair(-1,-1));
  conn.insert(make_pair(name, conns));
}

void Graph::removeNode(int name) {
  // Break connections to the node to be removed
  for (pair<int,int> p : conn.at(name)) {
    if (p != make_pair(-1,-1)) {
      conn.at(p.first)[p.second].first = -1;
      conn.at(p.first)[p.second].second = -1;
    }
  }

  // Erase the node from data structures.
  conn.erase(name);
  nodes.erase(name);
}

set<int> Graph::getNodes() const {
  return nodes;
}

void Graph::connect(int node1, int conn1, int node2, int conn2) {

  if (conn.at(node1)[conn1].first != -1) {
    // Currently connected to another node. Break this connection first
    int other_node = conn.at(node1)[conn1].first;
    int other_conn = conn.at(node1)[conn1].second;
    conn.at(other_node)[other_conn].first = -1;
    conn.at(other_node)[other_conn].second = -1;
  }
  conn.at(node1)[conn1].first = node2;
  conn.at(node1)[conn1].second = conn2;

  if (conn.at(node2)[conn2].first != -1) {
    // Currently connected to another node. Break this connection first
    int other_node = conn.at(node2)[conn2].first;
    int other_conn = conn.at(node2)[conn2].second;
    conn.at(other_node)[other_conn].first = -1;
    conn.at(other_node)[other_conn].second = -1;
  }
  conn.at(node2)[conn2].first = node1;
  conn.at(node2)[conn2].second = conn1;
}

vector<pair<int,int>> Graph::connections(int node) const {
  return conn.at(node);
}

set<pair<pair<int,int>,pair<int,int>>> Graph::allConnections() const {

  // Result container to be filled
  set<pair<pair<int,int>,pair<int,int>>> r;

  // We browse through all nodes and all connections of all the nodes. When
  // we find a node with a connection to another node. We make a connection
  // to that node if
  // a) The connection is to another node with a higher value
  // b) The connection is to the same node with a higher index
  // This way we avoid duplication

  for (int node : nodes) {
    for (int i = 0; i < conn.at(node).size(); ++i) {
      pair<int,int> c = conn.at(node)[i];
      if (node < c.first) {
        r.insert(make_pair(make_pair(node,i),c));
      }
      else if (node == c.first && i < c.second) {
        r.insert(make_pair(make_pair(node,i),c));
      }
    }
  }

  return r;

}

bool Graph::contains(const Graph& other) const {
  // Iterate through all nodes of the other graph
  for (int n : other.nodes) {
    // Check that the node exists in this graph
    if (find(nodes.begin(), nodes.end(), n) == nodes.end())
      return false;

    // Check that the node is connected to the same nodes as this one
    for (int i = 0; i < conn.at(n).size(); ++i) {
      if ((other.conn.at(n)[i] != make_pair(-1,-1)) && // Skip if open indices
          (conn.at(n)[i] != other.conn.at(n)[i]))
        return false;
    }
  }
  return true;
}

bool Graph::reduce(const Graph& graph, int replacement) {

  if (!contains(graph))
    return false;

  vector<pair<int,int>> connections;
  for (int n : graph.getNodes()) {

    // Find the open connections of the input graph at node n
    vector<pair<int,int>> c = graph.connections(n);
    for (int i = 0; i < c.size(); ++i) {
      if (c[i] == make_pair<int,int>(-1,-1)) {
        // Found an open connection. Find the equivalent connection in our graph
        connections.push_back((conn[n])[i]); // i'th connection for node n
      }
    }

  }

  // Add the new node and connect it.
  addNode(replacement, connections.size());
  for (int i = 0; i < connections.size(); ++i) {
    // Connect the replacement with the rest of the graph
    if (!(connections[i] == make_pair<int,int>(-1,-1)))
      connect(replacement, i, connections[i].first, connections[i].second);
  }

  // Remove replaced nodes
  for (int n : graph.getNodes())
    removeNode(n);

  return true;

}

vector<Graph> Graph::splitToConnected() const {
  vector<Graph> r; // Result, will contain all the connected subgraphs
  set<int> not_assigned; // Set of nodes not yet assigned to any graph
  queue<int> q; // Queue of nodes waiting to be inserted into this graph

  // Initiate all nodes as unassigned
  for (int n : nodes)
    not_assigned.insert(n);

  while (!not_assigned.empty()) {
    // Create new empty graph
    Graph g;

    // Insert next unassigned node in the queue
    int start_node = *not_assigned.begin();
    g.addNode(start_node,0); // Dummy-value for number of connections.
    not_assigned.erase(start_node);
    q.push(start_node);

    while (!q.empty()) {

      int node = q.front();
      q.pop();

      // Go through connections, add to graph and assign to queue
      vector<pair<int,int>> connections = conn.at(node);
      for (pair<int,int> c : connections) {
        if (not_assigned.find(c.first) != not_assigned.end()) {
          // First time we see this node: add and move to queue
          g.addNode(c.first,0);
          not_assigned.erase(c.first);
          q.push(c.first);
        }
      }

      // Add connections
      g.conn[node] = connections;

    }

    // No more nodes in queue. The graph is done.
    r.push_back(g);

  }

  return r;
}

bool Graph::operator==(const pichi::Graph& rhs) const {
  if (nodes == rhs.nodes)
    return conn == rhs.conn;
  else
    return false;
}

bool Graph::operator<(const pichi::Graph& rhs) const {
  if (nodes == rhs.nodes)
    return conn < rhs.conn;
  else
    return nodes < rhs.nodes;
}

}
