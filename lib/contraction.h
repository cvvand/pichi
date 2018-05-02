#ifndef PICHI_CONTRACTION_H
#define PICHI_CONTRACTION_H

#include "tensor.h"
#include <unordered_map>
#include <queue>

namespace pichi {

/* ************************************************************************
 *
 * This file declares the functions used to contract tensors
 *
 * A contraction is a unary or binary tensor operation, in which one or more
 * indices on the input tensors are contracted in order to create a single
 * output tensor which has fewer combined indices or a number (no indices). An
 * simple example is matrix-matrix multiplication, which is the contraction
 * of a single index on two rank two tensors, creating a new rank two tensor.
 * Another example is the trace operation, which is a unary operation on a
 * rank 2 tensor.
 *
 * There are two contraction operations one can perform:
 *
 * 1) Contracting all indices on one or two tensors resulting in an output
 * number.
 * 2) Contracting some indices on one or two tensors, creating a new tensor
 * from the result.
 *
 * The input tensors are not modified during contractions (other than a
 * possible internal restructure of the data, which wont affect the actual
 * tensor or contractions).
 *
 * We tell the functions which indices are contracted in a list of pairs of
 * integers. If we contract a rank 3 tensor 'A' and a rank 5
 * tensor 'B', then the list {{0,1},{2,4}} means that we want to contract the
 * first index of A with the second index of B, as well as the third index of
 * A with the fifth index of B. This corresponds to the contraction
 *
 *    A_abc B_daefc
 *
 * The result of this contraction is a rank 4 tensor.
 *
 * ***********************************************************************/

/*
 * Contracts all indices on a single tensor, resulting in a number. For a
 * rank 2 tensor, this is just the trace.
 */
cdouble contract(Tensor& tensor,
                 const std::vector<std::pair<int, int>>& idx);

/*
 * Contract all indices on two tensors of equal rank. The resulting number
 * is returned in the end.
 * The input list of contracted indices must be the same length as the
 * tensor ranks. Returns the result.
 */
cdouble contract(Tensor& tensor1, Tensor& tensor2,
                 const std::vector<std::pair<int, int>>& idx);

/*
 * Contract a number of indices on a tensor. The resulting tensor data will
 * be inserted in tensor_out, which must have the correct rank and size
 * before the call.
 */
void contract(Tensor& tensor1, const std::vector<std::pair<int, int>>& idx,
              Tensor& tensor_out);

/*
 * Contract a number of indices on two tensors. The resulting tensor data will
 * be inserted in tensor_out, which must have the correct rank and size
 * before the call.
 */
void contract(Tensor& tensor1, Tensor& tensor2,
              const std::vector<std::pair<int, int>>& idx,
              Tensor& tensor_out);


}

#endif //PICHI_CONTRACTION_H
