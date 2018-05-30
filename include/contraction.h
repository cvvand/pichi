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
 * output tensor which has fewer combined indices or a number (no indices). A
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
 * Contracts indices on a single tensor, resulting in a new tensor. If all
 * indices are contracted, the output is a rank 0 tensor.
 */
Tensor contract(Tensor& tensor,
                const std::vector<std::pair<int, int>>& idx);


/*
 * Contract indices on two tensors. The resulting tensor is returned.
 */
Tensor contract(Tensor& tensor1, Tensor& tensor2,
                const std::vector<std::pair<int, int>>& idx);


}

#endif //PICHI_CONTRACTION_H
