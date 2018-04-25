
/* ****************************************************************************
 *
 * Implementation of the Tensor class defined in TENSOR.H
 *
 * ***************************************************************************/

#include <iostream>
#include "tensor.h"

using namespace std;

namespace pichi {

/*
 * Default constructor implementation
 * Creates a 2x2 tensor and initialises everything to 0.
 */
Tensor::Tensor() :
    dim(2), n(2), data(new cdouble[4]) {

  storage = {0,1};

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

  // Set default storage
  storage = vector<int>(rank);
  for (int i = 0; i < rank; ++i)
    storage[i] = i;

}

Tensor::Tensor(int rank, int size, const std::vector<int>& store) :
    Tensor(rank, size) {
  storage = store;
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

  // Copy storage information
  storage = other.storage;

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

  // Copy storage data
  storage = other.storage;

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

  // Copy storage data
  storage = other.storage;

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

cdouble Tensor::getElement(const std::vector<int>& index) const {
  // Translate index into offset by using storage information
  int os = 0;
  int mult = 1;
  for (int i = 0; i < dim; ++i) {
    os += mult*index[storage[i]];
    mult *= n;
  }
  return data[os];
}

void Tensor::setElement(const std::vector<int>& index, cdouble value) {
  // Translate index into offset by using storage information
  int os = 0;
  int mult = 1;
  for (int i = 0; i < dim; ++i) {
    os += mult*index[storage[i]];
    mult *= n;
  }
  data[os] = value;
}

void Tensor::getSlice(const std::vector<int>& slice, cdouble * buff) const {

  // Check if the data is correctly aligned:
  // The data is aligned correctly if the free indices of the slice
  // corresponds to the first two entries in the storage tensor in ascending
  // order
  bool aligned = true;
  if (storage[0] > storage[1])
    aligned = false;
  for (int i = 0; i < dim && aligned; ++i) {
    if (slice[i] < 0) {
      // Still aligned if i is in one of the first two entries in the storage
      aligned = ((i == storage[0]) || (i == storage[1]));
    }
  }

  // If  the data is aligned, simply copy the relevant part of the data pointer
  if (aligned) {
    // Find the offset
    int os = 0;
    int mult = n*n;
    for (int i = 2; i < dim; ++i) {
      os += slice[storage[i]]*mult;
      mult *= n;
    }

    // Copy from the offset and n*n elements forward
    std::copy(data+os, data + os + n*n, buff);

  } else { // If not, do it the slow way

    // Find the position of the running indices
    int r1, r2;
    bool flag = false;
    for (int i = 0; i < dim; ++i) {
      if (slice[i] < 0) {
        if (!flag) {
          r1 = i;
          flag = true;
        } else {
          r2 = i;
          break;
        }
      }
    }

    // Make a copy of the input slice vector with 0's on the running indices
    vector<int> cslice(slice);
    cslice[r1] = 0; cslice[r2] = 0;

    int idx = 0;
    for (int i = 0; i < n; ++i) {
      cslice[r2] = i;
      for (int j = 0; j < n; ++j) {
        cslice[r1] = j;
        buff[idx++] = getElement(cslice);
      }
    }

  }

}

void Tensor::setSlice(const std::vector<int>& slice, cdouble * buff) {

  // Check if the data is correctly aligned:
  // The data is aligned correctly if the free indices of the slice
  // corresponds to the first two entries in the storage tensor in ascending
  // order
  bool aligned = true;
  if (storage[0] > storage[1])
    aligned = false;
  for (int i = 0; i < dim && aligned; ++i) {
    if (slice[i] < 0) {
      // Still aligned if i is in one of the first two entries in the storage
      aligned = ((i == storage[0]) || (i == storage[1]));
    }
  }

  // If  the data is aligned, simply copy the relevant part of the data pointer
  if (aligned) {
    // Find the offset
    int os = 0;
    int mult = n*n;
    for (int i = 2; i < dim; ++i) {
      os += slice[storage[i]]*mult;
      mult *= n;
    }

    // Copy from n*n elements from the buffer to the offset in the data
    std::copy(buff, buff + n*n, data + os);

  } else { // If not, do it the slow way

    // Find the position of the running indices
    int r1, r2;
    bool flag = false;
    for (int i = 0; i < dim; ++i) {
      if (slice[i] < 0) {
        if (!flag) {
          r1 = i;
          flag = true;
        } else {
          r2 = i;
          break;
        }
      }
    }

    // Make a copy of the input slice vector with 0's on the running indices
    vector<int> cslice(slice);
    cslice[r1] = 0; cslice[r2] = 0;

    int idx = 0;
    for (int i = 0; i < n; ++i) {
      cslice[r2] = i;
      for (int j = 0; j < n; ++j) {
        cslice[r1] = j;
        setElement(cslice, buff[idx++]);
      }
    }

  }

}

vector<int> Tensor::getStorage() const {
  return storage;
}

void Tensor::setStorage(const std::vector<int>& store) {
  // Check that the storage is different
  if (store != storage) {
    // Create a new data array and assign with new storage info
    int total_size = 1;
    for (int i = 0; i < dim; ++i)
      total_size *= n;

    cdouble *ndata = new cdouble[total_size];
    vector<int> index(dim, 0);
    bool flag = false;
    int idx = 0;
    while (!flag) {
      // Insert element at the current index
      ndata[idx++] = getElement(index);

      // Increase the current index
      flag = true;
      for (int i = 0; i < dim && flag; ++i) {
        // Increase index store[i]
        int z = ++index[store[i]];
        if (z == n) {
          // We need to roll over
          index[store[i]] = 0;
        }
        else // Index increased, escape loop and continue with next element
          flag = false;
      }
    }

    // Use the new data pointer
    delete[] data;
    data = ndata;

  }
  storage = store;
}

}