#include <stdexcept>
#include <unordered_set>
#include <iostream>
#include "slice_iterator.h"

using namespace std;

namespace pichi {


DoubleSliceIterator::DoubleSliceIterator(int rank1, int rank2, int size,
                                         const std::vector<
                                             std::pair<int, int>>& contr) :
    size(size) {

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

  // Set up data structures

  slice1 = vector<int>(rank1, 0);
  slice2 = vector<int>(rank2, 0);
  slice_out = vector<int>(rank1+rank2-2*contr.size(), 0);

  // We slice input tensors along the first contracted index (SC)
  slice1[contr[0].first] = -1;
  slice2[contr[0].second] = -1;

  // If there are two or more contracted indices, we slice along the second
  // contracted index as well.
  if (contr.size() >= 2) {
    slice1[contr[1].first] = -2;
    slice2[contr[1].second] = -2;
  }

  // The rest of the contracted indices are put in the NC category
  for (int i = 2; i < contr.size(); ++i)
    nc.push_back(contr[i]);

  // Find the number of free indices on each input tensor
  int nfree_1 = rank1 - contr.size();
  int nfree_2 = rank2 - contr.size();

  // Search for free indices on tensor 1
  int out_index = 0; // Keep track of the index number on the output tensor
  bool first_free = true;
  for (int i = 0; i < rank1; ++i) {
    bool free = true;
    for (int j = 0; j < contr.size() && free; ++j) {
      // Figure out if index i is contracted. If not, it is free
      if (contr[j].first == i)
        free = false;
    }
    if (free && first_free) {
      // If this is the first free index on tensor 1, we slice the output
      // along this index (SF). If there is only one contracted index, we
      // instead slice the first input tensor along this index
      if (contr.size() >= 2)
        sf1.push_back(i);
      else
        slice1[i] = -2;
      slice_out[out_index++] = -1;
      first_free = false;
    } else if (free && nfree_2 == 0 && sf1.size() == 1) {
      // The index is free, and there are no free indices on tensor 2. sf1
      // has size 1, which means we only have 1 free index so far.
      // Therefore, we also make this an SF index
      sf1.push_back(i);
      slice_out[out_index++] = -1;
    } else if (free) {
      // If the index is free, but not the first free index, it belongs in
      // the NF category.
      nf1.emplace_back(i, out_index++);
    }
  }

  // Search for free indices on tensor 2
  first_free = true;
  for (int i = 0; i < rank2; ++i) {
    bool free = true;
    for (int j = 0; j < contr.size() && free; ++j) {
      if (contr[j].second == i)
        free = false;
    }
    if (free && first_free) {
      if (contr.size() >= 2)
        sf2.push_back(i);
      else
        slice2[i] = -2;
      slice_out[out_index++] = -1;
      first_free = false;
    } else if (free && nfree_1 == 0 && sf2.size() == 1) {
      sf2.push_back(i);
      slice_out[out_index++] = -1;
    } else if (free) {
      nf2.emplace_back(i,out_index++);
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

