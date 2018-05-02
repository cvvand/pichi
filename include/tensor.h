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
 * This tensor class does not support tensors of rank 1 (vectors) or 0
 * (scalars). The minimum rank is 2.
 *
 * We interact with tensors in "slices". A slice is a 2-dimensional
 * piece of the tensor, which we can access and modify. This allows us to
 * "extract a matrix from a tensor", enabling the use of highly optimised
 * linear algebra code for doing tensor contractions.
 * A slice is identified by a vector of numbers. If we have a rank 5 tensor
 * of size 12, a possible "slice" of the tensor is
 *    (3, 11, *, 0, *)
 * The numbers in the tuple define the "fixed" indices of the tensor, while
 * the asterisks define the "running" indices.
 * For 2-dimensional tensors, the slices are simply the entire tensor.
 * For tensors of rank 3 or more, only 2-dimensional slices and be accessed.
 *
 * "Under the hood" the data in the tensor is stored in a single array. When
 * accessing a slice of the tensor, it is not always possible to simply copy
 * one consecutive piece of the underlying array. The way the data is stored
 * in the tensor can be accessed and changed and is indicated by an array.
 * For a rank 4 tensor, the following example "storage tensor"
 *   (2,3,0,1)
 * indicates that the leading dimension of the tensor is dimension 2, then 3
 * etc. This means that it is particularly easy to obtain the slices
 * (0,0,*,*) (and similarly with 0 changed to something else) since they are
 * already in the leading dimension of the underlying data array.
 * Default storage is (0,1,...,R-1), where R is the tensor rank.
 *
 *
 * ***********************************************************************/

class Tensor {

public:

  // --- Constructors, destructor, copying ------------------------------

  /*
   * Default constructor
   * Creates a 2x2 tensor with all elements set to 0. Uses default storage.
   */
  Tensor();

  /*
   * Creates a tensor with a given rank and size.
   * All elements in the tensor will be initialised to 0. Uses default storage.
   */
  Tensor(int rank, int size);

  /*
   * Creates a tensor with a given rank and size and with a set storage vector.
   * All elements in the tensor will be initialised to 0.
   */
  Tensor(int rank, int size, const std::vector<int>& store);

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
   * We assume always that the first running index is the leading dimension
   * of the array.
   *
   * For a rank 2 tensor, the slice will be the entire tensor.
   *
   * Note that if the tensor data is stored such that the slice lies along
   * the leading dimensions of the tensor, the functions will be able to copy
   * a large segment of the underlying data structure in one go and the call
   * will return more quickly. Consider setting the storage vector
   * appropriately before a getting a number of slices along the same
   * dimensions.
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
   */
  void getSlice(const std::vector<int>&, cdouble*) const;
  void setSlice(const std::vector<int>&, cdouble*);


  // --- Data storage ---------------------------------------------------

  /*
   * Gets or sets the storage vector.
   * The storage vector explains how the underlying data is stored. For a
   * storage vector like
   *    (3,1,0,2)
   * the leading dimension of the underlying data structure is dimension 3 of
   * the actual tensor, then dimension 1 etc.
   * When setting the storage vector, the data will be reallocated if the
   * storage is different from the current one.
   */
  std::vector<int> getStorage() const;
  void setStorage(const std::vector<int>& store);



  // --- Tensor details --------------------------------------------------

  /*
   * Gets the rank of the tensor.
   */
  int rank() const { return dim; };

  /*
   * Gets the size of the tensor (length of each dimension)
   */
  int size() const { return n; };

private: // --------------------------------------------------------------

  /* Initialise the tensor with a given rank and size */
  void init(int rank, int size);

  /* Set or get a single element in the tensor by index */
  cdouble getElement(const std::vector<int>& index) const;
  void setElement(const std::vector<int>& index, cdouble value);

  /* The dimensions of the tensor */
  int dim;
  int n;

  /* The actual data in the tensor */
  cdouble* data;

  /* Storage information on data */
  std::vector<int> storage;
};

}

#endif //PICHI_TENSOR_H
