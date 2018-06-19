#include <stdexcept>
#include <unordered_set>
#include <queue>
#include <iostream>
#include "slice_iterator.h"

using namespace std;

namespace pichi {


DoubleSliceIterator::DoubleSliceIterator(
    const Tensor& t1, const Tensor& t2,
    const std::vector<std::pair<int, int>>& contr) {

  int rank1 = t1.getRank();
  int rank2 = t2.getRank();
  size = t1.getSize();

  // Check rank and size
  if (rank1 < 2 || rank2 < 2) {
    throw invalid_argument("Tensor must have at least rank 2");
  }
  if (size < 2) {
    throw invalid_argument("Tensor must have at least size 2");
  }

  // Check contractions input
  if (contr.empty()) {
    throw invalid_argument("Contraction list is empty");
  }
  unordered_set<int> seen1;
  unordered_set<int> seen2;
  for (pair<int,int> p : contr) {
    if (p.first < 0 || p.first >= rank1 || p.second < 0 || p.second >= rank2)
      throw invalid_argument("Invalid index in contraction list");
    if (!(seen1.insert(p.first).second && seen2.insert(p.second).second))
      throw invalid_argument("Repeated index in contraction list");
  }

  vector<int> store1 = t1.getStorage();
  vector<int> store2 = t2.getStorage();

  // Set up data structures

  slice1 = vector<int>(rank1, 0);
  slice2 = vector<int>(rank2, 0);
  slice_out = vector<int>(rank1+rank2-2*contr.size(), 0);

  if (contr.size() == 1) {

    // There is one contracted index:

    // Two cases:
    // 1) The contracted index is in a leading dimension.
    //    SC: the contracted index
    //    NC: none
    //    SF: the index in the remaining leading dimension
    //    NF: the rest
    // 2) The contracted index is not in a leading dimension.
    //    SC: the contracted index
    //    NC: none
    //    SF: the index in the leading dimension
    //    NF: the rest

    pair<int,int> c = contr[0];

    int out_index = 0;
    // Start with tensor 1
    for (int i = 0; i < rank1; ++i) {
      if (i == c.first) {
        // This is the contracted index: SC
        slice1[i] = -1;
      }
      else if (i == store1[0]) {
        // Not contracted, but leading: SF
        slice1[i] = -2;
        slice_out[out_index++] = -1;
      }
      else if (i == store1[1]) {
        // Not contracted, but sub-leading: SF or NF
        if (c.first == store1[0]) {
          // SF
          slice1[i] = -2;
          slice_out[out_index++] = -1;
        }
        else {
          // NF
          nf1.push_back(make_pair(i,out_index++));
        }
      }
      else {
        // Not contracted, not a leading dimension: NF
        nf1.push_back(make_pair(i,out_index++));
      }
    }
    // ... then tensor 2
    for (int i = 0; i < rank2; ++i) {
      if (i == c.second) {
        // This is the contracted index: SC
        slice2[i] = -1;
      }
      else if (i == store2[0]) {
        // Not contracted, but leading: SF
        slice2[i] = -2;
        slice_out[out_index++] = -1;
      }
      else if (i == store2[1]) {
        // Not contracted, but sub-leading: SF or NF
        if (c.second == store2[0]) {
          // SF
          slice2[i] = -2;
          slice_out[out_index++] = -1;
        }
        else {
          // NF
          nf2.push_back(make_pair(i,out_index++));
        }
      }
      else {
        // Not contracted, not a leading dimension: NF
        nf2.push_back(make_pair(i,out_index++));
      }
    }


  }
  else {

    // There are two or more contracted indices:

    // We slice along two of the contracted indices. We go through the list
    // of contracted indices repeatedly until we have two indices to slice.
    // First pass: take any index in a leading dimension on both tensors
    // Second pass: take any index in a leading dimension on one tensor.
    // Last pass: take any contracted index until we have two.

    // After having selected the SC indices, all the rest of the contracted
    // indices are NC indices

    // We now choose the SF indices. If the result is not a scalar, then we
    // need two SF indices. Otherwise we need none.
    // We start adding any index which is a leading dimension index but is
    // not contracted. If we are still missing one or two SF indices, we
    // simply pick the first two free indices available.

    // The rest of the free indices are NF indices.

    // We keep a list of whether the indices are free or contracted
    vector<bool> free1(rank1, true);
    vector<bool> free2(rank2, true);

    // First we insert all the contractions into a queue
    queue<pair<int,int>> contr_cp;
    for (pair<int,int> c : contr) {
      free1[c.first] = false;
      free2[c.second] = false;
      contr_cp.push(c);
    }

    // Pass through list of contracted indices until we have two SC
    int pass = 0; int found = 0;
    while (found < 2) {
      int pass_length = contr_cp.size();
      for (int i = 0; (i < pass_length && found < 2); ++i) {
        pair<int,int> c = contr_cp.front();
        contr_cp.pop();
        if (pass == 0 &&
            ((c.first  == store1[0] || c.first  == store1[1]) &&
             (c.second == store2[0] || c.second == store2[1]))) {
          // First pass and index is leading on both tensors
          slice1[c.first]  = -(++found);
          slice2[c.second] = -found;
        }
        else if (pass == 1 &&
                 (c.first  == store1[0] || c.first  == store1[1] ||
                  c.second == store2[0] || c.second == store2[1])) {
          // Second pass and index is leading on one tensor
          slice1[c.first]  = -(++found);
          slice2[c.second] = -found;
        }
        else if (pass == 2) {
          // Third pass: take the index no matter what
          slice1[c.first]  = -(++found);
          slice2[c.second] = -found;
        }
        else {
          // The contracted index is not a good slicing index. Back in line.
          contr_cp.push(c);
        }
      }
      ++pass;
    }

    // All indices still in the queue go in the NC category
    while (!contr_cp.empty()) {
      nc.push_back(contr_cp.front());
      contr_cp.pop();
    }

    // If the two tensors are completely contracted, we stop here
    if (slice_out.size() == 0)
      return;

    // There are free indices. Put them into a queue
    queue<pair<int,int>> free1q;
    queue<pair<int,int>> free2q;

    int out_index = 0;
    for (int i = 0; i < rank1; ++i) {
      if (free1[i])
        free1q.push(make_pair(i,out_index++));
    }
    for (int i = 0; i < rank2; ++i) {
      if (free2[i])
        free2q.push(make_pair(i,out_index++));
    }

    // Pass through the free indices until we have two SF
    pass = 0; found = 0;
    while (found < 2) {
      int pass_length = free1q.size();
      for (int i = 0; (i < pass_length && found < 2); ++i) {
        pair<int,int> f = free1q.front();
        free1q.pop();
        if (pass == 0 &&
            (f.first == store1[0] || f.first == store1[1])) {
          // First pass and index is leading on tensor 1
          sf1.push_back(f.first);
          slice_out[f.second] = -1;
          ++found;
        }
        else if (pass == 1) {
          // Second pass: take the index no matter what
          sf1.push_back(f.first);
          slice_out[f.second] = -1;
          ++found;
        }
        else {
          // The index is not a good slicing index. Back in line.
          free1q.push(f);
        }
      }
      pass_length = free2q.size();
      for (int i = 0; (i < pass_length && found < 2); ++i) {
        pair<int,int> f = free2q.front();
        free2q.pop();
        if (pass == 0 &&
            (f.first == store2[0] || f.first == store2[1])) {
          // First pass and index is leading on tensor 1
          sf2.push_back(f.first);
          slice_out[f.second] = -1;
          ++found;
        }
        else if (pass == 1) {
          // Second pass: take the index no matter what
          sf2.push_back(f.first);
          slice_out[f.second] = -1;
          ++found;
        }
        else {
          // The index is not a good slicing index. Back in line.
          free2q.push(f);
        }
      }
      ++pass;
    }

    // All the rest of the free indices go in the NF category
    while (!free1q.empty()) {
      nf1.push_back(free1q.front());
      free1q.pop();
    }
    while (!free2q.empty()) {
      nf2.push_back(free2q.front());
      free2q.pop();
    }



  }

}

std::vector<int> DoubleSliceIterator::getSlice1() const {
  return slice1;
}

std::vector<int> DoubleSliceIterator::getSlice2() const {
  return slice2;
}

std::vector<int> DoubleSliceIterator::getSliceOut() const {
  return slice_out;
}


bool DoubleSliceIterator::nextContracted() {
  if (nc.empty())
    return false;

  // We use a flag to indicate when we hit an NC index, which did not roll over
  bool flag = true;
  for (int i = 0; i < nc.size() && flag; ++i) {
    // Increase the i'th NC index on slice 1
    int z = ++slice1[nc[i].first];
    if (z == size) {
      // This NC index needs to roll over
      slice1[nc[i].first] = 0;
      slice2[nc[i].second] = 0;
      // Now continue with i+1'th NC index
    } else {
      // No roll over, so increase the corresponding NC index on slice 2...
      ++slice2[nc[i].second];
      // ... and escape the loop
      flag = false;
    }
  }
  // flag is true if all NC indices rolled over, false if not.
  return !flag;
}

bool DoubleSliceIterator::nextNonSlicedFree() {
  if (nf1.empty() && nf2.empty())
    return false;

  // We use a flag to indicate when we reached an NF index which did not roll
  // over
  bool flag = true;
  for (int i = 0; i < nf1.size() && flag; ++i) {
    // Increase the i'th NF index on tensor 1
    int z = ++slice1[nf1[i].first];
    if (z == size) {
      // This NF index needs to roll over
      slice1[nf1[i].first] = 0;
      slice_out[nf1[i].second] = 0;
    } else {
      // No roll over, so increase the corresponding index on the output
      // slice...
      ++slice_out[nf1[i].second];
      // .. and escape the loop
      flag = false;
    }
  }

  // If we found an NF index on slice 1 that didn't roll, then we stop here...
  if (!flag)
    return true;

  // ... otherwise we start on slice 2 instead
  for (int i = 0; i < nf2.size() && flag; ++i) {
    int z = ++slice2[nf2[i].first];
    if (z == size) {
      slice2[nf2[i].first] = 0;
      slice_out[nf2[i].second] = 0;
    } else {
      ++slice_out[nf2[i].second];
      flag = false;
    }
  }

  // flag is false if we found a non-rolling NF index, otherwise it is true
  return !flag;
}

bool DoubleSliceIterator::nextSlicedFree() {

  bool flag = true;

  for (int i = 0; i < sf1.size() && flag; ++i) {
    int z = ++slice1[sf1[i]];
    if (size == z) {
      slice1[sf1[i]] = 0;
    } else
      flag = false;
  }
  for (int i = 0; i < sf2.size() && flag; ++i) {
    int z = ++slice2[sf2[i]];
    if (size == z) {
      slice2[sf2[i]] = 0;
    } else
      flag = false;
  }

  return !flag;
}


}

