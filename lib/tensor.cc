
/* ****************************************************************************
 *
 * Implementation of the Tensor class defined in TENSOR.H
 *
 * ***************************************************************************/

#include "tensor.h"

using namespace std;

namespace pichi {

/*
 * Default constructor implementation
 * Creates a 2x2 tensor and initialises everything to 0.
 */
Tensor::Tensor() :
    dim(2), n(2), data(new cdouble[4]) {

  data[0] = 0.0; data[1] = 0.0;
  data[2] = 0.0; data[3] = 0.0;

}

/*
 * Create a tensor with a given rank and size and initialise everything to 0
 */
Tensor::Tensor(int rank, int size) :
    dim(rank), n(size) {

  // Get the total number of components of the tensor ( size^rank )
  int total_size = 1;
  for (int i = 0; i < dim; ++i)
    total_size *= n;

  // Allocate the data for the tensor and initialise everything to 0.
  data = new cdouble[total_size];
  for (int i = 0; i < total_size; ++i)
    data[i] = 0.0;

}

/*
 * Copy constructor
 * Makes a deep copy of the input tensor.
 */
Tensor::Tensor(const Tensor& other) :
    dim(other.dim), n(other.n) {

  // Having copied the dimensions, calculate the total size of the tensor.
  int size = 1;
  for (int i = 0; i < dim; i++)
    size *= n;

  // Allocate the space for the data and copy the numbers from the input tensor.
  data = new cdouble[size];
  std::copy(other.data, other.data + size, data);

}

/*
 * Move constructor.
 * Moves the data from the input tensor to this one.
 */
Tensor::Tensor(Tensor&& other) noexcept {
  // Get the dimensions from the input.
  dim = other.dim;
  n = other.n;
  // Simply grab the data pointer.
  data = other.data;

  // Set the dimension vector of the input to be empty.
  other.dim = 0;
  // Make the input data pointer point to NULL. This is safe, since we
  // already got the pointer to the actual data.
  other.data = nullptr;
}

/*
 * Move assignment.
 * Moves the data from the input vector to this one.
 */
Tensor& Tensor::operator=(Tensor&& other) noexcept {
  // First: avoid problems where we try to move from this to this.
  if (this == &other) {
    return *this;
  }

  // Get the right dimensions from the input.
  dim = other.dim;
  n = other.n;

  // Delete our data and grab the input data pointer.
  delete[] data;
  data = other.data;

  // Clear the input dimension vector and data pointer. This is safe since we
  // already grabbed the pointer to the actual data.
  other.dim = 0;
  other.data = nullptr;

  return *this;
}

/*
 * Copy assignment
 * Makes a deep copy of the input tensor.
 */
Tensor& Tensor::operator=(const Tensor& other) {
  Tensor tmp(other); // Make deep copy of input tensor.
  *this = move(tmp); // Make this point to the new copy.
  return *this;
}

/*
 * Destructor
 */
Tensor::~Tensor() {
  delete[] data;
}


void Tensor::getSlice(const std::vector<int>& slice, cdouble * buff) {

  // TODO Fix this mess

  if (dim == 0)
    return;
  if (dim == 1 || dim == 2) {
    int size = 1;
    for (int i = 0; i < dim; ++i)
      size *= n;
    std::copy(data, data + size, buff);
    return;
  }
  bool foundOne = false;
  int iCoord = 0;
  int mult = 1;
  int mult1,mult2;
  for (int i = 0; i < dim; ++i) {
    if (slice[i] < 0) {
      if (!foundOne) {
        mult1 = mult;
        foundOne = true;
      }
      else {
        mult2 = mult;
      }
    } else {
      iCoord += mult*slice[i];
    }
    mult *= n;
  }
  int idx = 0;
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      buff[idx++] = data[iCoord + j*mult1 + i*mult2];
    }
  }

}

void Tensor::setSlice(const std::vector<int>& slice, cdouble * buff) {

  // TODO Fix this mess as well

  if (dim == 0)
    return;
  if (dim == 1 || dim == 2) {
    int size = 1;
    for (int i = 0; i < dim; ++i)
      size *= n;
    std::copy(buff, buff + size, data);
    return;
  }
  bool foundOne = false;
  int iCoord = 0;
  int mult = 1;
  int mult1,mult2;
  for (int i = 0; i < dim; ++i) {
    if (slice[i] < 0) {
      if (!foundOne) {
        mult1 = mult;
        foundOne = true;
      }
      else {
        mult2 = mult;
      }
    } else {
      iCoord += mult*slice[i];
    }
    mult *= n;
  }
  int idx = 0;
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      data[iCoord + j*mult1 + i*mult2] = buff[idx++];
    }
  }

}

}