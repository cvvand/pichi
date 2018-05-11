
#include "util.h"
#include <queue>
#include <iostream>

using namespace pichi;
using namespace std;

namespace pichi {

vector<string> split(string s) {

  queue<string> nodes;
  string this_node;
  for (char c : s) {
    if (isdigit(c)) {
      if (!this_node.empty()) {
        nodes.push(this_node);
        this_node.clear();
      }
    }
    this_node += c;
  }
  if (!this_node.empty()) {
    nodes.push(this_node);
  }

  vector<string> comps;
  while (!nodes.empty()) {

    string this_comp = nodes.front();
    nodes.pop();

    int count = 0;
    while (count++ < nodes.size()) {
      string node = nodes.front();
      nodes.pop();

      for (char c : this_comp) {
        if (node.find(c) != string::npos) {
          this_comp += node;
          count = 0;
          break;
        }
      }
      if (count != 0) {
        nodes.push(node);
      }

    }

    comps.push_back(this_comp);

  }


  return comps;
}

}

