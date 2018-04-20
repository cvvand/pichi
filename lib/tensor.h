#ifndef PICHI_TENSOR_H
#define PICHI_TENSOR_H

#include <complex>
#include <vector>

typedef std::complex<double> cdouble;

namespace pichi {

/* ************************************************************************
 *
 * This file declares the Tensor class.
 *
 * A tensor is characterised by a rank and a size. The rank is its
 * dimensionality, and the size is how many elements it has along each
 * dimension. Here, a tensor is always "square" i.e., the size of each
 * dimension of any given tensor is the same.
 * As an example, a square matrix of size N x N is a tensor of rank 2 and
 * size N.
 *
 * We interact with tensors in "slices". A slice is a 1 or 2-dimensional
 * piece of the tensor, which we can access and modify. This allows us to
 * "extract a matrix from a tensor", enabling the use of highly optimised
 * linear algebra code for doing tensor contractions.
 * A slice is identified by a vector of numbers. If we have a rank 5 tensor
 * of size 12, a possible "slice" of the tensor is
 *    (3, 11, *, 0, *)
 * The numbers in the tuple define the "fixed" indices of the tensor, while
 * the asterisks define the "running" indices.
 * For 1 and 2-dimensional tensors, the slices are simply the entire tensor.
 * For tensors of rank 3 or more, only 2-dimensional slices and be accessed.
 *
 *
 * ***********************************************************************/

class Tensor {

public:

  // --- Constructors, destructor, copying ------------------------------

  /*
   * Default constructor
   * Creates a 2x2 tensor with all elements set to 0
   */
  Tensor();

  /*
   * Creates a tensor with a given rank and size.
   * All elements in the tensor will be initialised to 0.
   */
  explicit Tensor(int rank, int size);

  /*
   * Copy constructor
   * Makes a deep copy of the input tensor. After the copying, the data of
   * the two tensors will point to two different addresses. The input tensor
   * is not modified.
   */
  Tensor(const Tensor&);

  /*
   * Move constructor
   * Moves the data from the input tensor to a new one. The input vector will
   * be rank 0 after the call finished and the data will
   * point to NULL.
   */
  Tensor(Tensor&&) noexcept;

  /*
   * Copy assignment
   * Copies the data and layout of the input tensor into this one. This
   * will also change the rank of this tensor to be equal to that of
   * the input. The data of the input vector is copied in a deep way, meaning
   * that after the call, the data pointers will point to two different
   * addresses. The input tensor is not modified.
   */
  Tensor& operator=(const Tensor&);

  /*
   * Move assignment
   * Moves the data and layout of the input tensor into this one. After the
   * call finishes the input tensor will be rank 0 and its data pointer will
   * point to NULL. All the original data in the input will be inherited by
   * this.
   */
  Tensor& operator=(Tensor&&) noexcept;

  /*
   * Destructor
   * Deletes all the data held by the tensor.
   */
  ~Tensor() noexcept;



  // --- Data ------------------------------------------------------------

  /*
   * Set or get a 2D slice of the tensor.
   * This is the intended way to get or set the data in the tensor. The first
   * input describes the slice of the tensor that we want. It should
   * have as many entries as the tensor has dimensions. The slice in question
   * is characterised by 2 running indices and n-2 fixed indices, where n is
   * the tensor rank.
   * Ex:
   * For a rank 3 tensor, we could have {*,1,*}. Here the second index is
   * fixed to the number 1 while the other two indices are running. This two
   * dimensional slice of the tensor will be read/written.
   *
   * In the actual input the * above should be replaced by a negative number,
   * signifying a running index.
   *
   * For a rank 0 tensor, this call has no effect.
   *
   * For a rank 1 or a rank 2 tensor, the slice will be the entire tensor.
   *
   * GET: The tensor data will be copied to the input buffer and decoupled from
   * the actual tensor. Therefore, any changes made to the buffer will not
   * affect the tensor or any of its other slices, and any changes made to
   * the tensor will not modify the data in the buffer.
   *
   * SET: When the call returns the data written to the tensor will be
   * decoupled from the input buffer. Later changes made to the tensor will
   * not modify the data in the buffer, and changes made to the buffer will
   * have no effect on the tensor.
   *
   * During the calls the data in the tensor might be restructured in order to
   * facilitate similar calls at a later time. The runtime of this function
   * can therefore vary a lot.
   */
  void getSlice(const std::vector<int>&, cdouble*);
  void setSlice(const std::vector<int>&, cdouble*);


  // --- Tensor details --------------------------------------------------

  /*
   * Gets the rank of the tensor.
   */
  int rank() const { return dim; };

  /*
   * Gets the size of the tensor (length of each dimension)
   */
  int size() const { return n; };

private:

  /* The dimensions of the tensor */
  int dim;
  int n;

  /* The actual data in the tensor */
  cdouble* data;

};

}

#endif //PICHI_TENSOR_H
