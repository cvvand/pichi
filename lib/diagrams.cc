#include <armadillo>
#include "diagrams.h"
#include "contraction.h"
#include "tensor.h"

using namespace arma;
using namespace std;


/* Utility functions */
namespace {

/*
 * Matches two strings:
 * Finds the subset of characters in the strings which occurs in both. The
 * result is a list of pairs of integers giving the position of the matches.
 *
 * Example:
 *    "abcd"+"aebf" -> [(0,0),(1,2)] ,
 * since "a" is in position 0 and 0, and "b" is in position 1 and 2.
 *
 * The result is empty if there are no matches.
 */
vector<pair<int,int>> matchChars(string s1, string s2) {

  vector<pair<int,int>> res;

  for (int i = 0; i < s2.length(); ++i) {
    int pos = s1.find(s2[i]);
    if (pos != string::npos)
      res.push_back(make_pair(pos,i));
  }

  return res;

};

/*
 * Combines two strings, cutting away the duplicates:
 * This function basically finds the symmetric difference between the two
 * strings. The result is this difference, starting with string 1, in order.
 *
 * Example:
 *    "abcd"+"aebf" -> "cdef"
 * since "a" and "b" occur in both strings, and the rest is in order.
 */
string residualString(string s1, string s2) {
  string res = "";
  for (char s : s1) {
    if (s2.find(s) == string::npos)
      res += s;
  }
  for (char s : s2) {
    if (s1.find(s) == string::npos)
      res += s;
  }
  return res;
}

}

namespace pichi {

vector<string> split(string s) {

  /* We first split the string into nodes:
   * "0abc1ad2de3bce" --> "0abc", "1ad", "2de", "3bce".
   */

  queue<string> nodes;    // Container for the nodes
  string this_node;       // The current node
  bool digit_last = true; // Currently looking at a multi digit number?

  for (char c : s) {
    if (isdigit(c)) {
      // c is a digit
      if (isdigit(c) && !digit_last) {
        // The last char was an index, so this is a new node.
        // Save the old node and start a new one.
        nodes.push(this_node);
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
    nodes.push(this_node);
  }

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


cdouble compute(std::string s, std::vector<Tensor>& tensors) {
  cdouble result = 0.0;

  // We iterate through all the connected components of the diagram
  for (string comp : split(s)) {

    vector<DiagramNode> nodes; // List of nodes we need for this diagram
    int idx = 0;
    // We keep count of the number of rank 2 and rank 3 tensors
    int n_rank2 = 0;
    int n_rank3 = 0;
    while (idx < comp.size()) {

      // Get the tensor index
      string tensor = "";
      while (isdigit(comp[idx]))
        tensor += comp[idx++];
      int tensor_idx = stoi(tensor);

      // Get the contraction string
      string pattern = "";
      while (!isdigit(comp[idx]) && idx < comp.size())
        pattern += comp[idx++];

      // Create the node
      DiagramNode node;
      node.idx = pattern;
      node.t = &tensors[tensor_idx];
      if (node.t->rank() == 2)
        n_rank2++;
      else
        n_rank3++;
      nodes.push_back(node);

    }

    // Choose the correct diagram based on the ranks of the tensors
    if (n_rank2 == 1 && n_rank3 == 0)
      result += diagram0(nodes);
    else if (n_rank2 == 2 && n_rank3 == 0)
      result += diagram1(nodes);
    else if (n_rank2 == 3)
      result += diagram2(nodes);
    else if (n_rank2 == 4)
      result += diagram3(nodes);
    else if (n_rank2 == 0 && n_rank3 == 2)
      result += diagram4(nodes);
    else if (n_rank2 == 1 && n_rank3 == 2)
      result += diagram5(nodes);
    else if (n_rank2 == 2 && n_rank3 == 2) {
      // There are two possibilities here. Try one and if it doesn't work,
      // it'll be the other...
      try {
        result += diagram6(nodes);
      } catch (invalid_argument& e) {
        result += diagram7(nodes);
      }
    }
    else if (n_rank3 == 4)
      result += diagram8(nodes);


  }
  return result;
}



/* ===============================================================
 *
 * Diagram implementations
 *
 * =============================================================== */

cdouble diagram0(vector<DiagramNode>& nodes) {
  return contract(*nodes[0].t,{{0,1}});
}

cdouble diagram1(vector<DiagramNode>& nodes) {

  return contract(*nodes[0].t,*nodes[1].t,
                  matchChars(nodes[0].idx,nodes[1].idx));

}

cdouble diagram2(vector<DiagramNode>& nodes) {

  // Contraction order is irrelevant for this diagram: all choices of two
  // nodes will be equivalent

  Tensor* t1 = nodes[0].t; string idx1 = nodes[0].idx;
  Tensor* t2 = nodes[1].t; string idx2 = nodes[1].idx;
  Tensor* t3 = nodes[2].t; string idx3 = nodes[2].idx;

  Tensor t4(2,t1->size());
  contract(*t1, *t2, matchChars(idx1,idx2),t4);
  string idx4 = residualString(idx1,idx2);

  return contract(t4, *t3, matchChars(idx4,idx3));

}

cdouble diagram3(vector<DiagramNode>& nodes) {

  // Strategy: Contract two nodes and handle the rest as a diagram2

  vector<DiagramNode> nodes2; // The nodes we will send to diagram2

  Tensor* x1; string xidx1;
  Tensor* x2; string xidx2;

  for (DiagramNode dn : nodes) {
    // We need to find two connected nodes and contract them. The rest is
    // passed on to diagram2
    if (xidx1.empty()) {
      // We always grab the first node
      x1 = dn.t;
      xidx1 = dn.idx;
    }
    else if (xidx2.empty() && !matchChars(xidx1, dn.idx).empty()) {
      // This node is the first we have seen that is connected to the other.
      // Grab it!
      x2 = dn.t;
      xidx2 = dn.idx;
    }
    else {
      // We either a) already have two connected nodes, or b) this node is
      // not connected to the first. Pass it on to diagram2.
      nodes2.push_back(dn);
    }
  }

  // Create a temporary tensor for the contraction.
  Tensor* t5 = new Tensor(2, x1->size());
  contract(*x1, *x2, matchChars(xidx1, xidx2), *t5);
  string idx5 = residualString(xidx1, xidx2);

  // Create a new node for the tensor.
  DiagramNode n5;
  n5.t = t5; n5.idx = idx5;
  nodes2.push_back(n5);

  // Compute the rest of the contraction as diagram2
  cdouble res = diagram2(nodes2);
  delete t5;
  return res;

}


cdouble diagram4(vector<DiagramNode>& nodes) {

  return contract(*nodes[0].t,*nodes[1].t,
                  matchChars(nodes[0].idx,nodes[1].idx));

}


cdouble diagram5(vector<DiagramNode>& nodes) {

  // Strategy: we contract the rank 2 tensor with a rank 3 tensor and compute
  // the rest as an instance of diagram4

  vector<DiagramNode> nodes2; // The nodes to pass to diagram4

  Tensor* x1; string xidx1;
  Tensor* x2; string xidx2;

  bool found_rank3 = false;
  for (DiagramNode dn : nodes) {
    // We need to find the rank 2 tensor and a rank 3 tensor and contract
    // them. The last one is passed on to diagram4
    if (dn.t->rank() == 2) {
      // Found the rank 2 tensor!
      x1 = dn.t;
      xidx1 = dn.idx;
    }
    else if (found_rank3) {
      // The node is the second rank 3 tensor. Pass it on to diagram4
      nodes2.push_back(dn);
    }
    else {
      // The first rank 3 tensor. Grab it!
      x2 = dn.t;
      xidx2 = dn.idx;
      found_rank3 = true;
    }
  }

  // Create a temporary tensor and contract.
  Tensor *t4 = new Tensor(3, x1->size());
  contract(*x1, *x2, matchChars(xidx1,xidx2), *t4);
  string idx4 = residualString(xidx1,xidx2);

  // Create the new node.
  DiagramNode n4;
  n4.t = t4; n4.idx = idx4;
  nodes2.push_back(n4);

  // Compute the rest of the contraction as a diagram4
  cdouble res = diagram4(nodes2);
  delete t4;
  return res;

}

cdouble diagram6(vector<DiagramNode>& nodes) {

  // Strategy: Contract the two rank 2 tensors and compute the rest of the
  // diagram as a diagram5

  vector<DiagramNode> nodes2; // The nodes to pass to diagram5

  Tensor* x1; string xidx1;
  Tensor* x2; string xidx2;

  bool found_one = false;
  for (DiagramNode dn : nodes) {
    // We look for the two rank 2 tensors to contract here. Rank 3 tensors
    // are passed on to diagram5
    if (dn.t->rank() == 2) {
      if (found_one) {
        // The second rank 2 tensor.
        x2 = dn.t;
        xidx2 = dn.idx;
      } else {
        // The first rank 2 tensor
        x1 = dn.t;
        xidx1 = dn.idx;
        found_one = true;
      }
    }
    else {
      // Rank 3 tensor. Pass it on.
      nodes2.push_back(dn);
    }
  }

  // Contract the two rank 2 tensors.
  Tensor* t5 = new Tensor(2,x1->size());
  contract(*x1,*x2, matchChars(xidx1,xidx2), *t5);
  string idx5 = residualString(xidx1,xidx2);

  // Create the new node.
  DiagramNode n5;
  n5.t = t5; n5.idx = idx5;
  nodes2.push_back(n5);

  // Compute the rest as a diagram5.
  cdouble res = diagram5(nodes2);
  delete t5;
  return res;

}

cdouble diagram7(vector<DiagramNode>& nodes) {

  // Strategy: contract a rank 2 and a rank 3 tensor and treat the rest as an
  // instance of diagram5

  Tensor* x1; string xidx1;
  Tensor* x2; string xidx2;

  vector<DiagramNode> nodes2; // The nodes to pass to diagram5

  bool found_rank2 = false;
  bool found_rank3 = false;
  for (DiagramNode dn : nodes) {
    // Find a rank 2 and a rank 3 tensor, and pass the rest on
    if (dn.t->rank() == 2) {
      if (found_rank2) {
        nodes2.push_back(dn);
      }
      else {
        x1 = dn.t;
        xidx1 = dn.idx;
        found_rank2 = true;
      }
    }
    else {
      if (found_rank3) {
        nodes2.push_back(dn);
      }
      else {
        x2 = dn.t;
        xidx2 = dn.idx;
        found_rank3 = true;
      }
    }
  }

  // Contract the two tensors
  Tensor* t5 = new Tensor(3, x1->size());
  contract(*x1,*x2, matchChars(xidx1,xidx2), *t5);
  string idx5 = residualString(xidx1,xidx2);

  // Create the new node
  DiagramNode n5;
  n5.t = t5; n5.idx = idx5;
  nodes2.push_back(n5);

  // Compute the result as diagram5
  cdouble res = diagram5(nodes2);
  delete t5;
  return res;

}

cdouble diagram8(vector<DiagramNode>& nodes) {

  // Strategy: Contract two tensors connected by two indices and compute the
  // rest as a diagram5

  vector<DiagramNode> nodes2;

  // We always use the first node. We need to find the node connected to the
  // first one by two indices
  Tensor* x; string xidx;

  for (int i = 1; i < 4; ++i) {
    if (matchChars(nodes[0].idx, nodes[i].idx).size() == 2) {
      x = nodes[i].t;
      xidx = nodes[i].idx;
    }
    else {
      nodes2.push_back(nodes[i]);
    }
  }

  // Contract the two tensors
  Tensor* t5 = new Tensor(2, x->size());
  contract(*nodes[0].t, *x, matchChars(nodes[0].idx, xidx), *t5);
  string idx5 = residualString(nodes[0].idx, xidx);

  // Create the new node
  DiagramNode n5;
  n5.t = t5; n5.idx = idx5;
  nodes2.push_back(n5);

  // Compute the result as diagram5
  cdouble res = diagram5(nodes2);
  delete t5;
  return res;

}





}

