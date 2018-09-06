
/* ****************************************************************************
 *
 * Implementation of the Contraction class defined in CONTRACTION.H
 *
 * ***************************************************************************/

#include "pichi/contraction.h"
#include "slice_iterator.h"
#include "pichi/graph.h"
#include "diagrams.h"
#include <armadillo>
#include <unordered_set>
//#include <cblas.h> If being used with blas

using namespace std;
using namespace arma;

namespace pichi {

pair<bool,bool> detectTranspose(const std::vector<int>& slice1,
                                const std::vector<int>& slice2) {

  /*
   * This function detects whether a slice of a tensor needs to be transposed
   * before matrix matrix multiplication. In its essence, matrix matrix
   * multiplication looks like
   * A_ab B_bc
   * The slices in this case looks like
   * A: (-2,-1)
   * B: (-1,-2)
   *
   * For a general tensor contraction, we could have the following scenario
   * A_ab B_cb
   * A: (-2,-1)
   * B: (-2,-1)
   *
   * Naively multiplying the two slices together would give the wrong result.
   * We first need to transpose the B slice.
   *
   * This function detects the relative positions of the -2 and -1  on the
   * two slices, and figures out which slices need to be transposed in order
   * to get to the correct pattern: (-2,-1) , (-1,-2).
   * The return value is a pair of booleans. The first boolean corresponds to
   * the first tensor. If the value is true, the tensor should be transposed.
   * If false, it should not. The same is true of the second tensor with the
   * secodn boolean in the returned pair.
   */

  int i11,i12,i21,i22;
  // iXY is the location of the -X on slice Y

  for (int i = 0; i < slice1.size(); ++i) {
    if (slice1[i] == -1)
      i11 = i; // -1 is at position i in tensor 1
    if (slice1[i] == -2)
      i21 = i; // -2 is at position i in tensor 1
  }
  for (int i = 0; i < slice2.size(); ++i) {
    if (slice2[i] == -1)
      i12 = i; // -1 is at position i in tensor 2
    if (slice2[i] == -2)
      i22 = i; // -2 is at position i in tensor 2
  }
  // The correct pattern is (-2 -1) , (-1 -2)    i21 < i11 && i12 < i22
  pair<bool,bool> res;
  if (i11 > i21) {
    // No transpose on tensor 1
    res.first = false;
    if (i22 > i12) {
      // No transpose on tensor 2
      res.second = false;
    }
    else
      res.second = true;
  } else {
    // Transpose on tensor 1
    res.first = true;
    if (i22 > i12)
      res.second = false;
    else
      res.second = true;
  }
  return res;
}

void setStorage(Tensor& tensor, const std::vector<int> slicing) {

  vector<int> storage = tensor.getStorage(); // The current storage
  int count = 0; // The currently found number of sliced indices
  for (int i = 0; count < 2; ++i) { // Find the 2 sliced indices
    // The index is sliced if the slice value is negative.
    if (slicing[i] < 0) {
      // The storage vector should have "i" at the current count.
      if (storage[count] != i) {
        // The storage vector has another value where it should be i, so we
        // find the current position of the i
        int current_pos = 0;
        while (storage[current_pos] != i)
          ++current_pos;
        // Now swap the i into the right position, putting the current value
        // where the i is now.
        int temp = storage[count];
        storage[count] = i;
        storage[current_pos] = temp;
      }
      ++count;
    }
  }
  tensor.setStorage(storage);

}

Tensor contract(Tensor& tensor, const std::vector<std::pair<int,int>>& idx) {

  if (idx.empty()) // No contractions: return input tensor unmodified.
    return tensor;

  // Find the output rank and size;
  int rank = tensor.getRank() - 2*idx.size();
  int size = tensor.getSize();

  // Check that no index appears twice and that they are valid indices
  unordered_set<int> seen;
  for (pair<int,int> p : idx) {
    if (p.first < 0 || p.first >= tensor.getRank() ||
        p.second < 0 || p.second >= tensor.getRank())
      throw invalid_argument("Indices must be between 0 and R-1, where R is "
                             "the tensor rank");
    if (!(seen.insert(p.first).second && seen.insert(p.second).second))
      throw invalid_argument("List of contracted indices "
                             "contains an index twice");
  }

  // Set up slice iterator
  SingleSliceIterator it(tensor, idx);

  // Set storage for input tensor
  setStorage(tensor, it.getSlice1());

  // Create output tensor with correct storage
  vector<int> storage_out(rank, 0);
  int count1 = 0;
  int count2 = 2;
  for (int i = 0; i < rank; ++i) {
    if (it.getSliceOut()[i] < 0)
      storage_out[count1++] = i;
    else
      storage_out[count2++] = i;
  }
  Tensor tout(rank, size, storage_out);

  cdouble data[tensor.getSize()*tensor.getSize()];
  cdouble data_out[tout.getSize() * tout.getSize()];


  do { // Loop over non-sliced free indices on the output tensor

    // x is the current index in data_out, which resets when we change
    // slice on the output tensor.
    int x = 0;

    do { // Loop through free indices sliced on the output tensor
      data_out[x] = 0;

      do { // Loop through contracted, non-sliced indices on input tensors

        // Get the current slices in matrix form
        tensor.getSlice(it.getSlice1(), data);

        data_out[x] += trace(cx_mat(data, tensor.getSize(), tensor.getSize()));

        // Increase the contracted non-sliced indices on input tensors
      } while (it.nextContracted());

      ++x;

      // Increase free indices, sliced on the output tensor
    } while (it.nextSlicedFree());

    // Set the current slice of the output tensor
    tout.setSlice(it.getSliceOut(), data_out);

  } while (it.nextNonSlicedFree());

  return tout;
}


Tensor contract(Tensor& t1, Tensor& t2,
                const std::vector<std::pair<int, int>>& idx) {

  // Check that there are contracted indices
  if (idx.empty())
    throw invalid_argument("List of contracted indices is empty");

  // Check that all indices are valid
  unordered_set<int> seen1;
  unordered_set<int> seen2;
  for (pair<int,int> p : idx) {
    if (p.first < 0 || p.first >= t1.getRank() ||
        p.second < 0 || p.second >= t2.getRank())
      throw invalid_argument("Indices must be between 0 and R-1, where R is "
                             "the tensor rank");
    if (!(seen1.insert(p.first).second && seen2.insert(p.second).second))
      throw invalid_argument("List of contracted indices "
                             "contains an index twice");
  }

  // Compute output tensor rank and size
  int rank1 = t1.getRank();
  int rank2 = t2.getRank();
  int nc = idx.size();
  int rank = rank1 + rank2 - 2*nc;
  int size = t1.getSize();

  // Set up the iterator
  DoubleSliceIterator it(t1, t2, idx);

  // We change the input data storage if it is beneficial based on a heuristic:
  // Let:
  //   r1 = rank(t1)
  //   r2 = rank(t2)
  //   rm = max(r1,r2)
  //   c = # of contracted indices
  //   m = r1 + r2 - 4          if  c = 1 ,
  //       r1 + r2 - 4 - (c-2)  if  c > 1
  // We reorder the storage if   rm - m < 2
  int rm = (rank1 > rank2 ? rank1 : rank2);
  int m = (nc == 1 ? rank1+rank2-4 : rank1+rank2-4-(nc-2));
  if (rm - m < 2) {
    setStorage(t1, it.getSlice1());
    setStorage(t2, it.getSlice2());
  }

  // Create output tensor and set storage
  vector<int> storage_out(rank, 0);
  int count1 = 0;
  int count2 = 2;
  for (int i = 0; i < rank; ++i) {
    if (it.getSliceOut()[i] < 0)
      storage_out[count1++] = i;
    else
      storage_out[count2++] = i;
  }
  Tensor tout(rank, size, storage_out);

  // Detect whether transposition is needed (sliced indices will not change
  // during iteration)
  auto trans = detectTranspose(it.getSlice1(),it.getSlice2());

  // Containers for the data for matrix multiplication
  cdouble data1[t1.getSize()*t1.getSize()];
  cdouble data2[t2.getSize()*t2.getSize()];
  cdouble data_out[tout.getSize() * tout.getSize()];

  do { // Loop through free indices, not sliced on the output tensor

    if (idx.size() >= 2) {
      // Mult->Trace branch

      // x is the current index in data_out, which resets when we change
      // slice on the output tensor.
      int x = 0;

      do { // Loop through free indices sliced on the output tensor
        data_out[x] = 0;

        do { // Loop through contracted, non-sliced indices on input tensors

          // Get the current slices in matrix form
          bool s1 = t1.getSlice(it.getSlice1(), data1);
          bool s2 = t2.getSlice(it.getSlice2(), data2);

          // Check whether matrices should be transposed or not before
          // multiplication
          bool trans1 = ((trans.first && !s1) || (!trans.first && s1));
          bool trans2 = ((trans.second && !s2) || (!trans.second && s2));

          cx_mat m1(data1, t1.getSize(), t1.getSize());
          cx_mat m2(data2, t2.getSize(), t2.getSize());

          if ((trans1 && trans2) || (!trans1 && !trans2))
            data_out[x] += trace(m1 * m2);
          else
            data_out[x] += trace(m1.st() * m2);

          /* --------- DIRECT BLAS -------
          int sz = t1.getSize();
          double alpha = 1.0; double beta = 0.0;
          cdouble res_data[sz*sz];

          if (transpose_type == 0 || transpose_type == 3) {
            cblas_zgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, sz, sz,
                        sz, &alpha,
                        reinterpret_cast<double *>(data1), sz,
                        reinterpret_cast<double *>(data2), sz, &beta,
                        reinterpret_cast<double *>(res_data), sz);
          }
          else {
            cblas_zgemm(CblasColMajor, CblasTrans, CblasNoTrans, sz, sz,
                        sz, &alpha,
                        reinterpret_cast<double *>(data1), sz,
                        reinterpret_cast<double *>(data2), sz, &beta,
                        reinterpret_cast<double *>(res_data), sz);
          }
          for (int i = 0; i < sz; ++i)
            data_out[x] += res_data[i+i*sz];*/

          // Increase the contracted non-sliced indices on input tensors
        } while (it.nextContracted());

        ++x;

        // Increase free indices, sliced on the output tensor
      } while (it.nextSlicedFree());

      // Set the current slice of the output tensor
      tout.setSlice(it.getSliceOut(), data_out);

    } else {

      // Mult branch

      // Get the current slices in matrix form
      bool s1 = t1.getSlice(it.getSlice1(), data1);
      bool s2 = t2.getSlice(it.getSlice2(), data2);

      // Check whether matrices should be transposed or not before
      // multiplication
      bool trans1 = ((trans.first && !s1) || (!trans.first && s1));
      bool trans2 = ((trans.second && !s2) || (!trans.second && s2));

      cx_mat m1(data1, t1.getSize(), t1.getSize());
      cx_mat m2(data2, t2.getSize(), t2.getSize());

      cx_mat mout; // The output slice in matrix form.
      // Do the multiplication based on transpose type
      if (!trans1 && !trans2)
        mout = m1 * m2;
      else if (trans1 && !trans2)
        mout = m1.st() * m2;
      else if (!trans1 && trans2)
        mout = m1 * m2.st();
      else
        mout = strans(m2 * m1);

      // Set the slice on the output tensor
      tout.setSlice(it.getSliceOut(), mout.memptr());


      /* --------- DIRECT BLAS -------
      int sz = t1.getSize();
      double alpha = 1.0; double beta = 0.0;
      cdouble res_data[sz*sz];
      CBLAS_TRANSPOSE trans1;
      if (transpose_type == 1)
        trans1 = CblasTrans;
      else
        trans1 = CblasNoTrans;
      CBLAS_TRANSPOSE trans2;
      if (transpose_type == 2)
        trans2 = CblasTrans;
      else
        trans2 = CblasNoTrans;

      if (transpose_type == 3) {
        cblas_zgemm(CblasRowMajor, CblasTrans, CblasTrans, sz, sz, sz, &alpha,
                    reinterpret_cast<double *>(data2), sz,
                    reinterpret_cast<double *>(data1), sz, &beta,
                    reinterpret_cast<double *>(res_data), sz);
      }
      else {
        cblas_zgemm(CblasColMajor, trans1, trans2, sz, sz, sz, &alpha,
                    reinterpret_cast<double *>(data1), sz,
                    reinterpret_cast<double *>(data2), sz, &beta,
                    reinterpret_cast<double *>(res_data), sz);
      }

      // Set the slice on the output tensor
      tout.setSlice(it.getSliceOut(), res_data);*/

    }

    // Increase the free indices not sliced on the output tensor
  } while (it.nextNonSlicedFree());

  return tout;

}


Tensor contract(const Graph& graph, std::vector<Tensor>& tensors) {

  Graph red(graph); // Working copy of the graph

  vector<Tensor> temps; // Storage of temporary tensors
  int idx = tensors.size(); // Keeps track of how many tensors we have.

  // We are done when the reduced graph has one node with no connections
  while (!( (red.getNodes().size()==1) &&
            (red.connections(*red.getNodes().begin()).empty()) )) {

    // Extract an optimal subdiagram
    int diag = identifyDiagram(red);
    if (diag == -1) { // Unknown diagram
      throw invalid_argument("Unknown diagram: " + red.toString());
    }
    Graph ext = extract(red, identifyDiagram(red));

    // Do the contractions based on the extracted diagram
    Tensor tout;
    vector<pair<int,int>> contractions;
    if (ext.getNodes().size() == 1) {
      // Extracted diagram has one node

      int tidx = *ext.getNodes().begin();
      for (auto c : ext.allConnections()) {
        contractions.push_back(make_pair(c.first.second,c.second.second));
      }
      tout = contract(tensors[tidx], contractions); // Tetraquark ERROR

    }
    else {
      // Extracted diagram has two nodes

      auto it = ext.getNodes().begin();
      Tensor* t1;
      if (*it < tensors.size())
        t1 = &tensors[*it];
      else
        t1 = &temps[*it - tensors.size()];

      it++;
      Tensor* t2;
      if (*it < tensors.size())
        t2 = &tensors[*it];
      else
        t2 = &temps[*it - tensors.size()];

      for (auto c : ext.allConnections()) {
        contractions.push_back(make_pair(c.first.second,c.second.second));
      }
      tout = contract(*t1,*t2, contractions);

    }

    // Put the newly calculated tensor into the temporary array
    temps.push_back(tout);
    // Reduce the graph
    red.reduce(ext, idx++);

  }
  return temps[idx-tensors.size()-1];
}


}
