#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include "graph.h"
#include "string_utils.h"

using namespace std;

namespace pichi {

Graph::Graph(const string& pattern) {

  // Iterate through nodes in the graph pattern
  for (string node : splitToNodes(pattern)) {

    // Make the node
    int idx = 0;
    while(isdigit(node[idx]))
      idx++; // Find the first non-digit character

    int name = stoi(node.substr(0,idx));
    string indices = node.substr(idx);
    nodes.push_back(name);

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
  nodes.push_back(name);
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
  nodes.erase( remove(nodes.begin(), nodes.end(), name));
}

vector<int> Graph::getNodes() const {
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

}
