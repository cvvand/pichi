#include "string_utils.h"
#include <queue>

using namespace std;

namespace pichi {

vector<pair<int,int>> matchChars(string s1, string s2) {

  vector<pair<int,int>> res;

  for (int i = 0; i < s2.length(); ++i) {
    int pos = s1.find(s2[i]);
    if (pos != string::npos)
      res.push_back(make_pair(pos,i));
  }

  return res;

};

vector<string> splitToNodes(const string& s) {
  vector<string> nodes;    // Container for the nodes
  string this_node;       // The current node
  bool digit_last = true; // Currently looking at a multi digit number?

  for (char c : s) {
    if (isdigit(c)) {
      // c is a digit
      if (isdigit(c) && !digit_last) {
        // The last char was an index, so this is a new node.
        // Save the old node and start a new one.
        nodes.push_back(this_node);
        this_node.clear();
        digit_last = true;
      }
    } else {
      // c is not a digit
      digit_last = false;
    }
    // Add c to the current node
    this_node += c;
  }
  if (!this_node.empty()) {
    // Push the last node into the queue
    nodes.push_back(this_node);
  }

  return nodes;
}

vector<string> splitToConnected(const string& s) {

  /* We first split the string into nodes:
   * "0abc1ad2de3bce" --> "0abc", "1ad", "2de", "3bce".
   */
  queue<string> nodes;    // Container for the nodes
  vector<string> temp = splitToNodes(s);
  for (string ss : temp)
    nodes.push(ss);

  // Now we add connected nodes into components
  vector<string> comps; // The connected components
  while (!nodes.empty()) {

    // Start a new component
    string this_comp = nodes.front();
    nodes.pop();

    // Check is the remaining nodes can be added to the component
    int count = 0;
    while (count++ < nodes.size()) {

      // Grab the next node
      string node = nodes.front();
      nodes.pop();

      for (char c : node) {
        // Check if any of the indices of the node matches the component
        if (!isdigit(c) && this_comp.find(c) != string::npos) {

          // The index matches. We add the node to the component
          this_comp += node;

          // We now have to check all the remaining nodes again, since we
          // might have new indices that were not in the component before.
          count = 0;
          break;
        }
      }
      if (count != 0) {
        // No more nodes, so we're done.
        nodes.push(node);
      }

    }

    // Add this component to the list.
    comps.push_back(this_comp);

    // If there are remaining nodes we start a new component. Otherwise
    // we're done.

  }

  return comps;

}


}
