
/* ****************************************************************************
 *
 * Implementation of the OPTensor class defined in OP_TENSOR.H
 *
 * ***************************************************************************/

#include "pichi/op_tensor.h"

using namespace std;

namespace pichi {

OPTensor::OPTensor(pichi::Tensor& tensor1, pichi::Tensor& tensor2) :
    t1(tensor1), t2(tensor2) {}

Tensor& OPTensor::getTensor1() {
  return t1;
}

Tensor& OPTensor::getTensor2() {
  return t2;
}

}