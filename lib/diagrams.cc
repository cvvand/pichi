#include <iostream>
#include <armadillo>
#include "diagrams.h"
#include "pichi/graph.h"
#include "pichi/contraction.h"

using namespace std;


namespace pichi {



int identifyDiagram(const Graph& graph) {
  // Count the node degrees
  int nodes2 = 0;
  int nodes3 = 0;
  for (int node : graph.getNodes()) {
    if (graph.connections(node).size() == 2)
      ++nodes2;
    else
      ++nodes3;
  }
  // We find the correct diagram from the node count.
  if (nodes2 == 1 && nodes3 == 0)
    return 0;
  if (nodes2 == 2 && nodes3 == 0)
    return 1;
  if (nodes2 == 3)
    return 2;
  if (nodes2 == 4)
    return 3;
  if (nodes2 == 0 && nodes3 == 2)
    return 4;
  if (nodes2 == 1 && nodes3 == 2)
    return 5;
  if (nodes2 == 2 && nodes3 == 2) {
    // This can be both diagram 6 and 7
    // We start by getting the first node in the graph
    vector<pair<int,int>> c0 = graph.connections(*graph.getNodes().begin());
    if (c0.size() == 2) {
      // The first node has two connections. If one of them is to another
      // node with two connections, its a diagram 6. Otherwise it's 7.
      if (graph.connections(c0[0].first).size() == 2)
        return 6;
      if (graph.connections(c0[1].first).size() == 2)
        return 6;
      return 7;
    }
    else {
      // Thge first node has three connections. We count the number of
      // connections to the other node with three connections. If it's 2 the
      // diagram is number 6, if it's 1 the diagram is nunber 7.
      int c3 = 0;
      if (graph.connections(c0[0].first).size() == 3)
        ++c3;
      if (graph.connections(c0[1].first).size() == 3)
        ++c3;
      if (graph.connections(c0[2].first).size() == 3)
        ++c3;
      if (c3 == 2)
        return 6;
      return 7;
    }
  }
  return 8;
}

Graph extract(const Graph& graph, int diagram) {
  Graph ext(graph);

  switch(diagram) {

    case 2: {
      // simply remove the first node
      ext.removeNode(*ext.getNodes().begin());
      break;
    }

    case 3: {
      // Remove the first node and its first connection
      int node1 = *ext.getNodes().begin();
      int node2 = ext.connections(node1)[0].first;
      ext.removeNode(node1);
      ext.removeNode(node2);
      break;
    }

    case 5: {
      // Remove the first node with three connections
      for (int n : graph.getNodes()) {
        if (graph.connections(n).size() == 3) {
          ext.removeNode(n);
          break;
        }
      }
      break;
    }

    case 6: {
      // Remove the nodes with three connections
      for (int n : graph.getNodes()) {
        if (graph.connections(n).size() == 3) {
          ext.removeNode(n);
        }
      }
      break;
    }

    case 7: {
      // We remove the first node
      int node1 = *ext.getNodes().begin();
      ext.removeNode(node1);
      // If the first node has two connections, remove the first connected node
      if (graph.connections(node1).size() == 2) {
        ext.removeNode(graph.connections(node1)[0].first);
      }
        // If the first node has three connections, we remove the first connected
        // node with two connections
      else {
        for (pair<int, int> p : graph.connections(node1)) {
          if (graph.connections(p.first).size() == 2) {
            ext.removeNode(p.first);
            break;
          }
        }
      }
      break;
    }

    case 8: {
      // Remove the first node and the node connected to it with two connections
      int node1 = *ext.getNodes().begin();
      ext.removeNode(node1);
      int nodex, nodey;
      int cx = 0;
      nodex = graph.connections(node1)[0].first;
      for (pair<int, int> p : graph.connections(node1)) {
        if (p.first == nodex)
          cx++;
        else {
          nodey = p.first;
        }
      }
      if (cx == 2)
        ext.removeNode(nodex);
      else {
        ext.removeNode(nodey);
      }
      break;
    }

    default:{} // Do nothing

  }

  return ext;
}



}

