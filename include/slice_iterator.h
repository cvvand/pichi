#ifndef PICHI_SLICE_ITERATOR_H
#define PICHI_SLICE_ITERATOR_H

#include <vector>
#include "tensor.h"

namespace pichi {

/* ************************************************************************
 *
 * This file declares a way to iterate through slices of a tensor when doing
 * contractions.
 *
 * Depending on the type of contractions, there are 1, 2 or 3 slices we need
 * to keep track of. There can be 1 or 2 input tensors and there may or may
 * not be an output tensor. On the input slices there are 4 types of indices:
 *
 *   - Sliced and contracted indices on input tensors (SC)
 *   - Non-sliced and contracted indices on input tensors (NC)
 *   - Free indices, sliced on the output tensor (SF)
 *   - Free indices, not sliced on the output tensor (NF)
 *
 * As an example, consider the contraction
 *
 * A_abcdef B_fedg = C_abcg
 *
 * Here we contract the three indices d,e and f. We could slice the input
 * tensors along indices d and e, while the output tensor is sliced along
 * indices a and g. In that case, the index types on tensor A is
 *    (SF,NF,NF,SC,SC,NC).
 * On tensor B the structure is
 *    (NC,SC,SC,SF)
 * On the output tensor, if there is one, we only care whether an index is
 * sliced (S) or not (F). Therefore, the index structur on C is
 *    (S,F,F,S)
 * where the two S-indices correspond to the SF indices on the input tensors
 * and the F indices correspond to the NF indices.
 * The structure is similar for contractions involving only one input tensor
 * and for contractions without any output tensor.
 *
 * The sliced indices are recognised by being negative numbers. In the
 * example above, the actual initial slices would look like this
 *
 *      a b c d  e  f
 *  A: (0,0,0,-1,-2,0)
 *
 *      f e  d  g
 *  B: (0,-2,-1,0)
 *
 *      a  b c g
 *  C: (-1,0,0,-1)
 *
 *  We use different negative numbers on the input tensor slices in order to
 *  see which indices are contracted with which.
 *
 *  Advancing the SF indices once would only change the slice on A:
 *  A: (1,0,0,-1,-2,0)
 *
 *  Then, advancing the NF indices would change the slice on A and C:
 *  A: (1,1,0,-1,-2,0)
 *  C: (-1,1,0,-1)
 *
 *  Now advancing the NC indices would change the input tensor slices
 *  A: (1,1,0,-1,-2,1)
 *  B: (1,-2,-1,0)
 *
 *  Whenever there is a choice as to which index to assign which role, we
 *  always try to make the optimal choice based on the storage in the tensors.
 *  If possible we slice on the leading dimensions of the tensors to be able
 *  to quickly grab the data in one go. When this is not possible, a
 *  sub-optimal choice will have to be made.
 *  For now this optimisation is only present in the DoubleSliceItarator
 *
 * ***********************************************************************/

class DoubleSliceIterator {

public:

  /*
   * Initiates an iterator for two tensors of a given rank and size as well
   * as a list of contracted indices.
   * The contractions is a list of pairs of integers. Each pair corresponds
   * to a contraction of indices, where the first number is the index number
   * on tensor 1 and the second number is the index number on tensor 2. For
   * example, the contraction
   *
   *    A_abcd B_aebf
   *
   * Would look like
   *    {{0,0},{1,2}}
   * Since the index a is the first index on both tensors (hence {0,0}, and b
   * is the second index on the first tensor and the third index on the
   * second tensor.
   *
   */
  DoubleSliceIterator(const Tensor& t1, const Tensor& t2,
                      const std::vector<std::pair<int,int>>& contractions);

  /*
   * Gets the current slices
   */
  std::vector<int> getSlice1() const;
  std::vector<int> getSlice2() const;
  std::vector<int> getSliceOut() const;

  /*
   * Advances the non-sliced contracted indices (NC) on the two input tensors.
   * Returns false if this returns the NC indices to their initial state.
   * Otherwise returns true.
   */
  bool nextContracted();

  /*
   * Advances the free indices which are sliced on the output tensor (SF).
   * Returns false if this returns the SF indices to their initial state.
   * Otherwise returns true.
   */
  bool nextSlicedFree();

  /*
   * Advances the free indices which are not sliced on the output tensor (NF).
   * Returns false if this returns the NF indices to their initial state.
   * Otherwise returns true.
   */
  bool nextNonSlicedFree();

private:

  // Size of the tensors
  int size;

  // List of SF indices on input# tensors
  std::vector<int> sf1;
  std::vector<int> sf2;

  // List of NF indices on {input#, output} tensors
  std::vector<std::pair<int,int>> nf1;
  std::vector<std::pair<int,int>> nf2;

  // List of NC indices on the {input1,input2} tensors
  std::vector<std::pair<int,int>> nc;

  // Current state of the slices
  std::vector<int> slice1;
  std::vector<int> slice2;
  std::vector<int> slice_out;

};



class SingleSliceIterator {

public:

  /*
   * Initiates an iterator for one tensor of a given rank and size as well
   * as a list of contracted indices.
   * The contractions is a list of pairs of integers. Each pair corresponds
   * to a contraction of indices. For example, the contraction
   *
   *    A_aabbcd
   *
   * Would look like
   *    {{0,1},{2,3}}
   */
  SingleSliceIterator(const Tensor&,
                      const std::vector<std::pair<int,int>>& contractions);

  /*
   * Gets the current slices
   */
  std::vector<int> getSlice1() const;
  std::vector<int> getSliceOut() const;

  /*
   * Advances the non-sliced contracted indices (NC) on the input tensor.
   * Returns false if this returns the NC indices to their initial state.
   * Otherwise returns true.
   */
  bool nextContracted();

  /*
   * Advances the free indices which are sliced on the output tensor (SF).
   * Returns false if this returns the SF indices to their initial state.
   * Otherwise returns true.
   */
  bool nextSlicedFree();

  /*
   * Advances the free indices which are not sliced on the output tensor (NF).
   * Returns false if this returns the NF indices to their initial state.
   * Otherwise returns true.
   */
  bool nextNonSlicedFree();

private:

  // Size of the tensors
  int size;

  // List of SF indices on the input tensor
  std::vector<int> sf;

  // List of NF indices on {input, output} tensors
  std::vector<std::pair<int,int>> nf;

  // List of NC indices on the input tensor
  std::vector<std::pair<int,int>> nc;

  // Current state of the slices
  std::vector<int> slice1;
  std::vector<int> slice_out;

};

}

#endif //PICHI_SLICE_ITERATOR_H
