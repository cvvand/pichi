#include <armadillo>
#include "diagrams.h"
#include "contraction.h"
#include "tensor.h"

using namespace arma;
using namespace std;

namespace pichi {

cdouble diagram0(Tensor& t) {
  return contract(t,{{0,1}});
}

cdouble diagram1(Tensor& t1, const std::string& idx1,
                 Tensor& t2, const std::string& idx2) {

  if (idx2[0] == idx1[0] && idx2[1] == idx1[1]) {
    return contract(t1,t2, {{0,0},{1,1}});
  } else {
    return contract(t1,t2, {{0,1},{1,0}});
  }

}

cdouble diagram2(Tensor& t1, const std::string& idx1,
                 Tensor& t2, const std::string& idx2,
                 Tensor& t3, const std::string& idx3) {

  int i1 = 0;
  int i2 = 0;

  Tensor t4(2,t1.size());
  if (idx1[0] == idx2[0]) {
    contract(t1, t2, {{0, 0}}, t4);
    i1 = 1; i2 = 1;
  }
  else if (idx1[0] == idx2[1]) {
    contract(t1, t2, {{0, 1}}, t4);
    i1 = 1; i2 = 0;
  }
  else if (idx1[1] == idx2[0]) {
    contract(t1, t2, {{1, 0}}, t4);
    i1 = 0; i2 = 1;
  }
  else if (idx1[1] == idx2[1]) {
    contract(t1, t2, {{1, 1}}, t4);
    i1 = 0; i2 = 0;
  }

  if (idx1[i1] == idx3[1] && idx2[i2] == idx3[0])
    return contract(t4,t3,{{0,1},{1,0}});
  else
    return contract(t4,t3,{{0,0},{1,1}});

}

cdouble diagram3(Tensor& t1, const std::string& idx1,
                 Tensor& t2, const std::string& idx2,
                 Tensor& t3, const std::string& idx3,
                 Tensor& t4, const std::string& idx4) {

  int i1 = 0;
  int i2 = 0;

  Tensor t5(2,t1.size());
  if (idx1[1] == idx2[0]) {
    contract(t1, t2, {{1, 0}}, t5);
    i1 = 0; i2 = 1;
  } else {
    contract(t1, t2, {{1, 1}}, t5);
    i1 = 0; i2 = 0;
  }

  Tensor t6(2,t1.size());

  if (idx2[i2] == idx3[0]) {
    contract(t5, t3, {{1, 0}}, t6);
  } else {
    contract(t5, t3, {{0, 1}}, t6);
  }
  return contract(t6,t4,{{1,0},{0,1}});

}

}

