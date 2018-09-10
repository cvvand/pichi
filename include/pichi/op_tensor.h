#ifndef PICHI_OP_TENSOR_H
#define PICHI_OP_TENSOR_H

#include "tensor.h"

namespace pichi {

class OPTensor {

public:
  OPTensor(Tensor& tensor1, Tensor& tensor2);

  Tensor& getTensor1();
  Tensor& getTensor2();

private:
  Tensor t1;
  Tensor t2;

};

}

#endif //PICHI_OP_TENSOR_H
