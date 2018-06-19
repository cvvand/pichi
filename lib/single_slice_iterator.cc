
#include <stdexcept>
#include <unordered_set>
#include "tensor.h"
#include "slice_iterator.h"

using namespace std;

namespace pichi {

SingleSliceIterator::SingleSliceIterator(
    const Tensor& tensor, const vector<std::pair<int, int>>& contractions)
     : size(tensor.getSize()) {

  int rank1 = tensor.getRank();

  // Check rank and size
  if (rank1 < 2) {
    throw invalid_argument("Tensor must have at least rank 2");
  }
  if (size < 2) {
    throw invalid_argument("Tensor must have at least size 2");
  }

  // Check contractions input
  if (contractions.empty()) {
    throw invalid_argument("Contraction list is empty");
  }
  unordered_set<int> seen;
  for (pair<int,int> p : contractions) {
    if (p.first < 0 || p.first >= rank1 || p.second < 0 || p.second >= rank1)
      throw invalid_argument("Invalid index in contraction list");
    if (!(seen.insert(p.first).second && seen.insert(p.second).second))
      throw invalid_argument("Repeated index in contraction list");
  }

  // Set up data structures
  slice1 = vector<int>(rank1, 0);
  slice_out = vector<int>(rank1-2*contractions.size(), 0);

  // We slice input tensors along the first contracted index (SC)
  slice1[contractions[0].first] = -1;
  slice1[contractions[0].second] = -1;

  // The rest of the contracted indices are put in the NC category
  for (int i = 1; i < contractions.size(); ++i)
    nc.push_back(contractions[i]);

  // Search for free indices on tensor 1
  int out_index = 0; // Keep track of the index number on the output tensor
  for (int i = 0; i < rank1; ++i) {
    bool free = true;
    for (int j = 0; j < contractions.size() && free; ++j) {
      // Figure out if index i is contracted. If not, it is free
      if (contractions[j].first == i || contractions[j].second == i)
        free = false;
    }
    if (free && out_index < 2) {
      // If this is the first or second free index on tensor 1, we slice the
      // output along this index (SF).
      sf.push_back(i);
      slice_out[out_index++] = -1;
    } else if (free) {
      // If the index is free, but not the first free index, it belongs in
      // the NF category.
      nf.emplace_back(i, out_index++);
    }
  }



}

vector<int> SingleSliceIterator::getSlice1() const {
  return slice1;
}

vector<int> SingleSliceIterator::getSliceOut() const {
  return slice_out;
}

bool SingleSliceIterator::nextContracted() {
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
      slice1[nc[i].second] = 0;
      // Now continue with i+1'th NC index
    } else {
      // No roll over, so increase the corresponding NC index on slice 2...
      ++slice1[nc[i].second];
      // ... and escape the loop
      flag = false;
    }
  }
  // flag is true if all NC indices rolled over, false if not.
  return !flag;
}

bool SingleSliceIterator::nextNonSlicedFree() {
  if (nf.empty())
    return false;

  // We use a flag to indicate when we reached an NF index which did not roll
  // over
  bool flag = true;
  for (int i = 0; i < nf.size() && flag; ++i) {
    // Increase the i'th NF index on tensor 1
    int z = ++slice1[nf[i].first];
    if (z == size) {
      // This NF index needs to roll over
      slice1[nf[i].first] = 0;
      slice_out[nf[i].second] = 0;
    } else {
      // No roll over, so increase the corresponding index on the output
      // slice...
      ++slice_out[nf[i].second];
      // .. and escape the loop
      flag = false;
    }
  }
  return !flag;
}

bool SingleSliceIterator::nextSlicedFree() {
  if (sf.empty())
    return false;

  // At this point, there are either 0 SF indices (when there is no output
  // tensor), or there is 2. Therefore, we assume the length of sf1
  // is equal to either 0 or 2.

  // If we got to this point, the length is 2

  // Increase the first SF index on slice 1
  int z = ++slice1[sf[0]];
  if (z == size) {
    // The first SF index on slice 1 needs to roll...
    slice1[sf[0]] = 0;
    // .. so we increase then second SF index instead
    z = ++slice1[sf[1]];
    if (z == size) {
      // This index also needs to roll
      slice1[sf[1]] = 0;
      return false;
    }
  }
  return true;
}

}
