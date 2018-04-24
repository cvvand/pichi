#ifndef PICHI_CONTRACTION_H
#define PICHI_CONTRACTION_H

#include "tensor.h"
#include <unordered_map>
#include <queue>

namespace pichi {

/* ************************************************************************
 *
 * This file declares the Contraction class.
 *
 * A contraction is a unary or binary tensor operation, in which one or more
 * indices on the input tensors are contracted in order to create a single
 * output tensor which has fewer combined indices. An simple example is
 * matrix-matrix multiplication, which is the contraction of a single index
 * on two rank two tensors, creating a new rank two tensor. Another example
 * is the trace operation, which is a unary operation on a rank 2 tensor.
 *
 * To contract tensors using this class, they have to be added to the
 * collection with a name. After this there are two contraction
 * operations one can perform:
 *
 * 1) Contracting all indices on one or two tensors in the collection,
 * resulting in an output number.
 * 2) Contracting some indices on two tensors in the collection, creating a
 * new tensor from the result. The result is added to the collection, so it
 * can be used for more contractions later. This gives no output.
 *
 * The input tensors in the collection can be reused after a contraction in
 * all three cases. They are not modified (other than a possible internal
 * restructure of the data, which wont affect the actual tensor or
 * contractions).
 *
 * We tell the class which indices are contracted in a list of pairs of
 * integers. If we have in our collection a rank 3 tensor 'A' and a rank 5
 * tensor 'B', then the list {{0,1},{2,4}} means that we want to contract the
 * first index of A with the second index of B, as well as the third index of
 * A with the fifth index of B. This corresponds to the contraction
 *
 *    A_abc B_daefc
 *
 * The result of this contraction is a rank 4 tensor, which is then inserted
 * into the collection. The same structure is used when contracting tensors
 * to numbers.
 *
 * ***********************************************************************/

template <class Key>
class Contraction {

public:

  /*
   * Add a tensor to the collection with a given name. The name has to be
   * unique.
   */
  void addTensor(Key name, Tensor& tensor);

  /*
   * Get a tensor reference from the collection. This can be used to modify
   * the tensor after insertion.
   */
  Tensor& getTensor(Key tensor);

  /*
   * Remove a tensor from the collection, if it exists.
   */
  void removeTensor(Key tensor);

  /*
   * Contracts all indices on a single tensor, resulting in a number. For a
   * rank 2 tensor, this is just the trace.
   */
  cdouble contract(Key tensor,
                   const std::vector<std::pair<int, int>>& idx) const;

  /*
   * Contract all indices on two tensors of equal rank. The resulting number
   * is returned in the end.
   * The input list of contracted indices must be the same length as the
   * tensor ranks. Returns the result
   */
  cdouble contract(Key tensor1, Key tensor2,
                const std::vector<std::pair<int, int>>& idx) const;

  /*
   * Contract a number of indices on two tensors in the collection. The
   * resulting tensor is added to the collection with a given name.
   */
  void contract(Key tensor1, Key tensor2,
                const std::vector<std::pair<int,int>>& idx, Key tensor_out);

private:

  // The collection of tensors
  std::unordered_map<Key, Tensor> tensors;

  // Detect whether a slice needs to be transposed before matrix multiplication.
  int detectTranspose(const std::vector<int>& slice1,
                      const std::vector<int>& slice2) const;

};

}

#endif //PICHI_CONTRACTION_H
