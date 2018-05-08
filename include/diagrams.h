
#ifndef PICHI_DIAGRAMS_H
#define PICHI_DIAGRAMS_H

#include "contraction.h"

namespace pichi {

cdouble diagram0(Tensor& t);

cdouble diagram1(Tensor& t1, const std::string& idx1,
                 Tensor& t2, const std::string& idx2);

cdouble diagram2(Tensor& t1, const std::string& idx1,
                 Tensor& t2, const std::string& idx2,
                 Tensor& t3, const std::string& idx3);

cdouble diagram3(Tensor& t1, const std::string& idx1,
                 Tensor& t2, const std::string& idx2,
                 Tensor& t3, const std::string& idx3,
                 Tensor& t4, const std::string& idx4);


}


#endif //PICHI_DIAGRAMS_H
