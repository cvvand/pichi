
/* ****************************************************************************
 *
 * Implementation of the Contraction class defined in CONTRACTION.H
 *
 * ***************************************************************************/


#include "contraction.h"
#include "slice_iterator.h"
#include <armadillo>

using namespace std;
using namespace arma;

namespace pichi {

template <class Key>
void Contraction<Key>::addTensor(Key name, Tensor& tensor) {
  tensors.insert({name,tensor});
}

template <class Key>
Tensor& Contraction<Key>::getTensor(Key tensor) {
  return tensors.at(tensor);
}

template <class Key>
void Contraction<Key>::removeTensor(Key tensor) {
  tensors.erase('A');
}

template <class Key>
int Contraction<Key>::detectTranspose(const std::vector<int>& slice1,
                                      const std::vector<int>& slice2) const {

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
   * The return values are:
   * 0 - no transposition needed (A_ab B_bc)
   * 1 - slice 1 should be transposed (A_ba B_bc)
   * 2 - slice 2 should be transposed (A_ab B_cb)
   * 3 - both slices should be transposed (A_ba B_cb)
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
  if (i11 > i21) {
    // No transpose on tensor 1
    if (i22 > i12) {
      // No transpose on tensor 2
      return 0;
    }
    else
      return 2;
  } else {
    // Transpose on tensor 1
    if (i22 > i12)
      return 1;
    else
      return 3; // Both need transpose
  }
}

template <class Key>
cdouble Contraction<Key>::contract(Key tensor,
                                   const std::vector<
                                       std::pair<int,int>>& idx) const {


  Tensor t = tensors.at(tensor);
  cdouble data[t.size()*t.size()];
  cdouble res = 0.0;

  // Set up slice iterator
  SingleSliceIterator it(t.rank(), t.size(), idx);

  do {

    t.getSlice(it.getSlice1(), data);
    res += trace(cx_mat(data, t.size(), t.size()));

  } while (it.nextContracted());

  return res;
}

template <class Key>
cdouble Contraction<Key>::contract(Key tensor1, Key tensor2,
                                   const std::vector<
                                       std::pair<int, int>>& idx) const {

  /*
   * Contractions on the form
   *   A_abcd B_dcba
   * which we compute as a sum of traces:
   *   tr(A_**00 B_00**) + tr(A_**10 B_01**) + tr(A_**20 B_02**) ...
   *     + tr(A_**01 B_10**) + tr(A_**11 B_11**) + ...
   */

  // Get the tensors in question
  Tensor t1 = tensors.at(tensor1);
  Tensor t2 = tensors.at(tensor2);

  // Set up the iterator
  DoubleSliceIterator it(t1.rank(), t2.rank(), t1.size(), idx);

  // Detect whether transposition is needed (sliced indices will not change
  // during iteration)
  int transpose_type = detectTranspose(it.getSlice1(),it.getSlice2());

  // Containers for the data for matrix multiplication
  cdouble data1[t1.size()*t1.size()];
  cdouble data2[t2.size()*t2.size()];

  cdouble result = 0.0;

  // Loop over the non-sliced contracted indices
  do {

    // Get the current slice in matrix form
    t1.getSlice(it.getSlice1(), data1);
    t2.getSlice(it.getSlice2(), data2);
    cx_mat m1(data1, t1.size(), t1.size());
    cx_mat m2(data2, t2.size(), t2.size());

    if (transpose_type == 0 || transpose_type == 3) {
      // The two types are degenerate from this property of the trace:
      //  Tr(A^T B^T) = Tr((BA)^T) = Tr(BA) = Tr(AB)
      result += trace(m1 * m2);
    }
    else {
      // The two types are degenerate from this trace property
      // Tr(AB^T) = Tr((BA^T)^T) = Tr(BA^T) = Tr(A^T B)
      result += trace(m1 * m2.st());
    }

    // Increase the contracted indices on the two input slices
  } while (it.nextContracted());

  return result;

}

template <class Key>
void Contraction<Key>::contract(Key tensor1,
                                const std::vector<std::pair<int, int>>& idx,
                                Key tensor_out) {

  /*
   * Contractions on the form
   *   A_aabbcde = B_cde
   * which we compute as a sum of traces on each output slice:
   *   B_012 = tr(A_**00012) + tr(A_**11012) + ...
   */


  // Get the tensors in question
  Tensor t1 = tensors.at(tensor1);

  // Get the number of free indices and create the output tensor
  int free_out = t1.rank() - 2*idx.size();
  Tensor t_out(free_out, t1.size());

  // Set up the iterator
  SingleSliceIterator it(t1.rank(), t1.size(), idx);

  // Make ready arrays for matrix multiplication
  cdouble data1[t1.size() * t1.size()];
  cdouble data_out[t_out.size() * t_out.size()];

  do { // Loop over non-sliced free indices on the output tensor

    // x is the current index in data_out, which resets when we change
    // slice on the output tensor.
    int x = 0;

    do { // Loop through free indices sliced on the output tensor
      data_out[x] = 0;

      do { // Loop through contracted, non-sliced indices on input tensors

        // Get the current slices in matrix form
        t1.getSlice(it.getSlice1(), data1);

        data_out[x] += trace(cx_mat(data1, t1.size(), t1.size()));

        // Increase the contracted non-sliced indices on input tensors
      } while (it.nextContracted());

      ++x;

      // Increase free indices, sliced on the output tensor
    } while (it.nextSlicedFree());

    // Set the current slice of the output tensor
    t_out.setSlice(it.getSliceOut(), data_out);

  } while (it.nextNonSlicedFree());

  // Insert the new tensor into the collection
  tensors.insert(make_pair(tensor_out,t_out));

}

template <class Key>
void Contraction<Key>::contract(Key tensor1, Key tensor2,
                                const std::vector<
                                    std::pair<int, int>>& idx,
                                Key tensor_out) {

  /*
   * Contractions on the form
   *   A_abc B_cd -> C_abd
   * The specifics of the computation depends on the number of contracted
   * indices:
   *
   * If there is only one contracted index, we compute a slice of C at a time
   * as a matrix multiplication:
   *    C_*0* = A_*0* B_**, C_*1* = A_*1* B_**, ...
   *
   * If there is more than one contracted index, we compute one element of C
   * at a time from a (sum of) trace(s) of slices of A and B:
   *    A_abcde B_edcfg -> C_abfg,
   *
   *    C_0000 = tr(A_000** B_**000) + tr(A_001** B_**100) + ...
   *    C_1000 = tr(A_100** B_**000) + tr(A_101** B_**100) + ...
   *    ...
   *
   */

  // Get the tensors in question
  Tensor t1 = tensors.at(tensor1);
  Tensor t2 = tensors.at(tensor2);

  // Get the number of free indices and create the output tensor
  int free_out = t1.rank() + t2.rank() - 2*idx.size();
  Tensor t_out(free_out, t1.size());

  // Set up the iterator
  DoubleSliceIterator it(t1.rank(), t2.rank(), t1.size(), idx);
  // Figure out if we need to transpose matrices (sliced indices do not
  // change during iteration)
  int transpose_type = detectTranspose(it.getSlice1(), it.getSlice2());

  // Make ready arrays for matrix multiplication
  cdouble data1[t1.size() * t1.size()];
  cdouble data2[t2.size() * t2.size()];
  cdouble data_out[t_out.size() * t_out.size()];

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
          t1.getSlice(it.getSlice1(), data1);
          t2.getSlice(it.getSlice2(), data2);
          cx_mat m1(data1, t1.size(), t1.size());
          cx_mat m2(data2, t2.size(), t2.size());

          // Some types are degenerate, see comments in
          // contract(char,char,vector)
          if (transpose_type == 0 || transpose_type == 3)
            data_out[x] += trace(m1 * m2);
          else
            data_out[x] += trace(m1.st() * m2);

          // Increase the contracted non-sliced indices on input tensors
        } while (it.nextContracted());

        ++x;

        // Increase free indices, sliced on the output tensor
      } while (it.nextSlicedFree());

      // Set the current slice of the output tensor
      t_out.setSlice(it.getSliceOut(), data_out);

    } else {

      // Mult branch

      // Get the current slices in matrix form
      t1.getSlice(it.getSlice1(), data1);
      t2.getSlice(it.getSlice2(), data2);
      cx_mat m1(data1, t1.size(), t1.size());
      cx_mat m2(data2, t2.size(), t2.size());

      cx_mat mout; // The output slice in matrix form.
      // Do the multiplication based on transpose type
      if (transpose_type == 0)
        mout = m1 * m2;
      if (transpose_type == 1)
        mout = m1.st() * m2;
      if (transpose_type == 2)
        mout = m1 * m2.st();
      if (transpose_type == 3)
        mout = strans(m2 * m1);

      // Set the slice on the output tensor
      t_out.setSlice(it.getSliceOut(), mout.memptr());

    }

    // Increase the free indices not sliced on the output tensor
  } while (it.nextNonSlicedFree());

  // Insert the new tensor into the collection
  tensors.insert(make_pair(tensor_out,t_out));

}

/*
 * Add support for key values: char, int
 */
template class Contraction<char>;
template class Contraction<int>;

}
